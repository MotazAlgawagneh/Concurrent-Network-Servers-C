#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#define MAXLINE 100
#define LISTENQ 30
#define PORT 9000

void str_echo(int connfd){

ssize_t n;
char buffer[MAXLINE];
char filtered[100];
int newlen;
int ispal;

for(;;){
newlen=0;
ispal=1;

    
n=read(connfd,buffer,sizeof(buffer));

if(n<=0)
break;

buffer[n]='\0';



for(int i=0;i<n;i++){
if(buffer[i] != ' '){
filtered[newlen]=tolower(buffer[i]);
newlen++;}}

filtered[newlen]='\0';


for(int i=0; i<newlen/2;i++){
if(filtered[i]!=filtered[newlen-i-1]){
ispal=0;
break;}}


if(ispal){
write(connfd,"Palindrome\n",strlen("Palindrome\n"));}
else{
write(connfd,"Not Palindrome\n",strlen("Not Palindrome\n"));}}}





int main(){

int listenfd, connfd,status;
struct sockaddr_in servaddr,clientaddr;
socklen_t clilen;
pid_t childpid;

listenfd=socket(AF_INET,SOCK_STREAM,0);

    
if(listenfd<0){
perror("Socket Error"); 
return -1; 
}


printf("Listening Socket Created\n");
bzero(&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
servaddr.sin_port=htons(PORT);


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
    
for(;;){
clilen=sizeof(clientaddr);
connfd = accept(listenfd,(struct sockaddr *)&clientaddr,&clilen);
if(connfd<0){
perror("Cannot create connection socket"); 
}
if((childpid=fork())==0){
close(listenfd);
str_echo(connfd);
close(connfd);
exit(0);
}

close(connfd);}
return 0;
}