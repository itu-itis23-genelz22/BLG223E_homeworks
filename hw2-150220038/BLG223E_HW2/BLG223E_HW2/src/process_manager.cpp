#include "process_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



void initialize_process_manager(PROCESS_MANAGER *pm){
    pm->front=0;
    pm->rear=-1;
    pm->size=0;
}

bool isFull(PROCESS_MANAGER *pm){
    return pm->size==MAX_PROCESS;
}

bool isEmpty(PROCESS_MANAGER *pm){
    return pm->size==0;
}

//High-priority process queues are added at the front.
void insert_front(PROCESS_MANAGER *pm, PROCESS_QUEUE pq){ 
    if(!isFull(pm)){
        //shift all then add front 
        for (int i = pm->size - 1; i >= 0; i--) {
        pm->deque[i + 1] = pm->deque[i];
        }
        //add new queue
        pm->deque[pm->front]=pq;

        pm->rear++;
        pm->size++;
    }
}

//Low-priority process queues are added at the rear.
void insert_rear(PROCESS_MANAGER *pm, PROCESS_QUEUE pq){ //NO need to shift 
    if(!isFull(pm)){
        pm->rear++;
        pm->deque[pm->rear]= pq;
        pm->size++; 
    }    
}

PROCESS_QUEUE delete_front(PROCESS_MANAGER *pm){ //SHIFT ALL 
     
        PROCESS_QUEUE wanted_front = pm->deque[pm->front];
        //shift all 
        for(int i=0; i< pm->rear; i++){
            pm->deque[i]= pm->deque[i+1];
        }
        pm->rear--;
        pm->size--;
        return wanted_front;
    
}


PROCESS_QUEUE delete_rear(PROCESS_MANAGER *pm){ //no need to shift
    
        PROCESS_QUEUE wanted_rear = pm->deque[pm->rear];
        pm->rear--;
        pm->size--;
        return wanted_rear;
    
}

