#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFSIZE 1024
#define MAX_CONN_BACKLOG 10

int main(int argc, char** argv)
{
   int sockfd, newsockfd;
   struct sockaddr_in servaddr;
   struct sockaddr_in cliaddr;
   socklen_t clilen;
   char msg[BUFFSIZE];
   int msg_int;
   int counter;

   if (argc != 2) {
      printf("usage: server <port>\n");
      exit(1);
   }
   memset(&servaddr, 0, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_port = htons(atoi(argv[1]));
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

   sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (sockfd < 0) {
      printf("Socket opening error");
      exit(1);
   }

   if (bind(sockfd, (struct sockaddr *) &servaddr,
        sizeof(servaddr)) < 0) {
      printf("Binding error");
      exit(1);
   }

   listen(sockfd, MAX_CONN_BACKLOG);
   printf("waiting for connections...\n");

   counter = 0;
   for (;;) {
      clilen = sizeof(cliaddr);
      newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, 
        &clilen);
      if (newsockfd < 0) {
         printf("ERROR on accept\n");
         continue;
      }

      memset(msg, 0, BUFFSIZE);
      recv(newsockfd, msg, BUFFSIZE, 0);

      if (sscanf(msg, "%i", &msg_int)) { 
         counter += msg_int;
         if (msg_int == 0) 
            counter = 0;
      }

      sprintf(msg, "%i", counter);
      send(newsockfd, msg, strlen(msg), 0);

      close(newsockfd);
   }

   close(sockfd);
   exit(0);
}
