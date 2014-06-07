// max line buffer
#define MAX_BUFFER 1024
 // max # args                        
#define MAX_ARGS 64
// token sparators                       
#define SEPARATORS " \t\n,"  

struct pcb {
    pid_t pid;             // system process ID
    char * args[MAX_ARGS];  // program name and args
    int arrivaltime;
    int remainingcputime;
    struct pcb * next;     // links for Pcb handlers
};
typedef struct pcb Pcb;
typedef Pcb * PcbPtr;

struct queue {
    char * name[MAX_BUFFER];
   	PcbPtr first;
	PcbPtr last;
	int length;
};
typedef struct queue Queue;
typedef Queue * QueuePtr;

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


QueuePtr createQueue(void);
