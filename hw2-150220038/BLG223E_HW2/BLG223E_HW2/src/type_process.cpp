#include "type_process.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void initialize_process(PROCESS *p, int pid, int priority){
    p->pid=pid;
    p->priority=priority;
}
