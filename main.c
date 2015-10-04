
#include <stdio.h>
#include <stdlib.h>
#include "packet_interface.h"
/*
struct __attribute__((__packed__)) pkt {
	ptypes_t type;//check part of the enum??
	uint8_t window; //check smaller than 32
	uint8_t seqnum; //check smaller than 255, mais normalement donc c'est bon
	uint16_t length;//plus petit que 512 is in BYTES
	uint32_t crc;
	char* data;//supposee allouee en un block
};*/
void print_pkt(pkt_t* pack)
{
	printf("-----Packet:\n");
	printf("type  =%d\n",pkt_get_type(pack));
	printf("window=%hu\n",pkt_get_window(pack));
	printf("seq   =%hu\n",pkt_get_seqnum(pack));
	int length = pkt_get_length(pack);
	printf("length=%hu\n",length);
	length = (length%4==0)? length: length+4-(length%4);
	int i;
	const char * data = pkt_get_payload(pack);
	if(data!=NULL){
		for(i=0;i<length;++i){
			if(i%4==0) printf("\n");
			printf("%3.c;",data[i]);
		}printf("\n");
	}
	else printf("NULL\n");
	printf("crc   =%u\n",pkt_get_crc(pack));
}
int main(int argc, char** argv)
{
	pkt_t* pack = pkt_new();
	if(pkt_set_type(pack,(ptypes_t)PTYPE_ACK)) printf("invalid command\n");
	if(pkt_set_window(pack,10)) printf("invalid command\n");
	if(pkt_set_seqnum(pack,128)) printf("invalid command\n");
	char line[]= "12345678901";
	if(pkt_set_payload(pack,line,11)) printf("invalid command\n");

	char buff[30]; size_t len = 30;
	print_pkt(pack);
	if(pkt_encode(pack,buff,&len)) printf("-----error encoding-----\n");
	print_pkt(pack);
	pkt_del(pack);

	pkt_t * pack2 = pkt_new();
	if(pkt_decode(buff,len,pack2)) printf("-----error decoding-----\n");
	print_pkt(pack2);
	pkt_del(pack2);
	return 0;
}










