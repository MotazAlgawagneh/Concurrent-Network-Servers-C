#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#define SERV_PORT 9000
#define MAXLINE 101
#define max(a,b) ((a) > (b) ? (a) : (b))





void str_cli(FILE *fp,int connfd){

int Maxfdp1, x;
fd_set rset;
char sendline[MAXLINE];
char recvline[MAXLINE];
char choice[10];
int length;
int newlength;
int haserror;


FD_ZERO(&rset);


printf("please enter a word : ");
fflush(stdout);

for(;;){

FD_SET(fileno(fp),&rset);
FD_SET(connfd,&rset);
Maxfdp1=max(fileno(fp),connfd)+ 1;

x=select(Maxfdp1,&rset,NULL,NULL,NULL);

if(x==0){
printf("Timeout of select expired\n");
continue;
}

else 
if(x==-1){
perror("Error in select");
exit(1);
}




if(FD_ISSET(connfd,&rset)){


if(read(connfd,recvline,MAXLINE) == 0){
perror("Server terminated prematurely");
return;
}

fputs(recvline, stdout);


while(1){
printf("\nNote: If you want to exit, press 'n' or 'N' ,if no press Enter\n");
printf("Do you want to try again? 🔁");
fgets(choice, 10, stdin);
if(strcmp(choice, "\n") == 0){
printf("please enter a word : ");
fflush(stdout);
break;
}else 
if(strcmp(choice, "n\n") == 0 || strcmp(choice, "N\n") == 0){
printf("OK 😢,Goodbye!\n");
close(connfd);
return;}
else{
printf("Error ❌: invalid input. Try again.\n");
}
}
}




if(FD_ISSET(fileno(fp),&rset)){

if(fgets(sendline,MAXLINE,fp)==NULL)
return;

length=strlen(sendline);
if(length > 0 &&sendline[length-1]=='\n'){
sendline[length-1]='\0';
length--;
}


//ادخال فارغ
if(length==0){
printf("Error ❌ :no input entered.\n");
printf("please enter a word : ");
fflush(stdout);
continue;
}

//ادخال اكثر من 100 حرف
if(length >= 100){
printf("warning ⚠️ :input is too long. Max is 100 characters.\n");
while(getchar() != '\n');
printf("please enter a word : ");
fflush(stdout);
continue;
}



//ادخال رموز او ارقام
haserror=0;
for(int i=0; i<length;i++){
if(!isalpha(sendline[i]) && !isspace(sendline[i])){
printf("Error ❌ :invalid character '%c'. Letters only.\n", sendline[i]);
haserror=1;
break;
}
}
if(haserror){
printf("please enter a word : ");
fflush(stdout);
continue;
}




//ادخال فراغات فقط
newlength=0;
for(int i=0; i<length;i++){
if(sendline[i] != ' ')
newlength++;
}
if(newlength==0){
printf("Error ❌ : input contains only spaces.\n");
printf("please enter a word : ");
fflush(stdout);
continue;
}


write(connfd, sendline, strlen(sendline));

}}}

















int main(int argc, char **argv){

int sockfd;
struct sockaddr_in servaddr;


if(argc!=2){
printf("usage: %s <IPaddress>\n",argv[0]);
exit(1);
}



sockfd=socket(AF_INET,SOCK_STREAM,0);
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



if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
perror("Cannot connect to Server");
exit(1);
}



printf("Connected to server on port %d\n",ntohs(SERV_PORT));




str_cli(stdin,sockfd);

close(sockfd);
return 0;
}