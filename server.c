#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

void sigintHandler(int sig_num)
{
   signal(SIGINT, sigintHandler);
   printf("\nTerminating\n");
   exit(0);
}

int main(int argc, char *argv[])
{
   signal(SIGINT,sigintHandler);

   char rxBuffer[256],txBuffer[256];
   int sockFd, clientSocketFd, portNo, clientLen;
   struct sockaddr_in serverAdd, clientAdd;
   int n;
   int bye,byebye;

   if(argc < 2) {
        printf("Argument Error\n");
        exit(1);
   }

   sockFd = socket(AF_INET, SOCK_STREAM, 0); //Arg 1: using IP Arg 2: using TCP Arg 3: provide appropriate socket
   if(sockFd < 0) {
        printf("Error Opening Socket\n");
        exit(0);
   }

   portNo = atoi(argv[1]);

   //Address structure
   serverAdd.sin_family = AF_INET;
   serverAdd.sin_addr.s_addr = INADDR_ANY;  //any addr in the current host
   serverAdd.sin_port = htons(portNo); //convert values between host and network 

   //bind addr and socket
   if(bind(sockFd, (struct sockaddr *) &serverAdd, sizeof(serverAdd)) < 0) {
        printf("Binding Error\n");
        close(sockFd);
        exit(0);
   }

   listen(sockFd,1); // Arg 2: Number of Clients
   printf("Waiting for incoming connection...\n");

   clientLen = sizeof(clientAdd);

   //Accept incoming connection
   clientSocketFd = accept(sockFd, (struct sockaddr *) &clientAdd,(socklen_t *)&clientLen);

   if(clientSocketFd < 0) {
       printf("Error in accepting connection\n");
       close(clientSocketFd);
       close(sockFd);
       exit(0);
   }
   else {
       printf("Connected to %s\nWelcome to the chatApp\n",inet_ntoa(clientAdd.sin_addr));
   }

    do {

        bzero(rxBuffer,255);
        if(recv(clientSocketFd,rxBuffer,255,0) < 0) {
            printf("Error readingi from socket\n");
            break;
        }
        printf("Message: %s\n",rxBuffer);

        bye = strncasecmp(rxBuffer,"bye",3);
        printf("bye: %d\n",bye);
        if(bye == 0)
            break;

        bzero(txBuffer,255);
        printf("Enter message: ");
        fgets(txBuffer,255,stdin);
        n = send(clientSocketFd,txBuffer,strlen(txBuffer),0);
        if(n < 0) {
            printf("Error writing to socket\n");
            break;
        }
    
        byebye =  strncasecmp(txBuffer,"bye",3);
        printf("byebye: %d\n",byebye);
        if(byebye == 0)
            break;
   } while(1);

   printf("Closing sockets\n");
   close(clientSocketFd);
   close(sockFd);

   return 0;
}
