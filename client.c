#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
   int sockFd, portNo;
   struct sockaddr_in serverAdd;
   int n;
   struct hostent *server;

   char buffer[256];

   if(argc < 3) {
        printf("Argument Error\n");
        exit(1);
   }
   portNo = atoi(argv[2]);

   sockFd = socket(AF_INET, SOCK_STREAM, 0); //Arg 1: using IP Arg 2: using TCP Arg 3: provide appropriate socket
   if(sockFd < 0) {
        printf("Error Opening Socket\n");
   }

    server = gethostbyname(argv[1]);

    if(server == NULL) {
        printf("No such host exists\n");
        exit(0);
    }

   bzero((char *) &serverAdd, sizeof(serverAdd));

   serverAdd.sin_family = AF_INET;
   bcopy((char *)server->h_addr,(char *)&serverAdd.sin_addr.s_addr,server->h_length);
   serverAdd.sin_port = htons(portNo); //convert values between host and network 

   if(connect(sockFd, &serverAdd, sizeof(serverAdd)) < 0) {
        printf("Connection Error\n");
   }

   printf("Enter the message\n");
   bzero(buffer,255);
   fgets(buffer,255,stdin);
   n = write(sockFd,buffer,strlen(buffer));

   if(n < 0) {
        printf("Error writing to socket\n");
   }
   bzero(buffer,256);

   n = read(sockFd,buffer,255);

   if(n < 0) {
        printf("Error reading from socket\n");
   }
   else {
        printf("%s\n",buffer);    
   }

   return 0;
}
