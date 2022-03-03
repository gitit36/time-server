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

    // Socket()
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
    struct sockaddr_in srvaddr;
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(9000);
    srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    char buffer[256];
    
    while(1)
    {
        // string copy "time" into the buffer
        strncpy(buffer, "time", sizeof(buffer));

        // send the buffer to server
        sendto(serverfd, (char *)buffer, sizeof(buffer), 0, (const struct sockaddr *) &srvaddr, sizeof(srvaddr));
        
        // receive "time" from server
        recvfrom(serverfd, (char *)buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *) &srvaddr, 0);

        // check if buffer contains the year-mark, which indicates time
        if (strcasestr(buffer, "/22"))
        {
            // print server time
            printf("\nServer Time: %s\n\n", buffer);
            break;
        } 
    }
    close(serverfd);
    return 0;
}