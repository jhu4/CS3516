#include <stdio.h>
#include <stdlib.h>
#include "project2.h"
#include "student2_common.h"

/* 
 * The following routine will be called once (only) before any other   
 * entity B routines are called. You can use it to do any initialization 
 */
void B_init() {
	global_Bstate=WaitForPkt0;
}

/*
 * B_input(packet),where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the A-side (i.e., as a result 
 * of a tolayer3() being done by a A-side procedure) arrives at the B-side. 
 * packet is the (possibly corrupted) packet sent from the A-side.
 */
void B_input(struct pkt packet) {
	switch(global_Bstate){
		case WaitForPkt0:
			if(!iscorrupted(packet) && packet.seqnum==0){

			}
			else if(!iscorrupted(packet) && packet.seqnum==1){

			}
			//if is it corrupted, doing nothing
			break;
		case WaitForPkt1:
			break;
		default:
			printf("student2B_APB.c B_input invalid state\n");
			break;
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