#include <sys/select.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>
#define MAXLINE 101
#define LISTENQ 30
#define SERV_PORT 9000


void handle_connection(int sockfd, char *buffer, int n){

char filtered[MAXLINE];
int newlen=0;
int ispal=1;
buffer[n]='\0';

for(int i=0;i<n;i++){
    
if(buffer[i]!=' '){
filtered[newlen]=tolower(buffer[i]);
newlen++;
}}

filtered[newlen]='\0';

for(int i=0;i< newlen/2;i++){
if(filtered[i]!=filtered[newlen-i-1]){
ispal=0;
break;}}

if(ispal)
write(sockfd, "Palindrome\n", strlen("Palindrome\n"));
else
write(sockfd, "Not Palindrome\n", strlen("Not Palindrome\n"));
}





int main(){

int listenfd, connfd, sockfd, status;
int i, maxi, maxfd;
int nready, client[FD_SETSIZE];
ssize_t n;
fd_set rset, allset;
char buffer[MAXLINE];
struct sockaddr_in servaddr, clientaddr;
socklen_t clilen;

listenfd=socket(AF_INET,SOCK_STREAM,0);

    
if(listenfd<0){
perror("Socket Error"); 
return -1; 
}


printf("Listening Socket Created\n");
bzero(&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
servaddr.sin_port=htons(SERV_PORT);


status=bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
if(status<0){
perror("Binding Error"); 
return -2; 
}

    
status=listen(listenfd,LISTENQ);
if(status<0){
perror("Listening Error"); 
return -3; 
}

printf("Server running...\n");
printf("Connect using: ./client 127.0.0.1\n");  



maxfd=listenfd;
maxi=-1;

for(i=0;i<FD_SETSIZE;i++){
client[i]=-1;}


FD_ZERO(&allset);
FD_SET(listenfd,&allset);
  
    
for(;;){
rset=allset;
nready=select(maxfd+1,&rset,NULL,NULL,NULL);

if(FD_ISSET(listenfd,&rset)){
clilen=sizeof(clientaddr);
connfd=accept(listenfd,(struct sockaddr *)&clientaddr,&clilen);

if(connfd<0)
perror("Cannot create connection socket");

for(i=0;i<FD_SETSIZE;i++){
if(client[i]<0){
client[i]=connfd; 
break;
}}

if(i==FD_SETSIZE)
perror("Too many clients\n");

FD_SET(connfd,&allset);
if(connfd>maxfd)
maxfd=connfd;       
if(i>maxi)
maxi=i;         

if(--nready<=0)
continue;              
}

for(i=0;i<=maxi;i++){
if((sockfd=client[i])<0)
continue;

if(FD_ISSET(sockfd,&rset)){
if((n=read(sockfd,buffer,MAXLINE))==0){
close(sockfd);
FD_CLR(sockfd,&allset);
client[i]=-1;}
else{
 handle_connection(sockfd, buffer, n);}

if(--nready <= 0)
break;
}}}
return 0;
}