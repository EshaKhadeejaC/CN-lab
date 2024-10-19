#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>


void main()
{
  int client;
  struct sockaddr_in servAddr;
  char servMsg[2000],cliMsg[2000];
  int server_struct_length=sizeof(servAddr);
  client=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  if(client<0)
  {
    printf("Error whle creating socket\n");
    exit(1);
  }
  printf("socket succesfully created\n");
  servAddr.sin_family=AF_INET;
  servAddr.sin_port=htons(2002);
  servAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
  printf("enter the message to server:");
  gets(cliMsg);
  if(sendto(client,cliMsg,strlen(cliMsg),0,(struct sockaddr*)&servAddr,server_struct_length)<0)
  {
   printf("Unable to send message\n");
   exit(1);
  }
  if(recvfrom(client,servMsg,sizeof(servMsg),0,(struct sockaddr*)&servAddr,&server_struct_length)<0)
  {
   printf("error while reciving server's message\n");
   exit(1);
  }
  close(client);
 }
  
