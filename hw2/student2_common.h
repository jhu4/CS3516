#ifndef STUDENT2_COMMON_H
#define STUDENT2_COMMON_H

struct msg;
struct pkt;

//ABP------------------------------------------------------------------- 
#define WAIT_TIME 50
#define BUFFER_SIZE 50

enum Astates{
	WaitForCall0,
	WaitForACK0,
	WaitForCall1,
	WaitForACK1
};

enum Bstates{
	WaitForPkt0,
	WaitForPkt1
};

typedef struct{
	struct msg* data;
	struct msg* front;
	struct msg* next;
	int size;
}Queue;


//global state
enum Bstates global_Bstate;
enum Astates global_Astate;
Queue A_queue;

void A_changestate(enum Astates state);
void B_changestate(enum Bstates state);


int iscorrupted(struct pkt packet);
int calculate_checksum(int acknum, int seqnum,char* data);
struct pkt makepkt(int acknum, int seqnum,struct msg data);
struct pkt makeACK(int acknum, int seqnum);
struct msg makemsg(char* data);

void initQueue(Queue* q);
int isEmpty(Queue* q);
int isFull(Queue* q);
void enqueue(Queue* q,struct msg data);
void dequeue(Queue* q);
struct msg peek(Queue* q);


#endif