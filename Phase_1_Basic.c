#include <stdio.h>
#include <string.h>
#include <ctype.h>
int main()
{
char word[101];
char filtered[101];
char choice[101];

while(1){
int length;
int newlength=0;
int ispalindrome=1;

printf("please enter a word : ");
fgets(word,101,stdin);
length=strlen(word);
if(length>0 && word[length-1]=='\n'){
word[length-1]='\0';
length--;
}



//ادخال فارغ
if(length ==0){
printf("Error ❌ :no input entered.\n");
continue;
}



//ادخال اكثر من 100 حرف
if(length>=100){
printf("warning ⚠️ :input is too long.Max is 100 characters.\n");
while(getchar() != '\n');
continue;
}


//ادخال رموز أو حروف
int haserror=0;
for(int i=0;i<length;i++){
if(!isalpha(word[i]) && !isspace(word[i])){
printf("Error ❌ :invalid character '%c'.Letters only.\n", word[i]);
haserror=1;
break;
}
}
if(haserror)
continue;


//فراغات وحروف كبيرة وصغيرة بالأدخال
for(int i=0;i<length;i++){
if(word[i] != ' '){
filtered[newlength]=tolower(word[i]);
newlength++;
}
}
filtered[newlength]='\0';

if(newlength == 0){
    printf("Error ❌ : input contains only spaces.\n");
    continue;
}


for(int i=0;i<newlength/2;i++){
if(filtered[i] != filtered[newlength-i-1]){
ispalindrome=0;
break;
}
}
if(ispalindrome == 1){
printf("This word is palindrome ✅\n");
}else{
printf("This word is not palindrome ❌\n");
}




while(1){
printf("\nNote: If you want to exit, press 'n' or 'N' ,if no press Enter\n");
printf("Do you want to try again? 🔁");
fgets(choice,10,stdin);
if(strcmp(choice, "\n") == 0){
break;}
else
if( strcmp(choice, "n\n") == 0 || strcmp(choice, "N\n") == 0){
printf("OK 😢,Goodbye!\n");
return 0;
}
else{
printf("Error ❌: invalid input. Try again.\n");

}
}
}
return 0;
}
