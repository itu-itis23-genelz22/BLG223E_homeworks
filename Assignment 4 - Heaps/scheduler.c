#include <stdlib.h>
#include "../include/scheduler.h"

// Compare function must return negative if first argument is smaller
int compare(const void* a, const void* b) {
    const Process* process_a = (const Process*)a;
    const Process* process_b = (const Process*)b;
    if (process_a->vruntime < process_b->vruntime) return -1;
    if (process_a->vruntime > process_b->vruntime) return 1;
    return 0; // vruntime is equal
}
/*
 * Creates and initializes a new scheduler instance.
 * The capacity parameter determines the maximum number of processes that can be managed.
 * Returns a pointer to the new scheduler, or NULL if allocation fails.
 */
Scheduler* create_scheduler(int capacity){
    Scheduler* new_scheduler = (Scheduler*)malloc(sizeof(Scheduler));
    if(!new_scheduler) return NULL;

    new_scheduler->process_queue= heap_create(capacity, sizeof(Process), compare);
    if(!new_scheduler->process_queue){ 
        free(new_scheduler);
        return NULL; 
    }

    new_scheduler->current_process=NULL;
    new_scheduler->time_slice=10; 
    return new_scheduler;
}

/*
 * Deallocates all resources associated with the scheduler.
 * This includes freeing the process queue, current process, and scheduler structure itself.
 * Should be called when the scheduler is no longer needed to prevent memory leaks.
 */
void destroy_scheduler(Scheduler* scheduler){
    if(!scheduler) return;
    if(scheduler->process_queue){
        heap_destroy(scheduler->process_queue);
    }  
    if(scheduler->current_process){
        free(scheduler->current_process);
    }              
    free(scheduler);       
}

/*
 * Adds a new process to the scheduler's queue.
 * The process is inserted into the min-heap based on its virtual runtime,
 * maintaining the scheduling order where processes with lower virtual runtime
 * have higher priority for execution.
 */
void schedule_process(Scheduler* scheduler, Process process){
    if(!scheduler) return;
    heap_insert(scheduler->process_queue, &process);
    
}

/*
 * Retrieves the next process to be executed based on virtual runtime.
 * Returns the process with the lowest virtual runtime from the queue.
 * If a process is currently running, it is placed back in the queue.
 * Returns NULL if no processes are available for execution.
 */
Process* get_next_process(Scheduler* scheduler){
    if( !scheduler ||!scheduler->process_queue||scheduler->process_queue->size==0){ //Returns NULL if no processes are available for execution.
        return NULL;
    }
    if(scheduler->current_process && scheduler->current_process->is_running){ //If a process is currently running, it is placed back in the queue.
        scheduler->current_process->is_running = false;
        if (!heap_insert(scheduler->process_queue, scheduler->current_process)) {
            free(scheduler->current_process); 
            scheduler->current_process = NULL;
            return NULL;
        }
        free(scheduler->current_process); 
        scheduler->current_process = NULL;
    }

    //get next process
    scheduler->current_process = (Process*)malloc(sizeof(Process));
    if (!scheduler->current_process) return NULL;

    if (!heap_extract_min(scheduler->process_queue, scheduler->current_process)) {
        free(scheduler->current_process);
        scheduler->current_process = NULL;
    } else {
        scheduler->current_process->is_running = true;
    }


    return scheduler->current_process;
}

/*
 * Updates the scheduler state for one time slice unit.
 * Increments the virtual runtime of the currently executing process
 */
void tick(Scheduler* scheduler){    
    if (scheduler && scheduler->current_process) {        
        update_vruntime(scheduler->current_process, scheduler->time_slice);
    }
}

