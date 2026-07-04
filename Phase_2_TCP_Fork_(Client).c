#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINE 100
#define SERV_PORT 9000

void str_cli(FILE *fp,int sockfd);




int main(int argc,char **argv){
int sockfd;
struct sockaddr_in servaddr;
    
if(argc != 2) {
printf("usage: %s <IPaddress>\n", argv[0]);
exit(1);
}
    
   
if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
perror("Cannot create socket");
exit(1);
}
    
   
bzero(&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_port=htons(SERV_PORT);
    
    
if(inet_pton(AF_INET, argv[1],&servaddr.sin_addr)<=0){
printf("inet_pton error for %s\n",argv[1]);
exit(1);
}
    
    
if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
perror("Cannot connect to Server");
exit(1);
}
    
printf("Connected to server on port %d\n", SERV_PORT);
    
str_cli(stdin, sockfd);
exit(0);
}








void str_cli(FILE *fp,int sockfd){
char sendline[MAXLINE+1];
char recvline[MAXLINE+1];
int length,newlength,haserror;
    
while(1){
printf("\nPlease enter a word: ");

if(fgets(sendline,MAXLINE+1,fp)==NULL){
printf("\nEOF detected. Closing connection.\n");
break;}
        
        
length=strlen(sendline);
if(length>0 &&sendline[length-1]=='\n') {
sendline[length-1]='\0';
length--;
}
        
  
       
if(length==0){
printf("Error: No input entered.\n");
continue;
}
        
        
if(length>=MAXLINE){
printf("Warning : Input is too long. Max is %d characters.\n",MAXLINE);
int c;
while((c=getchar())!='\n' && c!=EOF);
continue;
}
        

haserror=0;
for(int i=0;i<length;i++){
if(!isalpha(sendline[i])&&!isspace(sendline[i])){
printf("Error: Invalid character '%c'. Letters and spaces only.\n",sendline[i]);
haserror=1;
break;}}

if(haserror)
continue;

      
newlength=0;
for(int i=0;i<length;i++){
    
if(sendline[i] != ' '){
newlength++;}}


if(newlength==0){
printf("Error: Input contains only spaces.\n");
continue;}
        

if(write(sockfd,sendline,strlen(sendline)+1)<0){
perror("Write error");
break;
}
        

bzero(&recvline,sizeof(recvline));
if (read(sockfd, recvline, MAXLINE) <= 0){
printf("Server terminated prematurely\n");
break;}
        

printf("%s",recvline);
        

char choice[10];
while (1){
printf("\nNote: If you want to exit, press 'n' or 'N', if no press Enter\n");
printf("Do you want to try again? ");

if (fgets(choice,sizeof(choice),fp)==NULL){
printf("\nExiting...\n");
return;}
            

if (strcmp(choice,"\n")==0){
break;
}else
if(strcmp(choice,"n\n")==0 || strcmp(choice,"N\n")==0){
printf("OK, Goodbye!\n");
return;}else{
printf("Error: Invalid input. Try again.\n");}
}
}
}