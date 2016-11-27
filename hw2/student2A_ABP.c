#include <stdio.h>
#include <stdlib.h>
#include "project2.h"
#include "student2_common.h"

/* The following routine will be called once (only) before any other    */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
	global_Astate=WaitForCall0;
	initQueue(&A_queue);
}

/* 
 * A_output(message), where message is a structure of type msg, containing 
 * data to be sent to the B-side. This routine will be called whenever the 
 * upper layer at the sending side (A) has a message to send. It is the job 
 * of your protocol to insure that the data in such a message is delivered 
 * in-order, and correctly, to the receiving side upper layer.
 */
void A_output(struct msg message) {
	//put the message into the queue
	enqueue(&A_queue,message);
	printf("------------------------------A Queue front %s size %d\n",peek(&A_queue).data,A_queue.size);
	
	switch(global_Astate){
		case WaitForCall0:
			//send it immediatly
			printf("------------------------------A send PKT0 %s\n",message.data);
			tolayer3(0,*makepkt(0,0,message));
			startTimer(0,WAIT_TIME);
			A_changestate(WaitForACK0);
			break;
		case WaitForCall1:
			printf("------------------------------A send PKT1 %s\n",message.data);
			tolayer3(0,*makepkt(0,1,message));
			startTimer(0,WAIT_TIME);
			A_changestate(WaitForACK1);
			break;

		case WaitForACK0:
			//doing nothing
			break;
		case WaitForACK1:
			//doing nothing
			break;
		default:
			printf("Student2A_ABP A_output invalid state\n");
			break;
	}
}


/* 
 * A_input(packet), where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the B-side (i.e., as a result 
 * of a tolayer3() being done by a B-side procedure) arrives at the A-side. 
 * packet is the (possibly corrupted) packet sent from the B-side.
 */
void A_input(struct pkt packet) {
	switch(global_Astate){
		case WaitForACK0:
			if(!iscorrupted(packet) && packet.acknum==0){
				
				printf("------------------------------A Receive ACK0\n");
				//delete the pkt0 backup
				printf("------------------------------A Dequeue %s\n",peek(&A_queue).data);
				dequeue(&A_queue);
				//if the queue now is empty, wait for call 1 from above
				if(isEmpty(&A_queue)){
					A_changestate(WaitForCall1);
				}//if the queue has pkt
				else{
					//make a pkt1 and send it to B

					printf("------------------------------A send PKT1 %s\n",peek(&A_queue).data);
					tolayer3(0,*makepkt(0,1,peek(&A_queue)));
					startTimer(0,WAIT_TIME);
					A_changestate(WaitForACK1);
				}
			}
			break;
		case WaitForACK1:
			if(!iscorrupted(packet) && packet.acknum==1){

				printf("------------------------------A Receive ACK1 %s\n",peek(&A_queue).data);
				//delete the pkt1 backup
				dequeue(&A_queue);
				if(isEmpty(&A_queue)){
					A_changestate(WaitForCall0);
				}
				else{

					printf("------------------------------A send PKT0\n");
					tolayer3(0,*makepkt(0,0,peek(&A_queue)));
					startTimer(0,WAIT_TIME);
					A_changestate(WaitForACK0);
				}
			}
			break;
		case WaitForCall0:
		case WaitForCall1:
			//doint nothing
			break;
		default:
			printf("Student2A_ABP A_input invalid state\n");
			break;
	}
}

/*
 * A_timerinterrupt()  This routine will be called when A's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void A_timerinterrupt() {

	switch(global_Astate){
		case WaitForACK0:
			stopTimer(0);
			//resend pkt to B
			printf("------------------------------A Resend PKT0\n");
			tolayer3(0,*makepkt(0,0,peek(&A_queue)));
			startTimer(0,WAIT_TIME);
			break;
		case WaitForACK1:
			stopTimer(0);
			//resend pkt to B
			printf("------------------------------A Resend PKT1\n");
			tolayer3(0,*makepkt(0,1,peek(&A_queue)));
			startTimer(0,WAIT_TIME);
			break;
		case WaitForCall0:
		case WaitForCall1:
		default:
			printf("Student2A_ABP A_timerinterrupt invalid state\n");
			break;	
	}
}  

