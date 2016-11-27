#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student2_common.h"
#include "project2.h"

void A_changestate(enum Astates state){
	global_Astate=state;
}

void B_changestate(enum Bstates state){
	global_Bstate=state;
}

int iscorrupted(struct pkt packet){
	return FALSE;
}

int calculate_checksum(int acknum, int seqnum,char* data){
	return 0;
}


struct pkt* makepkt(int acknum, int seqnum,struct msg data){
	struct pkt* packet=malloc(sizeof(struct pkt));
	packet->seqnum=seqnum;
    packet->acknum=acknum;
    packet->checksum=calculate_checksum(acknum,seqnum,data.data);
    strncpy(packet->payload,data.data,MESSAGE_LENGTH);
    printf("Makepkt Payload: %s \n",packet->payload );
    return packet;
}

struct pkt makeACK(int acknum, int seqnum){
	char payload[20]="   ACK   ";
	struct pkt packet;
	packet.seqnum=seqnum;
	packet.acknum=acknum;
	packet.checksum=calculate_checksum(acknum,seqnum,NULL);
	strncpy(packet.payload,payload,MESSAGE_LENGTH);
	return packet;
}

struct msg makemsg(char* data){
	struct msg hi;
	strncpy(hi.data,data,MESSAGE_LENGTH);
	return hi;
}

//-----------------------------------------------------------Queue
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

struct msg peek(Queue* q){
	printf("Peek%s\n", (q->front)->data);
	return *q->front;
}
//-----------------------------------------------------------