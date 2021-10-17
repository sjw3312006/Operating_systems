/* 
CMPT 300 Assignment 2
Jeongwoon Suh: 301313489

Description: 
Using four threads, keyboard input thread, UDP output thread, UDP input thread, and screen output thread,
this program enables someone at one terminal to communicate with the other at another terminal. 
It will appear character by character on the sender's terminal as they type it, and will appear on the receiver's 
terminal once the sender presses enter. 
It uses UDP sockets to commnicate over network, and will terminate once either user types "!".
*/

#include "socket.h"
#include "list.h"
#include "thread.h"
#include <sys/socket.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char** argv){

    int keyborad_thread, sender_thread, screen_thread, reciever_thread; //4 threads

    //initializing thread variables
    pthread_t* thread = get_threads();

    struct screen_thread ip_data[NUM_THREADS_IO]; //keyboard and screen
    struct send_recv_data_thread sr_data[NUM_THREADS_SR]; //sender and reciever

    //initializing thread data locations
    int keyboard_data = 0;
    int send_data = 0;
    int screen_data = 1;
    int recv_data = 1;

    //creating new list to store datas
    List* msg_to_send = List_create();
    List* msg_to_output = List_create();
    
    //initializing the ids of thread
    int keyboard_id = get_keyboard_id;
    int send_id = get_send_id;
    int screen_id = get_screen_id;
    int recv_id = get_recv_id;

    //intializing network utililty variables
    char buf[SIZE];
    char msg[SIZE];
    int n, len;

    int local_port = atoi(argv[1]);
    int remote_port = atoi(argv[3]);
    char remote_machine_name[128];
    strcpy(remote_machine_name, argv[2]);
    
    //initializing network functional variables
    int network_socket_server, network_socket_client;
    struct sockaddr_in server, client;
    struct hostent *client_info;

    //creating sockets for server and clinet
    network_socket_server = socket_create();
    network_socket_client = socket_create();

    server_init(&server, local_port); //initializing server info with local port number
    client_init(&client, remote_machine_name, remote_port, &client_info); //initializing client info
    bind_socket(network_socket_server, &server, sizeof(server)); //bind those two sockets

    bzero(msg, SIZE);
    bzero(buf, SIZE);

    //initializing the keyboard thread 
    ip_data[keyboard_data].list_m = msg_to_send;
    ip_data[screen_data].list_m = msg_to_output;

    //initializing the sender thread
    sr_data[send_data].socket = network_socket_client;
    sr_data[send_data].msg = msg;
    sr_data[send_data].msg_len = strlen(msg);
    sr_data[send_data].client_addr = &client;
    sr_data[send_data].size = sizeof(client);
    sr_data[send_data].shared_data = msg_to_send;

    //initializing the reciever thread
    sr_data[recv_data].socket = network_socket_server;
    sr_data[recv_data].msg = buf;
    sr_data[recv_data].msg_len = SIZE;
    sr_data[recv_data].client_addr = &server;
    sr_data[recv_data].size = sizeof(server);
    sr_data[recv_data].shared_data = msg_to_output;

    //starting all 4 threads
    keyborad_thread = pthread_create(&thread[keyboard_id], NULL, keyboard_input_thread, &ip_data[keyboard_data]);
    sender_thread = pthread_create(&thread[send_id], NULL, send_thread, &sr_data[send_data]);
    screen_thread = pthread_create(&thread[screen_id], NULL, screen_output_thread, &ip_data[screen_data]);
    reciever_thread = pthread_create(&thread[recv_id], NULL, recv_thread, &sr_data[recv_data]);

    //wait until all thereads are done executing

    int finish = 0;
    while(!finish){
        if(pthread_join(thread[screen_id], NULL) == 0){
            finish = 1;
        }
        else if(pthread_join(thread[send_id], NULL) == 0){
            finish = 1;
        }
    }

    printf("Done.");
}