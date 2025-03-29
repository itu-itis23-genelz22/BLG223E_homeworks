#include "process_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void initialize_process_queue(PROCESS_QUEUE *pq){ //initially empty
    pq->front=0;
    pq->rear=-1;
    pq->size=0;
    pq->priority=0;
    pq->iteration=0;
}

bool isFull(PROCESS_QUEUE *pq){
    return pq->size == QUEUE_SIZE;
    
}

bool isEmpty(PROCESS_QUEUE *pq){
    return pq->size == 0;
}

PROCESS peek(PROCESS_QUEUE *pq){ //dont remove just return it
   
        return pq->queue[pq->front]; 
    

}

void enqueue(PROCESS_QUEUE *pq, PROCESS data){ //add to queue from back
    if(!isFull(pq)){ //if not full
        pq->rear = pq->rear + 1;
        pq->queue[pq->rear] = data;
        pq->size++;
    }

}

PROCESS dequeue(PROCESS_QUEUE *pq){ //removes from the front
   
        PROCESS wanted = pq->queue[pq->front];
        //shift all
        for(int i=0; i< pq->rear; i++){
            pq->queue[i]= pq->queue[i+1];
        }

        pq->rear--;
        pq->size--;
        return wanted;
    
}