#include "insertion_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>




void initialize_execution_queue(INSERTION_QUEUE *iq){
    iq->front=0;
    iq->rear=-1;
    iq->size=0;

}

bool isFull(INSERTION_QUEUE *iq){
    return iq->size==MAX_OPERATION;
}

bool isEmpty(INSERTION_QUEUE *iq){
    return iq->size==0;
}

PROCESS_QUEUE peek(INSERTION_QUEUE *iq){
    
        return iq->queue[iq->front];
    
}

void enqueue(INSERTION_QUEUE *iq, PROCESS_QUEUE data){ //add back
    if(!isFull(iq)){
        iq->rear++;
        iq->queue[iq->rear]=data;
        iq->size++;

    }
}

PROCESS_QUEUE dequeue(INSERTION_QUEUE *iq){ //remove front and shift all 
    
        PROCESS_QUEUE wanted_front= iq->queue[iq->front];
        //shift all elements
        for(int i=0; i< iq->rear; i++){
            iq->queue[i]= iq->queue[i+1];
        }
        iq->size--;
        iq->rear--;
        return wanted_front ;
    
}

