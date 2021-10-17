//Jeongwoon Suh : 301313489 : CMPT 300 : Assignment 2

/*Description: 
In order for four threads to communicate with each other via shared list, creation of the socket
is required. Also, there are some implementation of network connections. 
The program will exit if there is an error on any critical sections. 
*/

#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>

void print_error(const char msg[]){ //to print error
    printf(stderr, "%s\n", msg);
    exit(1); //exit if any error occured
    return;
}

int socket_create(){
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if(s == -1){
        print_error("No server exists");
    }
    return s;
}

void server_init(struct sockaddr_in* server_addr, int local_port){
    if(!server_addr){
        print_error("No address of server exist.");
    }

    server_addr -> sin_family = AF_INET;
    server_addr -> sin_port = htons(local_port);
    server_addr -> sin_addr.s_addr = INADDR_ANY; //uses the IP of the local machine
    if(!memset(&server_addr -> sin_zero, "\0", 8)){
        print_error("No memset() exists.");
    }
    return;
}

void client_init(struct sockaddr_in* client_addr, char server_name[], int port_num, struct hostent** client_info){
    client_addr -> sin_family = AF_INET;
    *client_info = gethostbyname(server_name);
    if(!(*client_info)){
        print_error("No gethostbyname exists.");
    }
    client_addr -> sin_port = htons(port_num);
    bcopy((char*)(*client_info) -> h_addr, (char*)& client_addr->sin_addr, (*client_info)->h_length);
    return;
}

void bind_socket(int s, struct sockaddr_in* server_addr, int size){
    int binding = bind(s, (struct sockaddr*) server_addr, size); //opens socket & bind
    if(binding == -1){
        print_error("No bind() exists.");
    }
    return;
}

int send_msg(int s, char* msg , int msg_len, struct sockaddr_in* client_addr, int size){
    int data_sent = sendto(s, msg, msg_len, 0, (struct sockaddr *) client_addr, size); //to send the msg
    if(data_sent == -1){
        print_error("No data has been sent");
    }
    return data_sent;
}

int recv_msg(int s, char* buf, int buf_len, struct sockaddr_in* client_addr, int* size){
    int data_recv = recvfrom(s, buf, buf_len, 0, (struct sockaddr *) client_addr, size); //to receieve the msg
    if(data_recv == -1){
        print_error("No data has been recieved");
    }
}