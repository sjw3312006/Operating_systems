//Jeongwoon Suh : 301313489 : CMPT 300 : Assignment 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "list.h"

#define msg_size 43

//ready queue
List* readyQ_High;
List* readyQ_Norm;
List* readyQ_Low;

List* jobQ; //holds all of the processes
List* running_process_Q; //holds currently running processes
List* blockedQ;

Semaphore* semQ[5];

int procCount = 0;
int* currentPid = NULL;

PCB* Copy_process(PCB* process){

    PCB* copy_process = malloc(sizeof* copy_process);
    strcpy(copy_process -> msg, process -> msg);
    copy_process -> id = process -> id;
    copy_process -> priority = process -> priority;
    copy_process -> p_state = process -> p_state;

    return copy_process;
}

int isEqual(void* x, void* y){
    return *((int*)x) == *((int*)y);
}

void Item_free(void *p){
    p = NULL;
}

void Enqueue_process(PCB* process){

    if(process -> priority == 0){
        process -> p_state = READY;
        List_append(readyQ_High, process);
    }
    else if(process -> priority == 1){
        process -> p_state = READY;
        List_append(readyQ_Norm, process);
    }
    else if(process -> priority == 2){
        process -> p_state = READY;
        List_append(readyQ_Low, process);
    }   
}

void Create(int priority){

    if(priority < 0 || priority > 2){
        printf("Invalid priority value");
        return;
    }

    PCB* process = malloc(sizeof(PCB));
    process -> id = procCount;
    process -> priority = priority;
    process -> msg[0] = '\0';

    process -> p_state = RUNNING;
     
    //if there is currently running process
    if(List_count(running_process_Q) > 1){
        PCB * current_running = List_last(running_process_Q);
        printf("currently running pid : %d\n", current_running -> id);
        Enqueue_process(process);
    }
    //init process running
    else{
        PCB* init_process_Q = List_first(running_process_Q);
        process -> p_state = RUNNING;
        init_process_Q -> p_state = READY;
        List_append(running_process_Q, process);
        printf("currently running pid: %d\n", process -> id);
    }

    List_append(jobQ, process);
}

void Fork(void){

    //if there is currently running process
    if(List_count(running_process_Q) > 0){
        //get the init process
        PCB * running_process = List_last(running_process_Q);

        //check if it's init process
        if(running_process -> id == 0 && running_process -> priority == 3){
            printf("Attempting to Fork the 'init' process should fail \n");
        }
        PCB* process = Copy_process(running_process);
        process -> id = procCount++;

        Enqueue_process(process);
        List_append(jobQ, process);
        printf("Success to Fork the process[%d]\n", process -> id);
    }
    else{
        printf("Failure to Fork any of the process\n");
    }
}

void Kill(int pid){

    int count = 0;
    bool killed = false;

    //init process
    if(pid == 0){
        if(List_count(jobQ) > 1){
            printf("Attempting to Kill the 'init' process should fail \n");
            return;
        }
    }

    PCB* kill_process;
    if(List_count(running_process_Q) > 0){
        kill_process = List_first(running_process_Q);
        while(kill_process != NULL){
            if(kill_process -> id = pid){
                List_remove(running_process_Q);
                count++;
                killed = true;
                break;
            }
            else{
                kill_process = List_next(running_process_Q);
            }
        }
    }
    if(List_count(jobQ) > 0){
        kill_process = List_first(jobQ);
        while(kill_process != NULL){
            if(kill_process -> id = pid){
                List_remove(jobQ);
                count++;
                break;
            }
            else{
                kill_process = List_next(jobQ);
            }
        }
    }

    if(List_count(blockedQ) > 0 && count == 1){
        kill_process = List_first(blockedQ);
        while(kill_process != NULL){
            if(kill_process -> id == pid){
                List_remove(blockedQ);
                count++;
                break;
            } 
            else{
                kill_process = List_next(blockedQ);
            }
        }
    }

    //check the ready queue
    if(pid != 0 && count == 1){
        if(List_count(readyQ_Low) > 0){ //low queue
            kill_process = List_first(readyQ_Low);
            while(kill_process != NULL){
                if(kill_process -> id = pid){
                    List_remove(readyQ_Low);
                    count++;
                    break;
                }
                else{
                    kill_process = List_next(readyQ_Low);
                }
            }
        }
        if(List_count(readyQ_Norm) > 0){ //norm queue
            kill_process = List_first(readyQ_Norm);
            while(kill_process != NULL){
                if(kill_process -> id = pid){
                    List_remove(readyQ_Norm);
                    count++;
                    break;
                }
                else{
                    kill_process = List_next(readyQ_Norm);
                }
            }
        }
        if(List_count(readyQ_High) > 0){ //high queue
            kill_process = List_first(readyQ_High);
            while(kill_process != NULL){
                if(kill_process -> id = pid){
                    List_remove(readyQ_High);
                    count++;
                    break;
                }
                else{
                    kill_process = List_next(readyQ_High);
                }
            }
        }
    }
    if(count == 0){
        printf("Failure to Kill any of the process\n");
    }
    else{
        if(pid == 0){ //kill the init process 
            printf("Success to Kill the init process\n");
        }
        else{
            printf("Success to Kill the process[%d]\n", pid);
        }
    }

    free(kill_process);
}

void Exit(void){

    PCB* exit_process;

    if(List_count(running_process_Q) > 0){
        //currently running process
        exit_process = List_last(running_process_Q);

        //init process
        if(exit_process -> id == 0 && exit_process -> priority == 3){
            int currentpid = exit_process -> id;

            List_remove(running_process_Q);
            exit_process = List_last(jobQ);

            if(exit_process -> id == 0){
                List_remove(jobQ);

                printf("Success to Kill the init process\n");
            }
        }

        else{
            int currentpid = exit_process -> id;

            List_remove(running_process_Q);
            printf("Success to Kill the process[%d]\n", exit_process -> id);

            //remove the process in jobQ
            while(exit_process != NULL){
                if(exit_process -> id == currentpid){
                    free(exit_process);
                    List_remove(jobQ);
                    break;
                }
                else{
                    exit_process = List_next(jobQ);
                }
            }
        }
    }
}

void Quantum(){

    PCB* quantum_process;

    if(List_count(running_process_Q) > 0){
        quantum_process = List_last(running_process_Q);

       if(quantum_process -> id == 0 && quantum_process -> priority == 3){
            printf("Fail to expire the init process\n");
        }
        else{
            quantum_process -> p_state = READY;
            quantum_process -> priority = 0;

            List_append(readyQ_Low, quantum_process);
            List_remove(running_process_Q);

            printf("Success to expire the time quantum of the process[%d]\n", quantum_process -> id);
        }
    }
}

int Send(int pid, char* msg){

    PCB* send_process;
    PCB* recv_process;

    if(pid == 0){
        printf("Attempting to Send the 'init' process should fail \n");
        return 0;
    }

    if(List_count(running_process_Q) > 0 && List_count(jobQ) > 0){
        send_process = List_last(running_process_Q);
    }

    recv_process = List_first(jobQ);

    while(recv_process != NULL){
        if(recv_process -> id = pid){
            //if send_process is init
            if(send_process -> id == 0){
                strcpy(recv_process -> msg, msg);
                printf("Success to send message from init process to process[%d]\n", recv_process -> id);
            }


        }
    }
}

void Receive(){

    PCB* recv_process;

    if(List_count(running_process_Q) > 0){
        recv_process = List_last(running_process_Q);

        if(recv_process -> id == 0){
            printf("currently running process is init process\n");
            printf("cannot recieve a message\n");
            return;
        }

        //message displayed
    }
    
}

int Reply(int pid, char* msg){

    PCB* send_process;
    PCB* recv_process;


    if(List_count(running_process_Q) > 0){
        recv_process = List_last(running_process_Q);

        if(recv_process -> id == 0){
            printf("currently running process is init process\n");
            printf("cannot reply a message\n");
            return 0;
        }


    }
}

int New_Semaphore(int semID, int initVal){

    if(semID < 0 || semID > 4){
        printf("Semaphore ID should be 0 - 4.\n");
        return 0;
    }
    if((semQ[semID] -> id) == semID){
        printf("Fail to use this id of semaphore. It's already used.\n");
        return 0;
    }
    //creating a semaphore
    else if((semQ[semID] -> id) == -1){
        Semaphore sem;
        sem.id = semID;
        sem.val = initVal;
        sem.process = List_create();
    }

    printf("Success to create new semaphore with id [%d]\n", semID);
}

int Semaphore_P(int semID){

    if(semID < 0 || semID > 4){
    printf("Semaphore ID should be 0 - 4.\n");
    return 0;
    }

    if(semQ[semID]->id == -1){
        printf("Semaphore ID does not exist\n");
        return 0;
    }

    PCB* semP_process;

    if(List_count(running_process_Q) > 0){
        semP_process = List_last(running_process_Q);

        if(semP_process -> priority == 3 && semP_process -> id == 0){
            printf("init process is currently running\n");
        }
        else{
            printf("currently running process is [%d]\n", semP_process -> id);
        }

        //block the process
        if(semQ[semID]->val <= 0 && semP_process -> id != 0 && semP_process -> priority != 3){
            semP_process -> p_state = BLOCKED;
            semP_process -> blocked = semID;
            List_append(blockedQ, semP_process);
            List_remove(running_process_Q);

            semQ[semID]->val--;
            printf("semaphoreP successfully decreased the sem value, and the process [%d] is blocked\n", semP_process -> id);
        }
        else{
            semQ[semID]->val--;
            printf("semaphoreP successfully decreased the sem value, and no process was blocked \n");
        }
    }
}

int Semaphore_V(int semID){

    if(semID < 0 || semID > 4){
    printf("Semaphore ID should be 0 - 4.\n");
    return 0;
    }

    if(semQ[semID]->id == -1){
        printf("Semaphore ID does not exist\n");
        return 0;
    }

    PCB* semV_process;
    PCB* curr_process;

    curr_process = List_last(running_process_Q);

    if(List_count(running_process_Q) < 1){
        printf("init process is currently running");
    }
    else{
       printf("currently running process is [%d]\n", curr_process -> id);
    }

    //block the process
    if(List_count(blockedQ) > 0 && semQ[semID]->val < 0){
        semV_process = List_last(blockedQ);

        while(semV_process != NULL){
            if(semV_process -> blocked == semID){
                //unblock the process
                semV_process -> p_state = READY;
                semV_process -> blocked = -1;

                Enqueue_process(semV_process);
                List_remove(blockedQ);
                semQ[semID]->val++;

                if(curr_process -> id == 0 && curr_process -> priority == 3){ //init process
                    printf("process id %d with priority %d was unblocked on behalf of init process\n", semV_process -> id, semV_process -> priority);
                }
                else{
                    printf("process id %d with priority %d was unblocked on behalf of currently running process %d \n", semV_process -> id, semV_process -> priority, curr_process -> id);
                }
                return 0;
            }
            else{
               semV_process = List_prev(blockedQ); 
            }
        }
    }

    else if(List_count(blockedQ) > 0){
        semQ[semID] -> val++;
        printf("process is not blocked with corresponding sem ID %d \n", semQ[semID] -> id);
    }

    else{
        semQ[semID] -> val++;
        printf("sem ID of %d is now %d\n", semQ[semID] -> id, semQ[semID] -> val);
    }
}

void Procinfo(int pid){

    PCB* process;

    if(List_count(jobQ) > 0){
        process = List_first(jobQ);

        while(process != NULL){
            if(process -> id == 0){
                printf("init process\n");
            }
            if(process -> id == pid){
                printf("PID : %d \n", process -> id);
                
                if(process -> p_state == BLOCKED){
                    printf("Process state: BLOCKED by semaphore ID of %d\n", process -> blocked);
                }

                else if(process -> p_state == READY){
                    printf("Process state: READY\n");
                }

                else{
                    printf("Process state: RUNNING\n");
                }
                return;
            }
            else{
                process = List_next(jobQ);
            }
        }
        printf("no process created\n");
    }
}

void Totalinfo(){

    PCB* process; 

    if(List_count(running_process_Q) > 0){
        process = List_last(running_process_Q);
        if(process -> id == 0){
            printf("init process is currently running\n");
        }
        else{
            printf("currently running process is %d\n", process -> id);
        }
    }

    if(List_count(readyQ_Low) > 0){
        process = List_first(readyQ_Low);
        printf("low priority queue processes: \n");

        while(process != NULL){
            printf("process id : %d\n", process -> id);
            process = List_next(readyQ_Low);
        }
        printf("\n");
    }

    else{
        printf("There is no process in low priority queue\n");
    }

    if(List_count(readyQ_Norm) > 0){
        process = List_first(readyQ_Norm);
        printf("norm priority queue processes: \n");

        while(process != NULL){
            printf("process id : %d\n", process -> id);
            process = List_next(readyQ_Norm);
        }
        printf("\n");
    }

    else{
        printf("There is no process in norm priority queue\n");
    }

    if(List_count(readyQ_High) > 0){
        process = List_first(readyQ_High);
        printf("high priority queue processes: \n");

        while(process != NULL){
            printf("process id : %d\n", process -> id);
            process = List_next(readyQ_High);
        }
        printf("\n");
    }

    else{
        printf("There is no process in high priority queue\n");
    }

    if(List_count(blockedQ) > 0){
        process = List_first(blockedQ);
        printf("blocked queue processes: \n");

        while(process != NULL){
            printf("process id : %d\n", process -> id);
            process = List_next(blockedQ);
        }
        printf("\n");
    }

    else{
        printf("There is no process in blocked queue\n");
    }
}

int process(){
    int priority;
    int pid;
    char msg;
    int semID;
    int semVal;

    display();

    readyQ_High = List_create();
    readyQ_Norm = List_create();
    readyQ_Low = List_create();
    jobQ = List_create();
    running_process_Q = List_create();
    blockedQ = List_create();

    for(int i = 0; i < 5; i++){
        semQ[i] -> id = -1;
        semQ[i] -> val = -1;
        semQ[i] -> process = NULL;
    }

    while(1){
        char InputChar = (char)getchar();

        if (InputChar == '\n'){
            continue;
        }
        switch(InputChar){

            case 'C':
                printf("Create - Enter the priority of (0 = high, 1 = norm, 2 = low) \n");
                scanf("%d", &priority);
                Create(priority);
                break;

            case 'F':
                printf("Fork - Forking the process\n");
                Fork();
                break;

            case 'K':
                printf("Kill - Enter the pid: \n");
                scanf("%u", &pid);
                Kill(pid);
                break;

            case 'E':
                printf("Exit - kill the currently running process\n");
                Exit();
                break;

            case 'Q':
                printf("Quantum - CPU time will be expired\n");
                Quantum();
                break;

            case 'S':
                printf("Send - Enter the pid of the process to send \n");
                msg = (char*)malloc(msg_size * sizeof(char));
                scanf("%u", &pid);
                getchar();
                fgets(msg, msg_size, stdin);
                Send(pid, msg);
                break;

            case 'R':
                printf("Receive - Current process will block \n");
                Receive();
                break;

            case 'Y':
                printf("Reply - Enter the pid to reply : \n");
                msg = (char*)malloc(msg_size * sizeof(char));
                scanf("%u", &pid);
                getchar();
                fgets(msg, msg_size, stdin);
                Reply(pid, msg);
                break;

            case 'N': 
                printf("(value can be 0 - 4) Enter semaphore ID: ");
                scanf("%d", &semID);
                printf("(value can be 0 or higher) Enter semaphore value: ");
                scanf("%d", &semVal);
                New_Semaphore(semID, semVal);
                break;

            case 'P': 
                printf("(value can be 0 - 4) Enter semaphore ID: ");
                scanf("%d", &semID);
                Semaphore_P(semID);
                break;

            case 'V': case 'v':
                printf("(value can be 0 - 4) Enter semaphore ID: ");
                scanf("%d", &semID);
                Semaphore_V(semID);
                break;

            case 'I': case 'i':
                printf("pid of process information is to be returned for: ");
                scanf("%u", &pid);
                Procinfo(pid);
                break;

            case 'T': case 't':
                Totalinfo();
                break;
            
            default:
                printf("Please follow the one of these commands\n");
                break;      
        }
    }
}

void display()
{
    printf("(C)reate: Create the new process with pid.\n");
    printf("(F)ork: Copy the currently running process.\n");
    printf("(K)ill: kill the named process.\n");
    printf("(E)xit: Kill the current running process.\n");
    printf("(Q)uantum: Time quantum of running process expires.\n");
    printf("(S)end: Send a message to another process - block until reply.\n");
    printf("(R)eceive: receive a message - block until one arrives.\n");
    printf("Repl(Y): unblocks sender and delivers reply.\n");
    printf("(N)ew Semaphore: Initialize the named semaphore with the value given.\n");
    printf("Semaphore(P): execute the semaphore P operation.\n");
    printf("Semaphore(V): execute the semaphore V operation.\n");
    printf("Proc(I)nfo: Print the process's info.\n");
    printf("(T)otalInfo: Display all process queues and their contents.\n");
}