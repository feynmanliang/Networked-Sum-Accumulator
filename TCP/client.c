#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define BUFFSIZE 1024
#define TIMEOUT_SECS 5
#define MAXTRIES 3

void catch_alarm(int);
int num_tries = 0;

int main(int argc, char** argv)
{
   int sockfd;
   struct sockaddr_in servaddr;
   char sendmsg[BUFFSIZE];
   char rcvmsg[BUFFSIZE];
   struct sigaction alarm_action;
   int conn;

   if (argc != 3) {
      printf("usage: client <ip> <port>\n");
      exit(1);
   }
   memset(&servaddr, 0, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = inet_addr(argv[1]);
   servaddr.sin_port = htons(atoi(argv[2]));

   sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (sockfd < 0) {
      printf("Socket opening error\n");
      exit(1);
   }

   alarm_action.sa_handler = catch_alarm;
   sigfillset(&alarm_action.sa_mask); /*block all signals during signal-catching*/
   alarm_action.sa_flags=0;
   sigaction(SIGALRM, &alarm_action, 0);

   conn = connect(sockfd, (struct sockaddr*) &servaddr, 
     sizeof(servaddr));
   if (conn < 0) {
      printf("Connection error\n");
      exit(1);
   }

   memset(sendmsg, 0, BUFFSIZE);
   fgets(sendmsg, BUFFSIZE, stdin);
   send(sockfd, sendmsg, strlen(sendmsg), 0);

   alarm(TIMEOUT_SECS);
   while ((recv(sockfd, rcvmsg, BUFFSIZE, 0)) < 0) {
      if (num_tries < MAXTRIES) {
         printf("timeout, %d tries left\n", MAXTRIES - num_tries);
         send(sockfd, sendmsg, strlen(sendmsg), 0);
         alarm(TIMEOUT_SECS);
      }
      else {
         printf("Connection timed out\n");
         close(sockfd);
         exit(1);
      }
   }

   printf("Msg from server: %s\n", rcvmsg);

   memset(sendmsg, 0, BUFFSIZE);
   memset(rcvmsg, 0, BUFFSIZE);
   exit(0);
}

void catch_alarm(int signal)
{
   num_tries += 1;
   if (num_tries < MAXTRIES) {
   }
}
