#include "uchat_server.h"

/********************************************************************************/
                        /* Test version of server */
/********************************************************************************/

#define MAX_CLIENTS 30
#define MAX_BUFFER  1025
#define MAX_LISTEN_SOCKETS 50

typedef struct s_server {
    bool run;
    int option;
    int timeout;
    int poll_size;
    int poll_number;
    int master_socket;
    int client_socket[MAX_CLIENTS];
    struct pollfd fds[MAX_CLIENTS + 1];
    struct sockaddr_in address; 
}              t_server;

void init_server(t_server *server)
{
    server->run = true;
    server->option = 1;
    server->timeout = (3 * 60 * 1000); // 3 min.

    for(int i = 0; i < MAX_CLIENTS; ++i)  
        server->client_socket[i] = 0;

    if((server->master_socket = socket(AF_INET , SOCK_STREAM , 0)) < 0) {  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  

    if(setsockopt(server->master_socket, SOL_SOCKET, SO_REUSEADDR, 
      (char *) &server->option, sizeof(server->option))) {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);
    }  
    
    // Set socket to be nonblocking. All of the sockets for      
    // the incoming connections will also be nonblocking since   
    // they will inherit that state from the listening socket.
    if(ioctl(server->master_socket, FIONBIO, (char *) &server->option) < 0) {
        perror("ioctl failed");
        close(server->master_socket);
        exit(-1);
    }

    //type of socket created 
    server->address.sin_family = AF_INET; 
    server->address.sin_addr.s_addr = INADDR_ANY;  
    server->address.sin_port = htons(PORT);  

    if(bind(server->master_socket, (struct sockaddr*) &server->address, sizeof(server->address))) {  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }

    if(listen(server->master_socket, MAX_LISTEN_SOCKETS) < 0) {
        perror("listen failed");
        close(server->master_socket);
        exit(-1);
    }
}

void set_poll_fd(struct pollfd *fds, int fd, int index)
{
    fds[index].fd = fd;
    fds[index].events = POLLIN;
    fds[index].revents = 0;
}

void init_poll_set(t_server *server) 
{
    memset(server->fds, 0, sizeof(server->fds));
    set_poll_fd(server->fds, server->master_socket, 0);
    server->poll_size = 1;
}

int get_free_socket(int socket[MAX_CLIENTS])
{
    for(int index = 0; index < MAX_CLIENTS; ++index) {
        if(!socket[index])
            return index;
    }

    return -1;
}

void process_poll_recvest(t_server *server)
{
    for(int index = 1; server->poll_number; ++index) {
        if(!server->fds[index].revents)
            continue;

        int connection = true;

        while(connection) {
            char buffer[MAX_BUFFER];

            //read buffer
            if((recv(server->fds[index].fd, buffer, sizeof(buffer), 0)) < 0) {
                if(errno != EWOULDBLOCK) {
                    perror("recv failed");
                    connection = false;
                }

                break;
            }

            //process request
            // process_request();

            //reply
            char *answer = "test responce\n";

            if(send(server->fds[index].fd, answer, strlen(answer), 0) < 0) {
                perror("send failed");
                connection = false;
                break;
            }
        }
    }

    if(server->fds[0].revents) {
        int free_socket;
        int new_sd = accept(server->master_socket, NULL, NULL);

        while(new_sd != -1) {
            if((new_sd = accept(server->master_socket, NULL, NULL)) < 0) {
                if(errno != EWOULDBLOCK) {
                    perror("accept failed");
                    server->run = false;
                }

                break;
            }

            if((free_socket = get_free_socket(server->client_socket)) >= 0)
                set_poll_fd(server->fds, new_sd, free_socket);
            else {
                printf("all sockets busy\n");
                close(new_sd);
                break;
            }

            new_sd = accept(server->master_socket, NULL, NULL);
        }
    }
}

int main(int argc , char *argv[])
{
    t_server server;
    init_server(&server);
    init_poll_set(&server);

    while(server.run) {
        if((server.poll_number = poll(server.fds, server.poll_size, server.timeout)) < 0)
            perror("poll error");
        else if(server.poll_number > 0) {
            process_poll_recvest(&server);
        }
    }
}


/********************************************************************************/
                        /* Standart Socket Example */
/********************************************************************************/
/*

int main(int argc, char *argv[]) {
    printf("\033[33;1mINITIALISE SERVER\033[0m\n");
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    time_t ticks;

    //domain: AF_INET (IPv4), AF_INET6 (IPv6); type: SOCK_STREAM (TCP), SOCK_DGRAM (UDP); protocol: 0 - Internet Protocol (IP) 
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);
    printf("\r\033[32;1mRUN SERVER\033[0m\n");

    //initialise poll structure
    struct pollfd pollfds[2];

    pollfds[0].fd = listenfd;
    pollfds[0].events = POLL_IN;
    
    pollfds[1].fd = STDIN_FILENO;
    pollfds[1].events = POLL_IN;

    while(1) {
        int poll_status = poll(pollfds, 2, 1000);

        if(poll_status == -1) {
            //ERROR
        }
        else if(poll_status == 0) {
            //TIMEOUT
        }
        else {
            if(pollfds[0].revents & POLLIN ) { //socet processing
                pollfds[0].revents = 0;
                
                connfd = accept(listenfd, NULL, NULL);

                //READ CLIENT REQUEST

                //UNSWER
                write(connfd, sendBuff, strlen(sendBuff));

                close(connfd);
            }

            if(pollfds[1].revents & POLLOUT ) { //server standart input processing 
                pollfds[1].revents = 0;
            }
        }
    }

    printf("\033[31;1mSTOP SERVER\033[0m\n");

    return EXIT_SUCCESS;
}
*/









/********************************************************************************/
                        /* Advanced Socket Example */
/********************************************************************************/


//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux  
// #define SERVER_PORT 50000

// main (int argc, char *argv[])
// {
//   int    len, rc, on = 1;
//   int    listen_sd = -1, new_sd = -1;
//   int    desc_ready, end_server = false, compress_array = false;
//   int    close_conn;
//   char   buffer[80];
//   struct sockaddr_in6   addr;
//   int    timeout;
//   struct pollfd fds[200];
//   int    nfds = 1, current_size = 0, i, j;

//   /*************************************************************/
//   /* Create an AF_INET6 stream socket to receive incoming      */
//   /* connections on                                            */
//   /*************************************************************/
//   listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
//   if(listen_sd < 0) {
//     perror("socket() failed");
//     exit(-1);
//   }

//   /*************************************************************/
//   /* Allow socket descriptor to be reuseable                   */
//   /*************************************************************/
//   rc = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR,
//                   (char *)&on, sizeof(on));
//   if(rc < 0) {
//     perror("setsockopt() failed");
//     close(listen_sd);
//     exit(-1);
//   }

//   /*************************************************************/
//   /* Set socket to be nonblocking. All of the sockets for      */
//   /* the incoming connections will also be nonblocking since   */
//   /* they will inherit that state from the listening socket.   */
//   /*************************************************************/
//   rc = ioctl(listen_sd, FIONBIO, (char *)&on);
//   if (rc < 0)
//   {
//     perror("ioctl() failed");
//     close(listen_sd);
//     exit(-1);
//   }

//   /*************************************************************/
//   /* Bind the socket                                           */
//   /*************************************************************/
//   memset(&addr, 0, sizeof(addr));
//   addr.sin6_family      = AF_INET6;
//   memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
//   addr.sin6_port        = htons(SERVER_PORT);
//   rc = bind(listen_sd,
//             (struct sockaddr *)&addr, sizeof(addr));
//   if (rc < 0)
//   {
//     perror("bind() failed");
//     close(listen_sd);
//     exit(-1);
//   }

//   /*************************************************************/
//   /* Set the listen back log                                   */
//   /*************************************************************/
//   rc = listen(listen_sd, 32);
//   if (rc < 0)
//   {
//     perror("listen() failed");
//     close(listen_sd);
//     exit(-1);
//   }

//   /*************************************************************/
//   /* Initialize the pollfd structure                           */
//   /*************************************************************/
//   memset(fds, 0 , sizeof(fds));

//   /*************************************************************/
//   /* Set up the initial listening socket                        */
//   /*************************************************************/
//   fds[0].fd = listen_sd;
//   fds[0].events = POLLIN;
//   /*************************************************************/
//   /* Initialize the timeout to 3 minutes. If no                */
//   /* activity after 3 minutes this program will end.           */
//   /* timeout value is based on milliseconds.                   */
//   /*************************************************************/
//   timeout = (3 * 60 * 1000);

//   /*************************************************************/
//   /* Loop waiting for incoming connects or for incoming data   */
//   /* on any of the connected sockets.                          */
//   /*************************************************************/
//   do {
//     /***********************************************************/
//     /* Call poll() and wait 3 minutes for it to complete.      */
//     /***********************************************************/
//     printf("Waiting on poll()...\n");
//     rc = poll(fds, nfds, timeout);

//     /***********************************************************/
//     /* Check to see if the poll call failed.                   */
//     /***********************************************************/
//     if (rc < 0) {
//         perror("  poll() failed");
//         break;
//     }

//     /***********************************************************/
//     /* Check to see if the 3 minute time out expired.          */
//     /***********************************************************/
//     if (rc == 0) {
//         printf("  poll() timed out.  End program.\n");
//         break;
//     }

//     /***********************************************************/
//     /* One or more descriptors are readable.  Need to          */
//     /* determine which ones they are.                          */
//     /***********************************************************/
//     current_size = nfds;

//     for (i = 0; i < current_size; i++) {
//         /*********************************************************/
//         /* Loop through to find the descriptors that returned    */
//         /* POLLIN and determine whether it's the listening       */
//         /* or the active connection.                             */
//         /*********************************************************/
//         if(fds[i].revents == 0)
//             continue;

//         /*********************************************************/
//         /* If revents is not POLLIN, it's an unexpected result,  */
//         /* log and end the server.                               */
//         /*********************************************************/
//         if(fds[i].revents != POLLIN) {
//             printf("  Error! revents = %d\n", fds[i].revents);
//             end_server = true;
//             break;
//         }

//         if(fds[i].fd == listen_sd) {
//             /*******************************************************/
//             /* Listening descriptor is readable.                   */
//             /*******************************************************/
//             printf("Listening socket is readable\n");

//             /*******************************************************/
//             /* Accept all incoming connections that are            */
//             /* queued up on the listening socket before we         */
//             /* loop back and call poll again.                      */
//             /*******************************************************/
//             do {
//             /*****************************************************/
//             /* Accept each incoming connection. If               */
//             /* accept fails with EWOULDBLOCK, then we            */
//             /* have accepted all of them. Any other              */
//             /* failure on accept will cause us to end the        */
//             /* server.                                           */
//             /*****************************************************/
//                 if((new_sd = accept(listen_sd, NULL, NULL)) < 0) {
//                     if(errno != EWOULDBLOCK) {
//                         perror("accept failed");
//                         end_server = true;
//                     }

//                     break;
//                 }

//             /*****************************************************/
//             /* Add the new incoming connection to the            */
//             /* pollfd structure                                  */
//             /*****************************************************/
//             printf("New incoming connection - %d\n", new_sd);
//             fds[nfds].fd = new_sd;
//             fds[nfds++].events = POLLIN;

//             /*****************************************************/
//             /* Loop back up and accept another incoming          */
//             /* connection                                        */
//             /*****************************************************/
//             } while (new_sd != -1);
//         }

//       /*********************************************************/
//       /* This is not the listening socket, therefore an        */
//       /* existing connection must be readable                  */
//       /*********************************************************/

//       else
//       {
//         printf("  Descriptor %d is readable\n", fds[i].fd);
//         close_conn = false;
//         /*******************************************************/
//         /* Receive all incoming data on this socket            */
//         /* before we loop back and call poll again.            */
//         /*******************************************************/

//         do
//         {
//           /*****************************************************/
//           /* Receive data on this connection until the         */
//           /* recv fails with EWOULDBLOCK. If any other         */
//           /* failure occurs, we will close the                 */
//           /* connection.                                       */
//           /*****************************************************/
//           rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
//           if (rc < 0) {
//             if (errno != EWOULDBLOCK) {
//               perror("recv failed");
//               close_conn = true;
//             }
//             break;
//           }

//           /*****************************************************/
//           /* Check to see if the connection has been           */
//           /* closed by the client                              */
//           /*****************************************************/
//             if (rc == 0) {
//                 printf("  Connection closed\n");
//                 close_conn = true;
//                 break;
//             }

//           /*****************************************************/
//           /* Data was received                                 */
//           /*****************************************************/
//           len = rc;
//           printf("  %d bytes received\n", len);

//           /*****************************************************/
//           /* Echo the data back to the client                  */
//           /*****************************************************/
//           rc = send(fds[i].fd, buffer, len, 0);
//           if (rc < 0) {
//             perror("  send() failed");
//             close_conn = true;
//             break;
//           }

//         } while(true);

//         /*******************************************************/
//         /* If the close_conn flag was turned on, we need       */
//         /* to clean up this active connection. This            */
//         /* clean up process includes removing the              */
//         /* descriptor.                                         */
//         /*******************************************************/
//         if (close_conn) {
//           close(fds[i].fd);
//           fds[i].fd = -1;
//           compress_array = true;
//         }
//       }  /* End of existing connection is readable             */
//     } /* End of loop through pollable descriptors              */

//     /***********************************************************/
//     /* If the compress_array flag was turned on, we need       */
//     /* to squeeze together the array and decrement the number  */
//     /* of file descriptors. We do not need to move back the    */
//     /* events and revents fields because the events will always*/
//     /* be POLLIN in this case, and revents is output.          */
//     /***********************************************************/
//     if (compress_array)
//     {
//       compress_array = false;
//       for (i = 0; i < nfds; i++)
//       {
//         if (fds[i].fd == -1)
//         {
//           for(j = i; j < nfds; j++)
//           {
//             fds[j].fd = fds[j+1].fd;
//           }
//           i--;
//           nfds--;
//         }
//       }
//     }

//   } while (end_server == false); /* End of serving running.    */

//   /*************************************************************/
//   /* Clean up all of the sockets that are open                 */
//   /*************************************************************/
//   for (i = 0; i < nfds; i++)
//   {
//     if(fds[i].fd >= 0)
//       close(fds[i].fd);
//   }
// }










/*
int main(int argc , char *argv[])  
{  
    int opt = true;  
    int master_socket , addrlen , new_socket , client_socket[30] , 
          max_clients = 30 , activity, i , valread , sd;  
    int max_sd;  
    struct sockaddr_in address;

    char buffer[1025];  //data buffer of 1K 
         
    //set of socket descriptors 
    fd_set readfds;  
         
    //a message 
    char *message = "ECHO Daemon v1.0 \r\n";  
     
    //initialise all client_socket[] to 0 so not checked 
    for(i = 0; i < max_clients; i++)  
        client_socket[i] = 0;
         
    //create a master socket 
    //domain: AF_INET (IPv4), AF_INET6 (IPv6)
    //type: SOCK_STREAM (TCP), SOCK_DGRAM (UDP)
    //protocol: 0 - Internet Protocol (IP) 
    if(!(master_socket = socket(AF_INET , SOCK_STREAM , 0))) {  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  
     
    //set master socket to allow multiple connections , 
    //Allow socket descriptor to be reuseable
    if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
       sizeof(opt)) < 0 ) {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  
     
    //type of socket created 
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons(PORT);  
         
    //bind the socket to localhost port
    if(bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) {  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }  

    // printf("Listener on port %d \n", PORT);
         
    //try to specify maximum of 3 pending connections for the master socket 
    if(listen(master_socket, 3) < 0) {  
        perror("listen");  
        exit(EXIT_FAILURE);  
    }
         
    //accept the incoming connection 
    addrlen = sizeof(address);  
    puts("Waiting for connections ...");  
         
    while(true) {  
        //clear the socket set 
        FD_ZERO(&readfds);  
     
        //add master socket to set 
        FD_SET(master_socket, &readfds);  
        max_sd = master_socket;  
             
        //add child sockets to set 
        for(i = 0 ; i < max_clients ; i++)  
        {  
            //socket descriptor 
            sd = client_socket[i];  
                 
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET(sd , &readfds);  
                 
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }  
     
        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);  
       
        if((activity < 0) && (errno!=EINTR))
            printf("select error"); 
             
        //If something happened on the master socket , 
        //then its an incoming connection 
        if(FD_ISSET(master_socket, &readfds))  
        {  
            if((new_socket = accept(master_socket, 
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {  
                perror("accept");  
                exit(EXIT_FAILURE);
            }  
             
            //inform user of socket number - used in send and receive commands 
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n", 
                   new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
           
            //send new connection greeting message 
            if(send(new_socket, message, strlen(message), 0) != strlen(message)) {  
                perror("send");  
            }  
                 
            puts("Welcome message sent successfully");  
                 
            //add new socket to array of sockets 
            for(i = 0; i < max_clients; i++) {  
                //if position is empty 
                if(!client_socket[i]) {  
                    client_socket[i] = new_socket;  
                    // Adding to list of sockets as i
                    break;  
                }  
            }  
        }  
             
        //else its some IO operation on some other socket
        for(i = 0; i < max_clients; i++) {  
            sd = client_socket[i];  
                 
            if(FD_ISSET(sd , &readfds)) {  
                //Check if it was for closing , and also read the 
                //incoming message 
                if(!(valread = read( sd , buffer, 1024))) {  
                    //Somebody disconnected , get his details and print 
                    getpeername(sd , (struct sockaddr*)&address , \
                        (socklen_t*)&addrlen);  
                    printf("Host disconnected , ip %s , port %d \n" , 
                          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                         
                    //Close the socket and mark as 0 in list for reuse 
                    close(sd);  
                    client_socket[i] = 0;  
                }  
                     
                //Echo back the message that came in 
                else {  
                    //set the string terminating NULL byte on the end 
                    //of the data read 
                    buffer[valread] = '\0';  
                    send(sd , buffer , strlen(buffer) , 0 );  
                }  
            }  
        }  
    }  
         
    return 0;  
}
*/

/********************************************************************************/
                            /* ZeroMQ Example */
/********************************************************************************/
/*
#include "zmq.h"
#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <sys/wait.h>
 
#define SOCKET_STRING                 "tcp://127.0.0.1:1102"
#define INTERNAL_WORKER_ADDRESS       "inproc://workers"
#define PACKET_SIZE                   1024
#define OUTPUT_BUFFER_SIZE            PACKET_SIZE*5
#define MAX_CLIENTS                   10
#define TEST_DURATION                 10
#define WORKER_THREAD_COUNT           2
#define WORKER_WORK_TIME              2000
 
struct ThreadStats
{
  unsigned long long packets_received;
  unsigned long long bytes_received;
};
 
bool flag_kill;
void *z_ctx;
 
//---------------------------------------------------------------------------
static void *client_thread(void *)
{
  void *sock = zmq_socket(z_ctx, ZMQ_DEALER);
  if (!sock)
    return NULL;
 
  if (zmq_connect(sock, SOCKET_STRING) < 0)
  {
    zmq_close(sock);
    return NULL;
  }
 
  char output_buffer[OUTPUT_BUFFER_SIZE];
  int output_buffer_len = 0;
 
  // отправляем на сервер несколько пакетов
  while (output_buffer_len+PACKET_SIZE <= sizeof(output_buffer))
  {
    int bytes_sent = zmq_send(sock, output_buffer+output_buffer_len, PACKET_SIZE, ZMQ_DONTWAIT);
    output_buffer_len += bytes_sent;
  }
 
  ThreadStats *stats = new ThreadStats;
  stats->packets_received = 0;
  stats->bytes_received = 0;
 
  // начинаем мониторить события сокета
  zmq_pollitem_t poll_fd;
  poll_fd.socket = sock;
  poll_fd.events = ZMQ_POLLIN;
  poll_fd.revents = 0;
 
  while(!flag_kill)
  {
    int res = zmq_poll(&poll_fd, 1, 500);
    if (res < 0)
      break;
    if (res == 0)
      continue;
 
    // есть новый пакет
    if (poll_fd.revents & ZMQ_POLLIN)
    {
      poll_fd.revents &= ~ZMQ_POLLIN;
 
      // читаем пакет с данными
      zmq_msg_t rcv_msg;
      if (zmq_msg_init(&rcv_msg) < 0)
      {
        flag_kill = true;
        break;
      }
 
      int rc = zmq_msg_recv(&rcv_msg, sock, ZMQ_DONTWAIT);
      if (rc < 0)
      {
        flag_kill = true;
        break;
      }
 
      int msg_size = zmq_msg_size(&rcv_msg);
      char *msg_buffer = (char *)zmq_msg_data(&rcv_msg);
      stats->bytes_received += msg_size;
      stats->packets_received++;
 
      // отправляем пакет с данными
      if (zmq_send(sock, msg_buffer, msg_size, ZMQ_DONTWAIT) < 0)
      {
        printf("zmq_send() error: %s\r\n", zmq_strerror(errno));
        break;
      }
 
      zmq_msg_close(&rcv_msg);
    }
  }
 
  zmq_close(sock);
  return stats;
}
 
//---------------------------------------------------------------------------
static void *worker_thread(void *)
{
  void *sock = zmq_socket(z_ctx, ZMQ_REP);
  if (!sock)
    return NULL;
 
  if (zmq_connect(sock, INTERNAL_WORKER_ADDRESS) < 0)
  {
    zmq_close(sock);
    return NULL;
  }
 
  // начинаем мониторить события сокета
  zmq_pollitem_t poll_fd;
  poll_fd.socket = sock;
  poll_fd.events = ZMQ_POLLIN;
  poll_fd.revents = 0;
 
  while(!flag_kill)
  {
    int res = zmq_poll(&poll_fd, 1, 500);
    if (res < 0)
      break;
    if (res == 0)
      continue;
 
    // есть новый пакет
    if (poll_fd.revents & ZMQ_POLLIN)
    {
      poll_fd.revents &= ~ZMQ_POLLIN;
 
      // читаем фрейм с идентификатором клиента
      char client_identity[255];
      int client_identity_len;
      client_identity_len = zmq_recv(sock, client_identity, sizeof(client_identity), ZMQ_DONTWAIT);
      if (client_identity_len <= 0)
        break;
 
      // читаем пакет с данными
      zmq_msg_t rcv_msg;
      if (zmq_msg_init(&rcv_msg) < 0)
      {
        flag_kill = true;
        break;
      }
 
      int rc = zmq_msg_recv(&rcv_msg, sock, ZMQ_DONTWAIT);
      if (rc < 0)
      {
        flag_kill = true;
        break;
      }
 
      int msg_size = zmq_msg_size(&rcv_msg);
      char *msg_buffer = (char *)zmq_msg_data(&rcv_msg);
 
      // имитируем обработку запроса (2ms)
      usleep(WORKER_WORK_TIME);
 
      // отправляем фрейм с идентификатором клиента
      if (zmq_send(sock, client_identity, client_identity_len, ZMQ_DONTWAIT | ZMQ_SNDMORE) < 0)
        break;
 
      // отправляем пакет с данными обратно
      if (zmq_send(sock, msg_buffer, msg_size, ZMQ_DONTWAIT) < 0)
      {
        printf("zmq_send() error: %s\r\n", zmq_strerror(errno));
        break;
      }
 
      zmq_msg_close(&rcv_msg);
    }
  }
 
  zmq_close(sock);
  return NULL;
}
 
//---------------------------------------------------------------------------
void termination_handler(int)
{
  flag_kill = true;
}
//---------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  signal(SIGTERM, termination_handler);
  signal(SIGSTOP, termination_handler);
  signal(SIGINT,  termination_handler);
  signal(SIGQUIT, termination_handler);
 
  z_ctx = zmq_ctx_new();
  if (!z_ctx)
    return 1;
 
//  zmq_ctx_set(z_ctx, ZMQ_IO_THREADS, 2);
 
  // стартуем сокет для обслуживания клиентов
  void *sock_srv = zmq_socket(z_ctx, ZMQ_ROUTER);
  if (!sock_srv)
  {
    zmq_ctx_destroy(z_ctx);
    return 2;
  }
  if (zmq_bind(sock_srv, SOCKET_STRING) < 0)
  {
    zmq_close(sock_srv);
    zmq_ctx_destroy(z_ctx);
    return 3;
  }
 
  // стартуем сокет для обслуживания потоков worker-ов
  void *sock_workers = zmq_socket(z_ctx, ZMQ_DEALER);
  if (!sock_workers)
  {
    zmq_close(sock_srv);
    zmq_ctx_destroy(z_ctx);
    return 2;
  }
  if (zmq_bind(sock_workers, INTERNAL_WORKER_ADDRESS) < 0)
  {
    zmq_close(sock_srv);
    zmq_close(sock_workers);
    zmq_ctx_destroy(z_ctx);
    return 3;
  }
 
  // запускаем потоки worker-ов
  pthread_t worker_thread_ids[WORKER_THREAD_COUNT];
  for (int i=0; i < WORKER_THREAD_COUNT; i++)
  {
    pthread_create(&worker_thread_ids[i], NULL, &worker_thread, NULL);
  }
 
  // запускаем потоки клиентов
  pthread_t thread_ids[MAX_CLIENTS];
  for (int i=0; i < MAX_CLIENTS; i++)
  {
    pthread_create(&thread_ids[i], NULL, &client_thread, NULL);
  }
 
  // начинаем мониторить события сокета
  zmq_pollitem_t poll_fds[2];
  poll_fds[0].socket = sock_srv;
  poll_fds[0].events = ZMQ_POLLIN;
  poll_fds[0].revents = 0;
  poll_fds[1].socket = sock_workers;
  poll_fds[1].events = ZMQ_POLLIN;
  poll_fds[1].revents = 0;
 
  struct timespec ts_start;
  struct timespec ts_current;
  clock_gettime(CLOCK_MONOTONIC, &ts_start);
  double start_time = ts_start.tv_sec + (double)ts_start.tv_nsec/1000000000;
  double cur_time = ts_start.tv_sec + (double)ts_start.tv_nsec/1000000000;
  zmq_msg_t rcv_msg;
  while(!flag_kill)
  {
    clock_gettime(CLOCK_MONOTONIC, &ts_current);
    cur_time = ts_current.tv_sec + (double)ts_current.tv_nsec/1000000000;
    if (cur_time-start_time > TEST_DURATION)
    {
      flag_kill = true;
      break;
    }
    int res = zmq_poll(poll_fds, 2, 500);
    if (res < 0)
      break;
    if (res == 0)
      continue;
 
    // есть новый пакет от клиента
    if (poll_fds[0].revents & ZMQ_POLLIN)
    {
      poll_fds[0].revents &= ~ZMQ_POLLIN;
 
      // читаем фрейм с идентификатором клиента
      char client_identity[255];
      int client_identity_len;
      client_identity_len = zmq_recv(sock_srv, client_identity, sizeof(client_identity), ZMQ_DONTWAIT);
      if (client_identity_len <= 0)
        break;
 
      // читаем фрейм с пакетом данных
      if (zmq_msg_init(&rcv_msg) < 0)
      {
        flag_kill = true;
        break;
      }
      int rc = zmq_msg_recv(&rcv_msg, sock_srv, ZMQ_DONTWAIT);
      if (rc < 0)
      {
        flag_kill = true;
        break;
      }
      int msg_size = zmq_msg_size(&rcv_msg);
 
      // отправляем delimiter-фрейм worker-у
      if (zmq_send(sock_workers, "", 0, ZMQ_DONTWAIT | ZMQ_SNDMORE) < 0)
        break;
 
      // отправляем фрейм с идентификатором клиента worker-у
      // worker нам его вернет обратно вместе с ответом на запрос
      // и мы будем знать какому клиенту направить ответ
      if (zmq_send(sock_workers, client_identity, client_identity_len, ZMQ_DONTWAIT | ZMQ_SNDMORE) < 0)
        break;
 
      // отправляем фрейм с пакетом данных worker-у
      if (zmq_send(sock_workers, zmq_msg_data(&rcv_msg), msg_size, ZMQ_DONTWAIT) < 0)
        break;
 
      zmq_msg_close(&rcv_msg);
    }
 
    // есть новый пакет от worker-а
    if (poll_fds[1].revents & ZMQ_POLLIN)
    {
      poll_fds[1].revents &= ~ZMQ_POLLIN;
 
      // читаем delimiter-фрейм
      char delimiter_buf[2];
      int delimiter_buf_len;
      delimiter_buf_len = zmq_recv(sock_workers, delimiter_buf, sizeof(delimiter_buf), ZMQ_DONTWAIT);
      if (delimiter_buf_len > 0)
        break;
 
      // читаем фрейм с идентификатором клиента
      char client_identity[255];
      int client_identity_len;
      client_identity_len = zmq_recv(sock_workers, client_identity, sizeof(client_identity), ZMQ_DONTWAIT);
      if (client_identity_len <= 0)
        break;
 
      // читаем фрейм с пакетом данных
      if (zmq_msg_init(&rcv_msg) < 0)
      {
        flag_kill = true;
        break;
      }
      int rc = zmq_msg_recv(&rcv_msg, sock_workers, ZMQ_DONTWAIT);
      if (rc < 0)
      {
        flag_kill = true;
        break;
      }
      int msg_size = zmq_msg_size(&rcv_msg);
 
      // отправляем фрейм с идентификатором клиента ROUTER-сокету
      if (zmq_send(sock_srv, client_identity, client_identity_len, ZMQ_DONTWAIT | ZMQ_SNDMORE) < 0)
        break;
 
      // отправляем фрейм с пакетом данных клиенту
      if (zmq_send(sock_srv, zmq_msg_data(&rcv_msg), msg_size, ZMQ_DONTWAIT) < 0)
        break;
 
      zmq_msg_close(&rcv_msg);
    }
  }
  double elapsed = cur_time-start_time;
 
  // ждем завершения потоков
  usleep(600000);
 
  for (int i=0; i < WORKER_THREAD_COUNT; i++)
  {
    void *res = NULL;
    pthread_join(worker_thread_ids[i], &res);
  }
 
  unsigned long long total_packets_received = 0;
  unsigned long long total_bytes_received = 0;
  // получаем статистику по потокам и считаем итог
  printf("thread id:\tbytes rcv\tpackets rcv\r\n");
  for (int i=0; i < MAX_CLIENTS; i++)
  {
    void *res = NULL;
    pthread_join(thread_ids[i], &res);
    ThreadStats *stat = (ThreadStats *)res;
    if (stat)
    {
      printf("thread %02d:\t%lld\t%lld\r\n", i+1, stat->bytes_received, stat->packets_received);
      total_bytes_received += stat->bytes_received;
      total_packets_received += stat->packets_received;
      delete stat;
    }
  }
  printf("    TOTAL:\t%lld\t%lld\r\n", total_bytes_received, total_packets_received);
  printf("\r\n");
  printf("Elapsed time: %.3lf\r\n", elapsed);
  printf("Avg speed: %d bytes/s, %d packets/s\r\n", (unsigned int)(total_bytes_received/elapsed), (unsigned int)(total_packets_received/elapsed));
 
  zmq_close(sock_workers);
  zmq_close(sock_srv);
  zmq_ctx_destroy(z_ctx);
  return 0;
}
*/
