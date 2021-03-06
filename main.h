// max line buffer
#define MAX_BUFFER 1024
// max # args
#define MAX_ARGS 64
// token sparators
#define SEPARATORS " \t\n,"

struct pcb
{
    pid_t pid;             // system process ID
    char *args[MAX_ARGS];   // program name and args
    int arrivaltime;
    int remainingcputime;
    int priority;
    int memoryrequired;

    struct pcb *next;      // links for Pcb handlers
};
typedef struct pcb Pcb;
typedef Pcb *PcbPtr;

struct queue
{
    char *name[MAX_BUFFER];
    PcbPtr first;
    PcbPtr last;
    int length;
};
typedef struct queue Queue;
typedef Queue *QueuePtr;


struct mab
{
    int offset;
    int size;
    int allocated;
    struct mab *next;
    struct mab *prev;
};
typedef struct mab Mab;
typedef Mab *MabPtr;

/*
- queue process (or join queues) at end of queue
- enqueues at "tail" of queue list.
returns:
  (new) head of queue
*/
PcbPtr enqPcb (QueuePtr queue, PcbPtr process);

/*
- dequeue process - take Pcb from "head" of queue.
returns:
  PcbPtr if dequeued,
  NULL if queue was empty
  & sets new head of Q pointer in adrs at 1st arg
*/
PcbPtr deqPcb (QueuePtr queue);

/*
- create inactive Pcb.
returns:
  PcbPtr of newly initialised Pcb
  NULL if malloc failed
*/
PcbPtr createnullPcb(void);

/*
- terminate a process
returns:
  PcbPtr of process
  NULL if terminate failed
  */
PcbPtr terminatePcb(PcbPtr process);

/*
 - start a process
 returns:
   PcbPtr of process
   NULL if start failed
*/
PcbPtr startPcb(PcbPtr process);

//create an empty queue object in memory
QueuePtr createQueue(void);

//get computed priority of a PCB
int get_priority(int x);

//find the queue which holds the process of highest priority
int highest_priority_process(QueuePtr *x);

//check if there are any processes in an array of queues
int process_in_queues(QueuePtr *x);


MabPtr memChk(MabPtr m, int size);   // check if memory available
MabPtr memAlloc(MabPtr m, int size); // allocate memory block
MabPtr memFree(MabPtr m);            // free memory block

MabPtr memMerge(MabPtr m);           // merge two memory blocks
MabPtr memSplit(MabPtr m, int size); // split a memory block
MabPtr createnullMab(void);

