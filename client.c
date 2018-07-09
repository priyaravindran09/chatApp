#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
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
   signal(SIGINT,sigintHandler);    //To catch Ctr+C

   int sockFd, portNo;
   struct sockaddr_in serverAdd;
   int n;
   struct hostent *server;  //struct containing server details
   int bye,byebye;

   char txBuffer[256], rxBuffer[256];   //Rx Tx Buffers

   if(argc < 3) {
        printf("Argument Error\n");
        exit(1);
   }

   portNo = atoi(argv[2]);  //Fetch Port Number

   //Creating a socket and fetching its file descriptor
   sockFd = socket(AF_INET, SOCK_STREAM, 0); //Arg 1: using IP Arg 2: using TCP Arg 3: provide appropriate socket
   if(sockFd < 0) {
        printf("Error Opening Socket\n");
        exit(0);
   }

   server = gethostbyname(argv[1]); //pass the name to get the address of the server

    if(server == NULL) {
        printf("No such host exists\n");
        exit(0);
    }

   bzero((char *) &serverAdd, sizeof(serverAdd));

   serverAdd.sin_family = AF_INET;  //Internet type
   bcopy((char *)server->h_addr,(char *)&serverAdd.sin_addr.s_addr,server->h_length);   //serverr addr
   serverAdd.sin_port = htons(portNo); //convert values between host and network 

   //request connection to the server
   if(connect(sockFd, (struct sockaddr *) &serverAdd, (socklen_t) sizeof(serverAdd)) < 0) {
        printf("Connection Error\n");
        close(sockFd);
        exit(0);
   }
   else {
       printf("Connected to %s\n",inet_ntoa(serverAdd.sin_addr));
   }


   do {

        printf("Enter message: ");
        bzero(txBuffer,255);
        fgets(txBuffer,255,stdin);
        bye = strncasecmp(txBuffer,"bye",3);
        n = send(sockFd,txBuffer,strlen(txBuffer),0);
        if(n < 0) {
            printf("Error writing to socket\n");
            break;
        }

        if(bye == 0)
            break;
      
        bzero(rxBuffer,256);
        n = recv(sockFd,rxBuffer,255,0);
        if(n < 0) {
            printf("Error reading from socket\n");
            break;
        }
        else {
            printf("Received message: %s\n",rxBuffer);    
        }
        byebye =  strncasecmp(rxBuffer,"bye",3);
        printf("byebye: %d\n",byebye);
        if(byebye == 0)
        break;

   } while(1);

   printf("Closing socket\n");
   close(sockFd);

   return 0;
}
