#include <stdio.h>
#include "project3.h"
#include "share.h"

extern int TraceLevel;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt1;
struct NeighborCosts   *neighbor1;

static int mynum = 1;

/* students to write the following two routines, and maybe some others */
void rtinit1() {
	int i,j;
	for(i=0;i<MAX_NODES;i++){
		for(j=0;j<MAX_NODES;j++){
			dt1.costs[i][j]=INFINITY;
		}
	}
	neighbor1=getNeighborCosts(1);
	//initialize itself's costs to neighbors
	for(i=0;i<MAX_NODES;i++){
		dt1.costs[mynum][i]=neighbor1->NodeCosts[i];
	}
	for(int i=0;i<MAX_NODES;i++){
		//cannot be myself or node that is not my neighbor
		if(i!=mynum && neighbor1->NodeCosts[i]<INFINITY){
			toLayer2(*makepacket(mynum,i,&dt1.costs[mynum][0]));	
		}
	}
}


void rtupdate1( struct RoutePacket *rcvdpkt ) {
	//just in case that this is actually a msg for me
	if(rcvdpkt->destid==mynum){
		//update the list for the sender node
		int i;
		int isChanged=NO;
		for(i=0;i<MAX_NODES;i++){
			dt1.costs[rcvdpkt->sourceid][i]=rcvdpkt->mincost[i];
		}
		//update itself's min distance to a node
		for(i=0;i<4;i++){
			int minToNodeI = smallest(dt1.costs[mynum][0]+dt1.costs[0][i]
							,dt1.costs[mynum][1]+dt1.costs[1][i]
							,dt1.costs[mynum][2]+dt1.costs[2][i]
							,dt1.costs[mynum][3]+dt1.costs[3][i]);
			//if the min value changed, flip the flag
			if(dt1.costs[mynum][i]!=minToNodeI){
				isChanged=YES;
				dt1.costs[mynum][i] = (minToNodeI<INFINITY?minToNodeI:INFINITY);
			}
		}
		if(isChanged){
			//broadcast the changing news to its neighbor
			for(int i=0;i<MAX_NODES;i++){
				//cannot be myself or node that is not my neighbor
				if(i!=mynum && neighbor1->NodeCosts[i]<INFINITY){
					toLayer2(*makepacket(mynum,i,&dt1.costs[mynum][0]));	
				}
			}
		}

	}
}


/////////////////////////////////////////////////////////////////////
//  printdt
//  This routine is being supplied to you.  It is the same code in
//  each node and is tailored based on the input arguments.
//  Required arguments:
//  MyNodeNumber:  This routine assumes that you know your node
//                 number and supply it when making this call.
//  struct NeighborCosts *neighbor:  A pointer to the structure 
//                 that's supplied via a call to getNeighborCosts().
//                 It tells this print routine the configuration
//                 of nodes surrounding the node we're working on.
//  struct distance_table *dtptr: This is the running record of the
//                 current costs as seen by this node.  It is 
//                 constantly updated as the node gets new
//                 messages from other nodes.
/////////////////////////////////////////////////////////////////////
void printdt1( int MyNodeNumber, struct NeighborCosts *neighbor, 
		struct distance_table *dtptr ) {
    int       i, j;
    int       TotalNodes = neighbor->NodesInNetwork;     // Total nodes in network
    int       NumberOfNeighbors = 0;                     // How many neighbors
    int       Neighbors[MAX_NODES];                      // Who are the neighbors

    // Determine our neighbors 
    for ( i = 0; i < TotalNodes; i++ )  {
        if (( neighbor->NodeCosts[i] != INFINITY ) && i != MyNodeNumber )  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }
    // Print the header
    printf("                via     \n");
    printf("   D%d |", MyNodeNumber );
    for ( i = 0; i < NumberOfNeighbors; i++ )
        printf("     %d", Neighbors[i]);
    printf("\n");
    printf("  ----|-------------------------------\n");

    // For each node, print the cost by travelling thru each of our neighbors
    for ( i = 0; i < TotalNodes; i++ )   {
        if ( i != MyNodeNumber )  {
            printf("dest %d|", i );
            for ( j = 0; j < NumberOfNeighbors; j++ )  {
                    printf( "  %4d", dtptr->costs[i][Neighbors[j]] );
            }
            printf("\n");
        }
    }
    printf("\n");
}    // End of printdt1

