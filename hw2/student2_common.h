#ifndef STUDENT2_COMMON_H
#define STUDENT2_COMMON_H

struct msg;
struct pkt;

//GBN------------------------------------------------------------------- 
#define WAIT_TIME 50
#define BUFFER_SIZE 50
#define WINDOW_CAPACITY 8



typedef struct{
	struct msg* data;
	struct msg* front;
	struct msg* next;
	struct msg* total;
	int size;
}Queue;



//msg Queue for A
Queue A_queue;

int B_expecting_seqnum;

int A_base_seqnum;
int A_next_seqnum;
int A_total_seqnum;
int A_init_flag;

int max(int a, int b);
int min(int a, int b);

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
struct msg peek(Queue* q,int index);
struct msg shiftWindow(Queue* q);
#endif
