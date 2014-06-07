#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>

#include "main.h"

int main()
{
    // 1. Initialize dispatcher queue;
    QueuePtr dispatch = createQueue();
    QueuePtr rr0 = createQueue();
    QueuePtr rr1 = createQueue();
    QueuePtr rr2 = createQueue();


    int dispatch_timer = 0;
    
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
            enqPcb(dispatch, new_pcb);
        }
    }

    PcbPtr current_process = NULL;

    while (1)
    {   
        if (current_process != NULL)
        {
            printf("%d [%d]: %d\n", current_process->arrivaltime, current_process->pid, current_process->remainingcputime);
        }

        if (current_process == NULL && dispatch->first == NULL && rr0->first == NULL)
        {
            exit(0);
        }

        if (dispatch->first != NULL && dispatch->first->arrivaltime <= dispatch_timer)
        {

            enqPcb(rr0, deqPcb(dispatch));
        }

        if (current_process != NULL)
        {
            current_process->remainingcputime --;
            if (current_process->remainingcputime <= 0)
            {
                terminatePcb(current_process);
                current_process = NULL;
                free(current_process);
            }
            else {
                kill(current_process->pid, SIGTSTP);
                enqPcb(rr0, current_process);
                current_process = NULL;

            }
        }
        if (current_process == NULL && rr0->first != NULL)
        {
            current_process = deqPcb(rr0);

            if (current_process->pid != 0)
            {
                kill(current_process->pid, SIGCONT);
            } 
            else
            {
                startPcb(current_process);
            }
        }
        sleep(1);
        dispatch_timer++;

    }
    return 0;
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

PcbPtr enqPcb (QueuePtr queue, PcbPtr process)
{
    if (process == NULL)
        return NULL;

    process->next = NULL;
    if (queue->first == NULL)
    {
        queue->first = process;
        queue->last = process;
    }
    else
    {
        queue->last->next = process;
        queue->last = process;
    }
    return queue->first;
}

PcbPtr deqPcb (QueuePtr queue)
{
    PcbPtr e = queue->first;
    if (e != NULL)
    {
        queue->first = e->next;
    }
    return e;

}

QueuePtr createQueue(void)
{
    return (QueuePtr) malloc(sizeof(Queue));
}
