#include "packet_interface.h"

/* Extra #includes */
#include "math.h"
#include <stdlib.h>
//#include <zlib.h>
/* Your code will be inserted here */

struct __attribute__((__packed__)) pkt {
    //ordre a importance?? cmt assigner l'ordre et le nombre de bit?
    uint32_t crc;
    const char *payload;
    uint16_t length;
    uint8_t seqnum;
    uint8_t window : 5;
	ptypes_t type : 3;
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
	/*char temp;
	temp = *data;
	int tempBit;
	int window [5];

	//Lecture Type et Mise en Paquet
	for (i = 0; i < 3; ++i) {
	  tempBit = (temp >> i) & 1;
	  if (type == 1){
	  	if(pkt->type != NULL) {return E_TYPE;} //Ce type n'existe pas (le bit 1 seulement possible une fois)
	  	if(i==0) {pkt->type = PTYPE_DATA; }
	  	else if(i==1) {pkt->type = PTYPE_ACK; }
	  	else {pkt->type = PTYPE_NACK; }
	  }
	}
	//Lecture Window et Mise en Paquet
	int window = 0;
        for (int i = 0; i < 5; i++) {
        	tempBit = (temp >> i+3) & 1;
        	if (tempBit) {window = window + pow (2, i);}
        }
        pkt->window = uint8_t(numeral);*/
    //char packet[520];

    pkt->length=(uint16_t)(*data+2);
    int l=pkt->length;
    pkt->payload=(char *)malloc((size_t)l);
    pkt->payload=data+4;
    pkt->crc=(uint32_t)(*data+(4+l));
    uint32_t crc2=crc32(0,pkt->payload,l+4);
    pkt->seqnum=(uint8_t)(*data+2);
    //TYPE
    char *header=data;
    int i,j;
    int type=0;
    for (i = 0; i < 3; ++i) {
	  int bit=getibit((char)*header,5+i);
	  if (bit) {type = type + pow (2, i);}
    }
	if(type==1 || type==2 || type==4){pkt->type=type;}
	else{return E_TYPE;}
	//WINDOW
	int window=0;
	for (j = 0; i < 5; j++) {
        int bit=getibit((char)*header,j)	;
        if (bit) {window = window + pow (2, j);}
    }
    pkt->window=(uint8_t)window;
}

pkt_status_code pkt_encode(const pkt_t* pkt, char *buf, size_t *len)
{
    return PKT_OK;
}

pkt_status_code pkt_set_type(pkt_t *pkt, const ptypes_t type)
{
    if (type==PTYPE_DATA || type==PTYPE_ACK || type==PTYPE_NACK)
    {
        pkt->type=type;
        return PKT_OK;
    }
    else
    {
        return E_TYPE;
    }
}

pkt_status_code pkt_set_window(pkt_t *pkt, const uint8_t window)
{
    if(window>=0 || window<=31)
    {
        pkt->window=window;
        return PKT_OK;
    }
    else
    {
        return E_WINDOW;
    }
}

pkt_status_code pkt_set_seqnum(pkt_t *pkt, const uint8_t seqnum)
{
    if(seqnum>=0 || seqnum<=255)
	{
        pkt->seqnum=seqnum;
        return PKT_OK;
	}
	else
    {
        return E_SEQNUM;
    }
}

pkt_status_code pkt_set_length(pkt_t *pkt, const uint16_t length)
{
    if(length>=0 || length <=512)
    {
        pkt->length=length;
        return PKT_OK;
    }
    else
    {
        return E_LENGTH;
    }
}

pkt_status_code pkt_set_crc(pkt_t *pkt, const uint32_t crc)
{
    if(1)
    {
        pkt->crc=crc;
        return PKT_OK;
    }
    else
    {
        return E_CRC;
    }
}

pkt_status_code pkt_set_payload(pkt_t *pkt,
							    const char *data,
								const uint16_t length)

{
	/* Your code will be inserted here */
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
