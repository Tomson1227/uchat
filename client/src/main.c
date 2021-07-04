#include "uchat.h"

int main(int argc, char *argv[]) {
    int sockfd = 0, n = 0;
    int count = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    if(argc != 2) {
        printf("\n Usage: %s <ip of server> \n", argv[0]);
        return 1;
    }

    memset(recvBuff, '0', sizeof(recvBuff));

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
       printf("\n Error : Connect Failed \n");
       return 1;
    }

    while((n = recv(sockfd, recvBuff, sizeof(recvBuff), 0)) > 0) {

        if(count == 10) {
            close(sockfd);
            break;
        }

        recvBuff[n] = '\0';        

        printf("Recived: %s", recvBuff);
        
        sleep(1);
        char request[20];
        sprintf(request, "Request: %d\n", count++);
        printf("To send: %s", request);

        send(sockfd, request, sizeof(request), 0);
        
        sleep(1);
    }

    if(n < 0) {
        printf("\n Read error \n");
    }

    return 0;
}
