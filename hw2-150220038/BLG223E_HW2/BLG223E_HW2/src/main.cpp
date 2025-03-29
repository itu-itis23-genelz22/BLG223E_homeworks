#include "process_manager.h"
#include "insertion_queue.h"
#include "failure_stack.h"
#include "execution_functions.h"
#include "process_queue.h"
#include "type_process.h"
#include <iostream>

int main()
{
    PROCESS_MANAGER pm;      
    initialize_process_manager(&pm);

    INSERTION_QUEUE eq;   
    initialize_execution_queue(&eq);

    FAILURE_STACK fs;
    initialize_failed_stack(&fs);


    const char *filename1 = "initial_processes.txt";
    const char *filename2 = "arriving_processes.txt";
   

    read_process_file(filename1, &pm);
    read_insertion_file(filename2, &eq);


    execution_loop(&pm, &eq, &fs);

    
    return 0;
}