#include "execution_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>



void read_process_file(const char *filename, PROCESS_MANAGER *pm){
    FILE *file = fopen(filename, "r");
    if(!file){
        printf("Error opening file: %s\n", filename);
        return;
    }
    //read line by line

    char lines[256]; //buffer
    
    PROCESS_QUEUE pq;
    initialize_process_queue(&pq);

    //dont get first line (headers)
    fgets(lines, sizeof(lines), file);

    while(fgets(lines, sizeof(lines), file)){
        int pid;
        int priority;
        int isHead;

        sscanf(lines, "%d, %d, %d", &pid, &priority, &isHead);

        PROCESS p;
        initialize_process(&p, pid, priority);
        //group related processes --> process queue
        enqueue(&pq, p); // add process to process queue(not the insertion queue??)
        
        //process queues --> process manager deque (priority!!)
        if(isHead == 1){
            if(priority == 1){
                insert_front(pm,pq);                
            }else if(priority == 0){
                insert_rear(pm, pq);
            }
        }

    }

    fclose(file);    
        
}

void read_insertion_file(const char *filename, INSERTION_QUEUE *eq){
    //read each line (parse)
    FILE *file = fopen(filename, "r");
    if(!file){
        printf("Error opening file: %s\n", filename);
        return;
    }
    
    char lines[256]; //buffer
    PROCESS_QUEUE pq;
    initialize_process_queue(&pq);

    int current_iteration = -1;

    //dont get first line (headers)
    fgets(lines, sizeof(lines), file);

    while(fgets(lines, sizeof(lines), file)){
        int iteration;
        int pid;
        int priority;
        int isHead;

        sscanf(lines, "%d, %d, %d, %d",&iteration, &pid, &priority, &isHead);

        PROCESS p;
        initialize_process(&p, pid, priority);
        //group related processes --> process queue
        if (iteration != current_iteration && pq.size >= 0) {
            pq.iteration = iteration;// Update the iteration
            pq.priority = priority;
            enqueue(eq, pq);               // add process to the insertion queue??)
            initialize_process_queue(&pq); // Reset the queue
        }

        enqueue(&pq, p);  //  (same iteration)

        // Update the current iteration after processing the new process
        current_iteration = iteration;
           

    }

    if (pq.size > 0) {
         pq.iteration = current_iteration;
        enqueue(eq, pq);
    }
    fclose(file);   
    // group related(by iteration num) processes -> process queue

    //process queues --> insertion queue
    //for later insertion queue-->(at iteration)process manager deque(!priority front or back )
}

void execution_loop(PROCESS_MANAGER *pm, INSERTION_QUEUE *eq, FAILURE_STACK *fs){
    //iterate 
    int iteration = 0;
    FILE *executionFile ;
    executionFile = fopen("execution_run.txt", "w");
    if(!executionFile){
        printf("Error opening file.\n");
        return;
    }

    //for each iteraton 
    while(!isEmpty(pm) || !isEmpty(eq)){
        //1-execute a process
        //  deque high priorty
        //  check if pid%8==0 ->push failure stack
        //2-handle arriving 
        //  insertion queue iteration check
        // add to process manager deque (by priority)
        

        if (!isEmpty(pm)) {
            PROCESS_QUEUE current_queue = delete_front(pm); // Get the highest-priority queue from front

            while (!isEmpty(&current_queue)) {
                PROCESS current_process = dequeue(&current_queue);
                
                // Check for failure
                if (current_process.pid % 8 == 0) {
                    fprintf(executionFile, "%d, f\n", current_process.pid); // write f
                    push(fs, current_queue); // Push to failure stack
                    break; 
                } else {
                    fprintf(executionFile, "%d, s\n", current_process.pid); // write s
                }
                if(peek(eq).iteration == iteration){
                    PROCESS_QUEUE arriving_queue = dequeue(eq); // get from insertion queue
                    if (arriving_queue.priority == 1) {
                        insert_front(pm, arriving_queue); // High priority add front
                    } else {
                    insert_rear(pm, arriving_queue);  // Low priority add back
                    }
                }


                iteration++;
            }
        }
        
       
    }
    
    
    fclose(executionFile);
}

