#include "packet_interface.h"

/* Extra #includes */
#include <math.h>
#include <stdlib.h>
#include <zlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

//Comment tester un invalid padding?
//Comment tester qu'il n'y a pas d'header?
//Comment tester qu'il est unconsistant ?

struct __attribute__((__packed__)) pkt {
    //ordre a importance?? cmt assigner l'ordre et le nombre de bit?
    ptypes_t type : 3;
    uint8_t window : 5;
    uint8_t seqnum;
    uint16_t length;
    char *payload;
    uint32_t crc;
};

int getibit(char c, int i)
{
     return ((c>>i) & 1);
}

pkt_t* pkt_new()
{
	pkt_t *packet=(pkt_t *) malloc(sizeof(pkt_t));
	if(packet==NULL)
    {
        return NULL;
    }
    else
    {
        packet->payload=NULL;
        return packet;
    }
}

void pkt_del(pkt_t *pkt)
{
    free(pkt->payload);
    free(pkt);
}

pkt_status_code pkt_decode(const char *data, const size_t len, pkt_t *pkt)
{
	uint16_t *l=(uint16_t *)(data+2);
	*l=ntohs(*l);
    	if(*l>512 || len>520)
    	{
        	pkt_del(pkt);
        	return E_LENGTH;
    	}
    	if(len<=4)
    	{
        	pkt_del(pkt);
       	 	return E_NOPAYLOAD;
    	}
    	if(len<*l)
    	{
        	pkt_del(pkt);
        	return E_NOHEADER;
    	}
    	else
    	{
        	pkt_set_length(pkt,*l);
    	}
    	uint16_t reste=*l%4;
    	pkt_set_payload(pkt,(data+4),*l+(4-reste));
    	char *test;
    	test=(char *)data;
    	uint32_t *crc=((uint32_t *)(data+(4+*l+4-reste)));
	*crc=ntohl(*crc);
    	uint32_t crc2=crc32(0,(const Bytef *)test,*l+4);
    	if(*crc!=crc2)
    	{
        	pkt_del(pkt);
        	return E_CRC;
    	}
    	else
    	{
        	pkt_set_crc(pkt,*crc);
    	}

    	pkt_set_seqnum(pkt,(uint8_t)*(data+2));
    //TYPE
    	uint8_t *header;
    	header=(uint8_t *)data;
    	int i,j;
    	ptypes_t type=0;
    	for (i = 0; i < 3; ++i) {
		int bit=getibit((char)*header,5+i);
  		if (bit) {type = type + pow (2, i);}
    	}
	if(type==PTYPE_DATA || type==PTYPE_ACK || type==PTYPE_NACK)
    	{
        	pkt_set_type(pkt,type);
    	}
	else
    	{
        	pkt_del(pkt);
        	return E_TYPE;
    	}
	//WINDOW
	uint8_t window=0;
	for (j = 0; j < 5; j++) {
        	int bit=getibit((char)*header,j)	;
        	if (bit) {window = window + pow (2, j);}
    	}
    	pkt_set_window(pkt,window);
    	return PKT_OK;
}

pkt_status_code pkt_encode(const pkt_t* pkt, char *buf, size_t *len)
{
    uint8_t window=pkt_get_window(pkt);
    uint16_t length=pkt_get_length(pkt);
    uint32_t crc=pkt_get_crc(pkt);
    uint8_t seqnum = pkt_get_seqnum(pkt);
	uint8_t type =(uint8_t)pkt_get_type(pkt);
printf(" ENCODE window : %hu,type: %d, crc : %u, length : %hu, seqnum : %hu\n",window,type,crc,length,seqnum);
    uint16_t reste=length%4;
	if(length+4+4+reste > (uint16_t)*len){ return E_NOMEM;}
	else{
        type=type<<5;
        uint8_t byteone=type|window;
	memcpy((buf),&byteone,1);
	memcpy((buf+1),&seqnum,1);
	uint16_t l2=htons(length);
        memcpy((buf+2),&l2,2);
	char *payload = (char *)pkt_get_payload(pkt);
        memcpy((buf+4), payload, length+4-reste);
	uint32_t c2=htonl(crc);
        memcpy((buf+length+4-reste+4),&c2,4);
        size_t l=length+8+4-reste;
        *len=l;
        return PKT_OK;
	}


}

pkt_status_code pkt_set_type(pkt_t *pkt, const ptypes_t type)
{
        pkt->type=type;
        return PKT_OK;
}

pkt_status_code pkt_set_window(pkt_t *pkt, const uint8_t window)
{
        pkt->window=window;
        return PKT_OK;
}

pkt_status_code pkt_set_seqnum(pkt_t *pkt, const uint8_t seqnum)
{
        pkt->seqnum=seqnum;
        return PKT_OK;
}

pkt_status_code pkt_set_length(pkt_t *pkt, const uint16_t length)
{
        pkt->length=length;
        return PKT_OK;
}

pkt_status_code pkt_set_crc(pkt_t *pkt, const uint32_t crc)
{
        pkt->crc=crc;
        return PKT_OK;
}

pkt_status_code pkt_set_payload(pkt_t *pkt,
							    const char *data,
								const uint16_t length)

{
    if(length > 512){ return E_LENGTH;}
    else{
      	if(pkt->payload != NULL) {free((char *)pkt->payload);}
      	int realSize= length + 4-(length % 4) ;
      	pkt->payload=(char *) calloc(realSize,sizeof(char));
      	if (pkt->payload == NULL) {return E_NOMEM;}

      	memcpy(pkt->payload,(char *)data,length);
    	return PKT_OK;
    	}
}

ptypes_t pkt_get_type  (const pkt_t* pkt)
{
    return pkt->type;
}

uint8_t  pkt_get_window(const pkt_t* pkt)
{
    return pkt->window;
}

uint8_t  pkt_get_seqnum(const pkt_t* pkt)
{
    return pkt->seqnum;
}

uint16_t pkt_get_length(const pkt_t* pkt)
{
    return pkt->length;
}

uint32_t pkt_get_crc   (const pkt_t* pkt)
{
    return pkt->crc;
}

const char* pkt_get_payload(const pkt_t* pkt)
{
    return pkt->payload;
}
