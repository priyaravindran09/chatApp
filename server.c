#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
   char buffer[256];
   int sockFd, clientSocketFd, portNo, clientLen;
   struct sockaddr_in serverAdd, clientAdd;
   int n;

   if(argc < 2) {
        printf("Argument Error\n");
        exit(1);
   }

   sockFd = socket(AF_INET, SOCK_STREAM, 0); //Arg 1: using IP Arg 2: using TCP Arg 3: provide appropriate socket
   if(sockFd < 0) {
        printf("Error Opening Socket\n");
   }

   bzero((char *) &serverAdd, sizeof(serverAdd));

   portNo = atoi(argv[1]);

   serverAdd.sin_family = AF_INET;
   serverAdd.sin_addr.s_addr = INADDR_ANY;
   serverAdd.sin_port = htons(portNo); //convert values between host and network 

   if(bind(sockFd, (struct sockaddr *) &serverAdd, sizeof(serverAdd)) < 0) {
        printf("Binding Error\n");
   }

   listen(sockFd,5); // Arg 2: Number of Clients
   clientLen = sizeof(clientAdd);

   clientSocketFd = accept(sockFd, (struct sockaddr *) &clientAdd, &clientLen);

   if(clientSocketFd < 0) {
       printf("Error in creating Accept\n");
   }

   bzero(buffer,255);

   n = read(clientSocketFd,buffer,255);

   if(n < 0) {
        printf("Error reading from socket\n");
   }
   else {
        printf("Message: %s\n",buffer);
   }

   n = write(clientSocketFd,"Received message",16);

   if(n < 0) {
        printf("Error writing to socket\n");
   }

   close(clientSocketFd);

   return 0;
}
