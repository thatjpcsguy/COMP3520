#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "main.h"



PcbPtr first_elem = NULL;
PcbPtr last_elem = NULL;
int dispatch_timer;

int main()
{
    // 1. Initialize dispatcher queue;
    // 2. Fill dispatcher queue from dispatch list file;
    char buf[MAX_BUFFER];

    // FILE *file = fopen (argv[1], "r");
    FILE *file = fopen ("fcfs.txt", "r");

    // set up variables for arguments
    char *args[MAX_ARGS];
    char **arg;

    // while there is lines to be processed from the input file
    while (!feof(file))
    {
        // read a line
        if (fgets (buf, MAX_BUFFER, file))
        {
            // tokenize input
            arg = args;
            *arg++ = strtok(buf, SEPARATORS);
            while ((*arg++ = strtok(NULL, SEPARATORS)));

            //create the new pcb in memory
            PcbPtr new_pcb = createnullPcb();

            // exit(1);
            //set pcb variables from file arguments
            new_pcb->arrivaltime = atoi(args[0]);
            new_pcb->remainingcputime = atoi(args[2]);
            new_pcb->args[0] = "process";
            new_pcb->args[1] = NULL;
            new_pcb->next = NULL;
            new_pcb->pid = 0;

            // printf("Created: %d\n", new_pcb->arrivaltime);
            enqPcb(new_pcb);
        }
    }

    PcbPtr current_process = NULL;
    
    print_pcbs();
    while (1)
    {
        if (current_process == NULL && first_elem == NULL)
        {
            exit(0);
        }

        if (current_process != NULL)
        {
            current_process->remainingcputime --;
            if (current_process->remainingcputime <= 0)
            {
                terminatePcb(current_process);
                // printf("terminated %d [%d]\n", current_process->arrivaltime, current_process->pid);
                current_process = NULL;
                free(current_process);
            }
        }
        if (current_process == NULL && first_elem != NULL)
        {
            if (first_elem->arrivaltime <= dispatch_timer)
            {
                current_process = deqPcb();
                startPcb(current_process);
                // printf("started %d [%d]\n", current_process->arrivaltime, current_process->pid);
            }
        }
        sleep(1);
        print_pcbs();
        if (current_process != NULL)
        {
            printf("%8d  | %8d  | %8d\n", current_process->arrivaltime, current_process->remainingcputime, current_process->pid);
        }
        dispatch_timer++;

    }
    return 0;
}


void print_pcbs()
{
    printf("\n\n\n\n\n\n\n");
    printf("%8s  | %8s  | %8s\n", "arrival", "cpu", "pid");
    PcbPtr x = first_elem;
    while (x != NULL)
    {
        printf("%8d  | %8d  | %8d\n", x->arrivaltime, x->remainingcputime, x->pid);
        x = x->next;
    }
}


PcbPtr startPcb(PcbPtr process)
{
    pid_t pid = fork();
    if (pid > 0) //parent process
    {
        process->pid = pid;
        return process;
    }
    else // child process
    {
        execvp(process->args[0], process->args);
        exit(1);
    }
}

PcbPtr terminatePcb(PcbPtr process)
{
    if (kill(process->pid, SIGINT))
    {
        return NULL;
    }
    return process;

}

PcbPtr createnullPcb(void)
{
    return (PcbPtr) malloc(sizeof(Pcb));
}

PcbPtr enqPcb (PcbPtr process)
{
    if (process == NULL)
        return NULL;

    process->next = NULL;
    if (first_elem == NULL)
    {
        first_elem = process;
        last_elem = process;
    }
    else
    {
        last_elem->next = process;
        last_elem = process;
    }
    return first_elem;
}

PcbPtr deqPcb ()
{
    PcbPtr e = first_elem;
    if (e != NULL)
    {
        first_elem = e->next;
    }
    return e;

}
