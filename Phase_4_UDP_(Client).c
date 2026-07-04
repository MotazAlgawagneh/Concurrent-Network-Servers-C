#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#define SERV_PORT 9000
#define MAXLINE 101






void str_cli(FILE *fp, int sockfd, struct sockaddr_in servaddr){

char sendline[MAXLINE];
char recvline[MAXLINE];
char choice[10];
int length, newlength, haserror;

while(1){
    
    
printf("\nplease enter a word : ");
fflush(stdout);

if(fgets(sendline,MAXLINE,fp)==NULL){
printf("\nEOF detected. Closing connection.\n");
return;}


length=strlen(sendline);
if(length>0 && sendline[length-1]=='\n'){
sendline[--length]='\0';}

if(length==0){
printf("Error ❌ :no input entered.\n");
continue;}


if(length>=100){
printf("warning ⚠️ :input is too long. Max is 100 characters.\n");
while(getchar()!='\n');
continue;}


haserror=0;
for(int i=0;i<length;i++){
if(!isalpha(sendline[i]) && !isspace(sendline[i])){
printf("Error ❌ :invalid character '%c'. Letters only.\n", sendline[i]);
haserror=1;
break;
}}
if(haserror) 
continue;

newlength=0;
for(int i=0;i<length;i++){
if(sendline[i] != ' ') 
newlength++;
}
if(newlength==0){
printf("Error ❌ : input contains only spaces.\n");
continue;
}


sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
bzero(recvline,sizeof(recvline));
recvfrom(sockfd,recvline,MAXLINE-1,0,NULL,NULL);
printf("%s",recvline);

while(1){
    printf("\nNote: If you want to exit, press 'n' or 'N' ,if no press Enter\n");
    printf("Do you want to try again? 🔁");
    if(fgets(choice,sizeof(choice),fp)==NULL){
    printf("\nExiting...\n");
    return;
    }
    if(strcmp(choice,"\n") == 0){
    break;
    }else 
    if(strcmp(choice,"n\n")==0 || strcmp(choice,"N\n")==0){
    printf("OK 😢,Goodbye!\n");
    return;
    }else{
    printf("Error ❌: invalid input. Try again.\n");
            }}}}

















int main(int argc, char **argv){

int sockfd;
struct sockaddr_in servaddr;


if(argc!=2){
printf("usage: %s <IPaddress>\n",argv[0]);
exit(1);
}



sockfd=socket(AF_INET,SOCK_DGRAM,0);
if(sockfd<0){
perror("socket error");
exit(1);
}




bzero(&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_port=htons(SERV_PORT);
if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<=0){
printf("inet_pton error for %s\n",argv[1]);
exit(1);
} 



printf("UDP Client ready. Server port: %d\n",ntohs(SERV_PORT));

str_cli(stdin,sockfd,servaddr);


return 0;
}