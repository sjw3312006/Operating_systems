//Jeongwoon Suh : 301313489 : CMPT 300 : Assignment 2

/*Description:
Setting up all four threads needed for the chat client. This program allows users to communicate concurrently.
*/

#include <pthread.h>
#include "list.h"

#define NUM_THREADS_IO 2 //input and output
#define NUM_THREADS_SR 2 //sender and reciever
#define ESCAPE_CHAR "!\n\0" //terminating factor : !

pthread_mutex_t msg_mutex;
pthread_cond_t msg_recieved;
pthread_cond_t msg_ready_to_output;

pthread_mutex_t sent_mutex;
pthread_cond_t msg_ready_to_send;

struct screen_thread{
    List* list_m; //shared list which all four threads will share
};

//required data for sender and reciever to communicate
struct send_recv_data_thread{ 
    List* shared_data;
    struct sockaddr_in* client_addr;
    int socket;
    char* msg;
    int msg_len;
    int size;
};

pthread_t* get_threads(); //return id of threads

int get_keyboard_id(); //return id of the keyboard thread

int get_send_id(); //return id of the sender thread

int get_screen_id(); //return id of the sreen thread

int get_recv_id(); //return id of the receieved thread 

void* keyboard_input_thread(void* data); //adds the input to the list of messages that need to be sent to the remote s-talk client

void* send_thread(void* data); //will take each msg off this list and send it over the network to the remote client 

void* screen_output_thread(void* data); //wil take each msg off this list and output it to the screen

void* recv_thread(void* data); //will put the msg onto the list of msg that need to be prined to the local screen