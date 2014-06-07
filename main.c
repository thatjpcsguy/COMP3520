#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>

#include "main.h"

#define NUM_RR 3

int main()
{
    // 1. Initialize dispatcher queue;
    QueuePtr dispatch = createQueue();

    QueuePtr rr[NUM_RR];

    for (int i = 0; i < NUM_RR; i++)
        rr[i] = createQueue();


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
            new_pcb->priority = atoi(args[1]);



            printf("Created: %d %d\n", new_pcb->arrivaltime, new_pcb->priority);
            enqPcb(dispatch, new_pcb);
        }
    }

    PcbPtr current_process = NULL;

    while (1)
    {   
        if (current_process != NULL)
        {
            printf("%d [%d]: %d (%d)\n", current_process->arrivaltime, current_process->pid, current_process->remainingcputime, current_process->priority);
        }

        if (current_process == NULL && dispatch->first == NULL && rr[0]->first == NULL)
        {
            exit(0);
        }

        if (dispatch->first != NULL && dispatch->first->arrivaltime <= dispatch_timer)
        {
            PcbPtr foo = deqPcb(dispatch);
            enqPcb(rr[get_priority(foo->priority)], foo);
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
            else if (process_in_queues(rr) == 1) {
                if (current_process->priority > 1)
                    current_process->priority--;

                kill(current_process->pid, SIGTSTP);
                enqPcb(rr[get_priority(current_process->priority)], current_process);
                current_process = NULL;
            }
        }
        if (current_process == NULL && process_in_queues(rr))
        {
            current_process = deqPcb(rr[highest_priority_process(rr)]);

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


int get_priority(int x)
{
    return (NUM_RR - x);
}

int process_in_queues(QueuePtr * x)
{
    for (int i = 0; i < NUM_RR; i++)
    {
        if (x[i]->first != NULL)
        {
            return 1;
        }
    }
    return 0;
}

int highest_priority_process(QueuePtr * x)
{
    for (int i = 0; i < NUM_RR; i++)
    {
        if (x[i]->first != NULL)
        {
            return i;
        }
    }
    return -1;
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
        printf("Terminated: %d [%d]\n", process->arrivaltime, process->pid);
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

MabPtr memChk(MabPtr m, int size)
{

}

MabPtr memAlloc(MabPtr m, int size)
{

}

MabPtr memFree(MabPtr m)
{

}
 
MabPtr memMerge(MabPtr m)
{

}

MabPtr memSplit(MabPtr m, int size)
{

}
