#include "packet_interface.h"

/* Extra #includes */
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

/* Extra code */
/* Your code will be inserted here */

pkt_t* pkt_new()
{
	pkt_t *packet=(pkt *) malloc(sizeof(pkt));
	if(pkt==NULL)
    {
        return NULL;
    }
    else
    {
        pkt->payload=NULL;
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
	char temp;
	temp = *data;
	int type [3];
	int window [5];
	
	for (i = 0; i < 3; ++i) {
	  type[i] = (temp >> i) & 1;
	}
	for (i = 0; i < 5; ++i) {
	  window [i] = (temp >> i) & 1;
	}
}

pkt_status_code pkt_encode(const pkt_t* pkt, char *buf, size_t *len)
{

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
    if()
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
    return pkt->.window;
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
