#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student2_common.h"
#include "project2.h"

//state changing method, avoiding A or B to mess up with global state
void A_changestate(enum Astates state){
	global_Astate=state;
}

void B_changestate(enum Bstates state){
	global_Bstate=state;
}

//see if a pkt is corrupted
int iscorrupted(struct pkt packet){
	int pktchecksum=calculate_checksum(packet.acknum,packet.seqnum,packet.payload);
	// printf("----------------------calculated %d inner %d\n",pktchecksum,packet.checksum);
	if(packet.checksum==pktchecksum) return FALSE;
	return TRUE;
}


int calculate_checksum(int acknum, int seqnum,char* data){
	int checksum=0;
	checksum-=2*acknum-1;
	checksum+=3*seqnum+2;
	if(data==NULL){
		return checksum;
	}
	int i;
	for(i=0;i<MESSAGE_LENGTH;i++){
		checksum+=(int)data[i]*i-4*i;
	}
	return checksum;
}

//make a normal pkt with data
struct pkt makepkt(int acknum, int seqnum,struct msg data){
	struct pkt* packet=malloc(sizeof(struct pkt));
	packet->seqnum=seqnum;
    packet->acknum=acknum;
    packet->checksum=calculate_checksum(acknum,seqnum,data.data);
    strncpy(packet->payload,data.data,MESSAGE_LENGTH);
    return *packet;
}

//make a ACK pkt
struct pkt makeACK(int acknum, int seqnum){
	char payload[20]="ACK";
	struct pkt packet;
	packet.seqnum=seqnum;
	packet.acknum=acknum;
	packet.checksum=calculate_checksum(acknum,seqnum,payload);
	strncpy(packet.payload,payload,MESSAGE_LENGTH);
	return packet;
}

//make a msg
struct msg makemsg(char* data){
	struct msg hi;
	strncpy(hi.data,data,MESSAGE_LENGTH);
	return hi;
}

//-----------------------------------------------------------Queue functions
void initQueue(Queue* q){
	q->data= malloc(BUFFER_SIZE*sizeof(struct msg));
	q->front=q->data;
	q->next=q->data;
	q->size=0;
}

int isEmpty(Queue* q){
	return q->size==0;
}

int isFull(Queue* q){
	return q->size==BUFFER_SIZE;
}

//put msg from layer 5 to a Queue buffer
void enqueue(Queue* q,struct msg data){
	if(isFull(q)){
		printf("Queue is Full\n");
		return;
	}

	q->size++;

	strncpy((q->next)->data,data.data,MESSAGE_LENGTH);

	if(q->next==&q->data[BUFFER_SIZE]){
		q->next=q->data;
	}
	else{
		q->next++;
	}
}

//delete the front msg, usually is used when receive a proper ACK
void dequeue(Queue* q){
	if(isEmpty(q)){
		printf("Queue is Empty\n");
		return;
	}

	q->size--;

	if(q->front==&q->data[BUFFER_SIZE]){
		q->front=q->data;
	}
	else{
		q->front++;
	}
}

//peek the front msg, usually use for resend
struct msg peek(Queue* q){
	return *q->front;
}
//-----------------------------------------------------------
