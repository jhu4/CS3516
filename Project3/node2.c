#include <stdio.h>
#include "project3.h"
#include "share.h"

extern int TraceLevel;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt2;
struct NeighborCosts   *neighbor2;


static int mynum = 2;

/* students to write the following two routines, and maybe some others */
void rtinit2() {
	int i,j;
	for(i=0;i<MAX_NODES;i++){
		for(j=0;j<MAX_NODES;j++){
			dt2.costs[i][j]=INFINITY;
		}
	}
	neighbor2=getNeighborCosts(2);
	//initialize itself's costs to neighbors
	for(i=0;i<MAX_NODES;i++){
		dt2.costs[mynum][i]=neighbor2->NodeCosts[i];
	}
	for(int i=0;i<MAX_NODES;i++){
		//cannot be myself or node that is not my neighbor
		if(i!=mynum && neighbor2->NodeCosts[i]<INFINITY){
			toLayer2(*makepacket(mynum,i,&dt2.costs[mynum][0]));	
		}
	}
}


void rtupdate2( struct RoutePacket *rcvdpkt ) {
	//just in case that this is actually a msg for me
	if(rcvdpkt->destid==mynum){
		//update the list for the sender node
		int i;
		int isChanged=NO;
		for(i=0;i<MAX_NODES;i++){
			dt2.costs[rcvdpkt->sourceid][i]=rcvdpkt->mincost[i];
		}
		//update itself's min distance to a node
		for(i=0;i<4;i++){
			int minToNodeI = smallest(dt2.costs[mynum][0]+dt2.costs[0][i]
							,dt2.costs[mynum][1]+dt2.costs[1][i]
							,dt2.costs[mynum][2]+dt2.costs[2][i]
							,dt2.costs[mynum][3]+dt2.costs[3][i]);
			//if the min value changed, flip the flag
			if(dt2.costs[mynum][i]!=minToNodeI){
				isChanged=YES;
				dt2.costs[mynum][i] = (minToNodeI<INFINITY?minToNodeI:INFINITY);
			}
		}
		if(isChanged){
			//broadcast the changing news to its neighbor
			for(int i=0;i<MAX_NODES;i++){
				//cannot be myself or node that is not my neighbor
				if(i!=mynum && neighbor2->NodeCosts[i]<INFINITY){
					toLayer2(*makepacket(mynum,i,&dt2.costs[mynum][0]));	
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
void printdt2( int MyNodeNumber, struct NeighborCosts *neighbor, 
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
}    // End of printdt2

