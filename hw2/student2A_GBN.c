#include <stdio.h>
#include <stdlib.h>
#include "project2.h"
#include "student2_common.h"

/* The following routine will be called once (only) before any other    */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
	initQueue(&A_queue);
	A_base_seqnum=1;
	A_next_seqnum=1;
 	A_total_seqnum=1;
 	A_init_flag=0;
}

/* 
 * A_output(message), where message is a structure of type msg, containing 
 * data to be sent to the B-side. This routine will be called whenever the 
 * upper layer at the sending side (A) has a message to send. It is the job 
 * of your protocol to insure that the data in such a message is delivered 
 * in-order, and correctly, to the receiving side upper layer.
 */
void A_output(struct msg message) {
	//save the msg to a queue and increment the total msg number
	enqueue(&A_queue,message);
	A_total_seqnum++;

	//if the window size is smaller than the window's capacity
	// and a msg is received from above, send the msg immediatly to B
	if(A_base_seqnum+WINDOW_CAPACITY>=A_total_seqnum){
		//printf("--------------------------------------------A expands window size, send pkt, seq %d, max %d\n",A_next_seqnum,A_total_seqnum);	
		tolayer3(0,makepkt(0,A_next_seqnum,shiftWindow(&A_queue)));
		//shiftWindow(&A_queue);
		if(A_base_seqnum==A_next_seqnum){
 			startTimer(0,WAIT_TIME);
		}
		A_next_seqnum++;
	}
}


/* 
 * A_input(packet), where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the B-side (i.e., as a result 
 * of a tolayer3() being done by a B-side procedure) arrives at the A-side. 
 * packet is the (possibly corrupted) packet sent from the B-side.
 */
void A_input(struct pkt packet) {
	//if recieve a ACK that is greater than or equals to base seqnum
	if(!iscorrupted(packet) && packet.acknum >= A_base_seqnum){
		stopTimer(0);
		//calculate how many slots the window need to shift
		int difference = packet.acknum+1 - A_base_seqnum;
		int i;
		for(i=0;i<difference;i++){
			//delete acked backup msgs
			dequeue(&A_queue);
			//printf("--------------------------------------------A receive ACK%d    difference %d\n",packet.acknum,difference);
			//printf("--------------------------------------------A dequeue pkt, seq %d, max %d\n",A_base_seqnum,A_total_seqnum);
			A_base_seqnum++;
			//if the next sequm number is smaller than total seqnum
			//shift the window and send more pkts to B
			if(A_next_seqnum<A_total_seqnum){
				//printf("--------------------------------------------A: sending more pkt, seq %d, max %d\n",A_next_seqnum,A_total_seqnum );
				tolayer3(0,makepkt(0,A_next_seqnum,shiftWindow(&A_queue)));
				A_next_seqnum++;
			}
		}
		startTimer(0,WAIT_TIME);
		
	}
	//doing nothing if the pkt is corrupted
}

/*
 * A_timerinterrupt()  This routine will be called when A's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void A_timerinterrupt() {
	//if there is unacked packet
	int diff = A_next_seqnum - A_base_seqnum;
	int i;
	for(i=0;i<diff;i++){
		//printf("--------------------------------------------A resending pkt, seq %d, max %d \n",A_base_seqnum+i,A_total_seqnum );
		tolayer3(0,makepkt(0,A_base_seqnum+i,peek(&A_queue,i)));
	}
	startTimer(0,WAIT_TIME);
}  

