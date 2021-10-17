//Jeongwoon Suh : 301313489 : CMPT 300 : Assignment 2

/*Description: 
Setting up all four threads needed for the chat client. This program allows users to communicate concurrently. 
*/

#include "thread.h"
#include "socket.h"
#include "list.h"
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

enum thread_index_id{
    keyboard = 0,
    sender = 1,
    screen = 2,
    reciever = 3
};

pthread_t thread[NUM_THREADS_IO + NUM_THREADS_SR];

pthread_t* get_threads(){
    return thread;
}

int get_keyboard_id(){
    return keyboard;
}

int get_send_id(){
    return sender;
}

int get_screen_id(){
    return screen;
}

int get_recv_id(){
    return reciever;
}

void* keyboard_input_thread(void* data){
    //accessing the data structure
    struct screen_thread* local_data = (struct screen_thread *) data;
    List* msg_to_be_sent = local_data -> list_m;

    char* buf = (char*) malloc(sizeof(char)*SIZE); //dynamically allocating the size of buffer
    bzero(buf, SIZE);

    if(buf == NULL){
        printf("buffer is NULL");
    }

    int escape_char = 0;

    //s-talk will terminate when user types "!"
    while(!escape_char){
        pthread_mutex_lock(&sent_mutex);

        //if there are left msg to be sent, wait
        if(List_count(msg_to_be_sent) >= 1){
            pthread_cond_wait(&msg_ready_to_send, &sent_mutex);
        }

        //clear the buffer and add new keystrokes to list
        bzero(buf, SIZE);
        read(STDIN_FILENO, buf, SIZE);
        List_prepend(msg_to_be_sent, buf);

        if(strcmp(buf, ESCAPE_CHAR) == 0){
            escape_char = 1;
        }

        pthread_cond_signal(&msg_ready_to_send);
        pthread_mutex_unlock(&sent_mutex);
    }

    //wait for other threads to finish
    printf("Closing threads.\n");

    printf("Freeing Memory\n");
    free(buf);

    printf("Clean\n");
    pthread_cancel(thread[sender]);
    pthread_cancel(thread[reciever]);
    pthread_cancel(thread[screen]);

    pthread_exit(NULL);
    }

void* send_thread(void* data){
    //accessing the data structure
    struct send_recv_data_thread* local_data = (struct send_recv_data_thread *) data;
    int socket = local_data -> socket;
    struct sockaddr_in* client_addr = local_data -> client_addr;
    int client_size = local_data -> size;
    List* msg_to_send = local_data -> shared_data;

    int escape_char = 0;

    while(!escape_char){
        pthread_mutex_lock(&sent_mutex);

        //if there is no msg to be sent then wait
        if(List_count(msg_to_send) == 0){
            pthread_cond_wait(&msg_ready_to_send, &sent_mutex);
        }

        //find most recent msg
        List_last(msg_to_send);

        //allocating memory, and clear
        int msglen = strlen(List_curr(msg_to_send));
        char msg[msglen];
        bzero(msg, msglen);

        //making local copy of msg
        strcpy(msg, List_curr(msg_to_send));

        //check if the msg is the terminator (which is "!")
        if(strcmp(msg, ESCAPE_CHAR) == 0){
            escape_char = 1;
        }

        //msg and clear from memory
        send_msg(socket, msg, msglen, client_addr, client_size);
        List_trim(msg_to_send);
        bzero(msg, msglen);

        pthread_cond_signal(&msg_ready_to_send);
        pthread_mutex_destroy(&sent_mutex);

    }

    printf("Closing connection\n");
    close(socket);

    pthread_cond_destroy(&msg_ready_to_send);
    pthread_mutex_destroy(&sent_mutex);
}

void* screen_output_thread(void* data){
    int escape_char = 0;

    while(!escape_char){
        struct screen_thread* local_data = (struct screen_thread *) data;
        List* msg_to_output = local_data -> list_m;

        //if there is no msg to output or its null, then
        if(msg_to_output != NULL && List_count(msg_to_output) != 0){
            pthread_mutex_lock(&msg_mutex);

            //if there is no msg to be output, wait
            if(List_count(msg_to_output) == 0){
                pthread_cond_wait(&msg_ready_to_output, &msg_mutex);
            }

            //locate correct msg, and output to user
            List_last(msg_to_output);
            printf("%s", (char*) List_curr(msg_to_output));

            if(strcmp(List_curr(msg_to_output), ESCAPE_CHAR) == 0){
                escape_char = 1;
            }

            bzero(List_trim(msg_to_output), SIZE);

            pthread_cond_signal(&msg_ready_to_output);
            pthread_mutex_unlock(&msg_mutex);
        }
    }

    pthread_cond_destroy(&msg_ready_to_output);
    pthread_mutex_destroy(&msg_mutex);
}

void* recv_thread(void* data){
    //accessing the data structure
    struct send_recv_data_thread* local_data = (struct send_recv_data_thread*) data;
    int socket = local_data -> socket;
    struct sockaddr_in* client_addr = local_data -> client_addr;
    int len = local_data -> size;
    List* msg_recieved_list = local_data -> shared_data;
    int buf_size = local_data -> msg_len;

    char* buf = (char*) malloc(sizeof(char)*SIZE); //dynamically allocating the size of buffer
    bzero(buf, SIZE);

    int byte_recieved = 1;
    int escape_char = 0;

    while(!escape_char){
        byte_recieved = recv_msg(socket, buf, buf_size, client_addr, &len);

        if(strcmp(buf, ESCAPE_CHAR) != 0 && byte_recieved > 0){
            pthread_mutex_lock(&msg_mutex);

            //if there are msgs that still need to be outputed, then wait
            if(List_count(msg_recieved_list) > 0){
                pthread_cond_wait(&msg_recieved, &msg_mutex);
            }

            //add recieved msg to list
            List_append(msg_recieved_list, buf);

            pthread_cond_signal(&msg_recieved);
            pthread_mutex_unlock(&msg_mutex);
        }

        else{
            escape_char = 1;
        }
    }

    //wait for other threads to finish
    printf("Closing threads\n");

    printf("Closing connection.\n");
    close(socket);

    printf("Freeing memory\n");
    free(buf);

    printf("Clean\n");
    pthread_cond_destroy(&msg_recieved);

    pthread_cancel(thread[sender]);
    pthread_cancel(thread[keyboard]);
    pthread_cancel(thread[screen]);

    pthread_exit(NULL);
}