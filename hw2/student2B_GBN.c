#include <stdio.h>
#include <stdlib.h>
#include "project2.h"
#include "student2_common.h"

/* 
 * The following routine will be called once (only) before any other   
 * entity B routines are called. You can use it to do any initialization 
 */
void B_init() {
	B_expecting_seqnum=1;
}

/*
 * B_input(packet),where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the A-side (i.e., as a result 
 * of a tolayer3() being done by a A-side procedure) arrives at the B-side. 
 * packet is the (possibly corrupted) packet sent from the A-side.
 */
void B_input(struct pkt packet) {
	//printf("------------------------------------------------------------B expecting %d, receiveing %d\n", B_expecting_seqnum,packet.seqnum);
	//if the the packet is not corrupted and seqnum is the expected one
	if(!iscorrupted(packet) && packet.seqnum==B_expecting_seqnum){
		//send ACK back
		//printf("---------------------------------------------------------------B send ACK %d\n", packet.seqnum);
		tolayer3(1,makeACK(packet.seqnum,0));
		//deliver data to layer5 of B
		tolayer5(1,makemsg(packet.payload));
		B_expecting_seqnum++;
	}
	//send ACK only if the seq number is less than the expecting seq number and no more less
	//than a window's size
	else if(!iscorrupted(packet) && packet.seqnum>=B_expecting_seqnum-WINDOW_CAPACITY){
		//printf("-------------------------------------------------------B send ACK %d\n", B_expecting_seqnum-1);
		tolayer3(1,makeACK(B_expecting_seqnum-1,0));
	}

}

/*
 * B_timerinterrupt()  This routine will be called when B's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void  B_timerinterrupt() {
}

//------------------------------------------------------------------------------------------------
//useless in this project
void B_output(struct msg message)  {

}
