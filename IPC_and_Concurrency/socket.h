//Jeongwoon Suh : 301313489 : CMPT 300 : Assignment 2

/*Description: 
In order for four threads to communicate with each other via shared list, creation of the socket
is required. Also, there are some implementation of network connections. 
The program will exit if there is an error on any critical sections. 
*/

//max buffer size for transeferring msg
#define SIZE 1024

//creating a UDP socket
int socket_create();

//initializing server info
void server_init(struct sockaddr_in* server_addr, int local_port);

//initializing client info
void client_init(struct sockaddr_in* client_addr, char server_name[], int port_num, struct hostent** client_info);

//bind socket to server
void bind_socket(int s, struct sockaddr_in* server_addr, int size);

//send msg
int send_msg(int s, char* msg , int msg_len, struct sockaddr_in* client_addr, int size);

//recieve msg
int recv_msg(int s, char* buf, int buf_len, struct sockaddr_in* client_addr, int* size);