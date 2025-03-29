#include "failure_stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



void initialize_failed_stack(FAILURE_STACK *fs){
    fs->top=-1;
}

bool isFull(FAILURE_STACK *fs){
    return fs->top==MAX_FAILED-1;
}

bool isEmpty(FAILURE_STACK *fs){
    return fs->top==-1;
}

//if pid%8 push to this stack
void push(FAILURE_STACK *fs, PROCESS_QUEUE data){ //add top
    if(!isFull(fs)){
        fs->top++;
        fs->stack[fs->top] = data;
    }
}

PROCESS_QUEUE pop(FAILURE_STACK *fs){ //remove top and return it
   
        PROCESS_QUEUE wanted_top= fs->stack[fs->top];

        fs->top--;
        return wanted_top;
    

}

