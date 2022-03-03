#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>


int main() {
    int serverfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    // socket()
    if(serverfd < 0)
    {
        perror("socket error");
        return -1;
    }

    if(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int))<0)
    {
        perror("setsockopt error");
        exit(-1);
    }

    // Bind (update server information)
    struct sockaddr_in srvaddr, cliaddr;
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(9000);
    srvaddr.sin_addr.s_addr = INADDR_ANY;
    
    // Bind socket with the address of server
    if(bind(serverfd, (const struct sockaddr*) &srvaddr, sizeof(srvaddr))<0)
    {
        perror("bind error");
        return -1;
    }
    

    int len = sizeof(cliaddr);
    char buffer[256];

    while(1)
    {
        // reeceive buffer from client
        recvfrom(serverfd, (char *)buffer, sizeof(buffer), MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);

        // print a message for every incoming message 
        // indicating that a message has been received and from which IP address and port number
        printf("\nTIME SERVER STARTED ********************\n");
        printf("[Client Info] - %s (Port : %d)\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
        printf("****************************************\n\n");

        // if the message it received from server is equal to "time"
        if (strcmp(buffer,"time")==0)
        {
            // Getting the current time
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            // Storing time in the buffer to send it back to client
            strftime(buffer, sizeof(tm), "%x %X%p", &tm);
        }
        // send buffer/message to the client
        sendto(serverfd, (char *)buffer, sizeof(buffer), 0, (const struct sockaddr *) &cliaddr, len);
    }

    return 0;
}