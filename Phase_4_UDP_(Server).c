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
#define SERV_PORT 9000


void handle_connection(int sockfd, char *buffer, int n, struct sockaddr_in clientaddr, socklen_t clilen){

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
sendto(sockfd,"Palindrome\n",strlen("Palindrome\n"),0,(struct sockaddr *)&clientaddr,clilen);
else
sendto(sockfd,"Not Palindrome\n",strlen("Not Palindrome\n"),0,(struct sockaddr *)&clientaddr,clilen);
}







int main(){

int listenfd, status;
ssize_t n;

char buffer[MAXLINE];
struct sockaddr_in servaddr, clientaddr;
socklen_t clilen;

listenfd=socket(AF_INET,SOCK_DGRAM,0);

    
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

    


printf("Server running...\n");
printf("Connect using: ./client 127.0.0.1\n");  






  
    
for(;;){
    clilen = sizeof(clientaddr);
    n=recvfrom(listenfd,buffer,MAXLINE,0,(struct sockaddr *)&clientaddr,&clilen);
    if(n < 0){
    perror("recvfrom error");
    continue;
    }
    handle_connection(listenfd, buffer, n, clientaddr, clilen);
}
return 0;
}