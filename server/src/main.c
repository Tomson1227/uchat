#include "uchat_server.h"

// /********************************************************************************/
//                         /* Test version of server */
// /********************************************************************************/

// void init_server(t_server *server)
// {
//     server->run = true;
//     server->option = 1;
//     server->timeout = (100); // 5 sec.

//     if((server->master_socket = socket(AF_INET , SOCK_STREAM , 0)) < 0) {  
//         perror("socket failed");  
//         exit(EXIT_FAILURE);  
//     }  

//     if(setsockopt(server->master_socket, SOL_SOCKET, SO_REUSEADDR, 
//       (char *) &server->option, sizeof(server->option))) {  
//         perror("setsockopt");  
//         exit(EXIT_FAILURE);
//     }  
    
//     // Set socket to be nonblocking. All of the sockets for      
//     // the incoming connections will also be nonblocking since   
//     // they will inherit that state from the listening socket.
//     if(ioctl(server->master_socket, FIONBIO, (char *) &server->option) < 0) {
//         perror("ioctl failed");
//         close(server->master_socket);
//         exit(-1);
//     }

//     //type of socket created 
//     server->address.sin_family = AF_INET; 
//     server->address.sin_addr.s_addr = INADDR_ANY;  
//     server->address.sin_port = htons(PORT);  

//     if(bind(server->master_socket, (struct sockaddr*) &server->address, sizeof(server->address))) {  
//         perror("bind failed");  
//         exit(EXIT_FAILURE);  
//     }

//     if(listen(server->master_socket, MAX_LISTEN_SOCKETS) < 0) {
//         perror("listen failed");
//         close(server->master_socket);
//         exit(-1);
//     }
// }

// void print_sockets(t_server *server)
// {
// 	RESTORE_CURSOR_POS;
	
// 	time_t curent_time = time(NULL);

// 	for(int index = 1; index <= MAX_CLIENTS; ++index) {
// 		printf("SOCKET %3d: ", index);
		
// 		if(server->fds[index].fd != -1)
// 			printf("%s %4d TIME = %ld\n", STATUS_CONNECTED, server->fds[index].fd, (curent_time - server->sockets[index].begin));
// 		else
// 			printf("\033[37;2m[FREE]\033[0m\n");
// 	}
// }

// void set_pollin_fd(struct pollfd *fds, int fd, int index)
// {
//     fds[index].fd = fd;
//     fds[index].events = POLLIN;
//     fds[index].revents = 0;
// }

// void init_poll_set(t_server *server) 
// {
//     set_pollin_fd(server->fds, server->master_socket, 0);

// 	for(size_t index = 1; index <= MAX_CLIENTS; ++index)
// 	    set_pollin_fd(server->fds, -1, index);

//     server->poll_size = MAX_CLIENTS + 1;
// }

// int get_free_socket(struct pollfd *fds)
// {
//     for(int index = 1; index <= MAX_CLIENTS; ++index)
//         if(fds[index].fd == -1)
//             return index;

//     return -1;
// }

// void disconnect_socket(t_server *server, int index)
// {
// 	close(server->fds[index].fd);
// 	server->fds[index].fd = -1;
// }

// void process_request(t_server *server, int index)
// {
// 	if(!(server->fds[index].revents & POLLIN))
// 		return;

// 	char buffer[1025];
// 	char *responce = "Hello from server\n";

// 	if((recv(server->fds[index].fd, buffer, sizeof(buffer), 0)) < 0) {
// 		if(errno != EWOULDBLOCK) {
// 			perror("recv failed");
// 		}
// 	}

// 	printf(CSI "1E" "%s" CSI "1F", buffer);

// 	if(send(server->fds[index].fd, responce, strlen(responce), 0) < 0)
// 		perror("send failed");

// 	server->poll_number--;
// }

// void process_poll_recvest(t_server *server)
// {
//     if(server->fds[0].revents) {
// 		server->poll_number--;
//         int free_socket;
//         int new_sd = 0;

//         while(new_sd >= 0) {
//             if((new_sd = accept(server->master_socket, NULL, NULL)) < 0) {
//                 if(errno != EWOULDBLOCK) {
//                     perror("accept failed");
//                     server->run = false;
//                 }

//                 break;
//             }

//             if((free_socket = get_free_socket(server->fds)) >= 0) {
//                 set_pollin_fd(server->fds, new_sd, free_socket);
// 				server->sockets[free_socket].fd = new_sd;
// 				time(&server->sockets[free_socket].begin);
// 				send(new_sd, "Connected\n", 10, 0);
// 			}
//             else {
// 				printf("\nNEW CONNECTION %4d \033[31;1m[DECLINED]\033[0m", new_sd);
//                 close(new_sd);
//                 break;
//             }
//         }
//     }

//     for(int index = 1; index <= MAX_CLIENTS && server->poll_number; ++index) {
//         if(!server->fds[index].revents)
//             continue;

// 		process_request(server, index);
//     }
// }

// int main(int argc , char *argv[])
// {
//     t_server server;
//     init_server(&server);
//     init_poll_set(&server);
//     // signal(SIGPIPE, pipesig_hendler);
    
// 	SAVE_CURSOR_POS;
// 	print_sockets(&server);

//     while(server.run) {
//         if((server.poll_number = poll(server.fds, server.poll_size, server.timeout)) < 0)
//             perror("poll error");
//         else if(server.poll_number > 0) {
// 			process_poll_recvest(&server);
//         }

// 		print_sockets(&server);
//     }
// }
