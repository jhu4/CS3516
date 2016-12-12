#include  "share.h"
#include "project3.h"
#include <stdlib.h>

typedef struct distance_table distanceTable;

struct RoutePacket* makepacket(int mynum, int destid,int* a){
	struct RoutePacket* rtpkt = malloc(sizeof(struct RoutePacket));
	rtpkt->sourceid=mynum;
	rtpkt->destid=destid;
	int i;
	for(i=0;i<MAX_NODES;i++){
		rtpkt->mincost[i]=a[i];
	}
	return rtpkt;
}

int min(int x, int y){
	return (x<y?x:y);
}

int smallest(int a, int b, int c, int d){
	return min(min(a,b),min(c,d));
}