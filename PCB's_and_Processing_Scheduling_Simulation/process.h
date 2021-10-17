//Jeongwoon Suh : 301313489 : CMPT 300 : Assignment 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

enum state {RUNNING, BLOCKED, READY};

typedef struct PCB{
    int id;//to identify the processes
    int priority;
    enum state p_state; //process state
    char msg[40]; //40 char max 
    char proc_message;
    int blocked;
} PCB;

typedef struct Semaphore{
    int id;
    int val;
    List* process;
} Semaphore;

PCB* Copy_process(PCB* process);

/*create a process and put it on the appropriate ready Q
reports: success or failure, the pid of the created process on success
*/
void Create(int priority);

/*copy the currently running process and put it on the ready Q corresponding to the 
original process' priority. Attempting to Fork the "init" process shoule fail
*/
void Fork(void);

/* kill the named process and remove it from the system
*/
void Kill(int pid);

/* kill the currently running process
*/
void Exit(void);

/* time quantum of running process expires
*/
void Quantum();

/* send a message to another process -block until reply
*/
int Send(int pid, char* msg);

/* receive a message - block until one arrives
*/
void Receive();

/* unblocks sender and delivers reply
*/
int Reply(int pid, char* msg);

/* initialize the named semaphore with the value given, ID's can take a value from 0 to 4. This 
can only be done once for a semaphore - subsequent attempts result in error. 
*/
int New_Semaphore(int semID, int initVal);

/* execute the semaphore P operation on behalf of the running process. You can assume semaphores ID
numbered 0 through 4.
*/
int Semaphore_P(int semID);

/* execute the semaphore V operation on behalf of the running process. You can assume semaphores ID
numbered 0 through 4.
*/
int Semaphore_V(int semID);

/* dump complete state information of process to screen (includes process status)
*/
void Procinfo(int pid);

/* display all process queues and their content
*/
void Totalinfo();

int process();

void display();