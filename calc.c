//Elkhan Ibrahimov: 251ADB061
//
// Compile with: gcc -O2 -Wall -Wextra -std=c17 -o calc calc.c -lm
/*Resources used: ChatGPT for explanations
https://man7.org/linux/man-pages/man2/mkdir.2.html?utm_source
https://man7.org/linux/man-pages/man3/dirname.3.html?utm_source
https://www.cppreference.com/index.php?title=c/io/fprintf&utm_source
https://www.cppreference.com/c/string/byte/strrchr?utm_source
https://beej.us/guide/bgc/html/split/strings.html?utm_source
https://en.cppreference.com/c/string/byte/isdigit?utm_source
https://en.cppreference.com/c/string/byte/isspace?utm_source
https://ruslanspivak.com/lsbasi-part5/?utm_source
https://github.com/WarpRules/recursive_descent_parser_tutorial?utm_source
https://en.cppreference.com/c/language/operator_arithmetic?utm_source
https://craftinginterpreters.com/parsing-expressions.html?utm_source
https://www.youtube.com/watch?v=SToUyjAsaFk
https://www.youtube.com/watch?v=NdW_ApiaivU
https://www.youtube.com/watch?v=N55XNj8KjC4
https://www.youtube.com/watch?v=rAZpPN1Q4so
https://stackoverflow.com/questions/30991941/how-to-ignore-unneccessary-comment-line-from-the-input-file-in-c?utm_source
https://introcs-python.cs.luc.edu/recursion/recursiondparser.html?utm_source
https://eli.thegreenplace.net/2009/03/14/some-problems-of-recursive-descent-parsers?utm_source
https://www.cs.fsu.edu/~lacher/courses/COP4020/fall15/assigns/proj2b.html?utm_source*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

void spaceIgnore(const char *text,int *e){
   while(text[*e]==' '||text[*e] =='\t') {
      (*e)++ ;
    }
}
long long expressionCal(const char *text,int *e,int *err,long long *errPos,long long startPos);
long long termCal(const char *text,int *e,int *err,long long *errPos,long long startPos);
long long valueCal(const char *text,int *e,int *err,long long *errPos,long long startPos);


/* AI (ChatGPT): prompt "listen For the ** part from the task I did parentheses
and +, -, *, / with precedence working as u know. Now i have problem 
with how to make ** right-associative so 2 ** 3 ** 2 becomes 512 and how
to report a negative integer exponent at the start of the exponent.
Can you explain the idea or give me a small example or hint or useful 
information website or vidieo which u didnt gave.
Can you explain the idea or give me a small example or hint, not the full solution?"
From exp I understand that  power should call itself for the
exponent so 2 ** 3 ** 2 is calculated as 2 ** (3 ** 2).
Link: <https://chatgpt.com/share/6adfc1e8-682c-83ed-9f74-6c9eaca39642> */
long long powerCal(const char *text,int *e,int *err,long long *errPos,long long startPos){
  long long base=valueCal(text,e,err,errPos,startPos);
    if (*err!= 0) {
        return 0 ;
    }
spaceIgnore ( text,e);
   if(text[*e]== '*' && text[*e+1] =='*') {
       (*e) +=2;
        spaceIgnore(text,e);
long long exponentPos = startPos +(*e) ;
   long long exponent = powerCal (text,e,err,errPos,startPos);
       if (*err!= 0) {
            return 0 ;
  }
    if (exponent <0) {
     *errPos = exponentPos ;
        *err = 1;
           return 0;
   }
long long result = 1;
  for(long long i = 0; i < exponent; i++) {
       result*= base ;
   }
   return result;
    }
    return base;
}
/* AI (ChatGPT): prompt "For the parentheses part from the task 
I already have +, -, * and / i am working with precedence.
 I am struggling with how to add parentheses,including nested parentheses 
 and how to report missing or extra ). Can you explain the idea or give me 
 a small example/hint,not the full solution?" and I asked for code example

I used the explanation to understand that expression inside parentheses
can parsed again until matching ')' is found.
Link: <https://chatgpt.com/share/6adf7ec3-a708-83eb-b368-64933d3453b8> */

long long valueCal(const char *text,int *e, int *err,long long *errPos,long long startPos  ){
  spaceIgnore(text,e);
    long long number= 0;
     if(text[*e]==  '('){
        (*e)++;
   number =expressionCal(text,e,err,errPos,startPos);
     if(*err!= 0){
            return 0;
          }
 spaceIgnore (text,e);
    if(text[*e]!=')') {
       *errPos = startPos+(*e);
         *err = 1;
            return 0;
          }
          (*e)++;
        }
    else if(isdigit((unsigned char )text[*e])){
   while(isdigit((unsigned char)text[*e])) {
     number=number * 10+(text[*e] -'0');
     (*e)++;
      }
    }else {
      *errPos = startPos+(*e);
   *err= 1;
     return 0;
    }
    return number ;
}
long long termCal(const char *text,int *e,int *err,long long *errPos,long long startPos ){
   long long value = powerCal (text,e,err,errPos,startPos);
    if(*err != 0) {
        return 0;
  }
   while(1) {
    spaceIgnore(text, e );
        if(text[*e]!= '*'&&text[*e]!='/') {
            break;
        }
  char operation =text[*e] ;
    (*e )++;
     spaceIgnore (text,e);
      long long rightPos = startPos +(*e);
        long long next =powerCal(text,e,err,errPos,startPos);
        if(*err!=0){
      return 0; }
   if (operation=='/' && next==0) {
     *errPos = rightPos;
        *err= 1;
         return 0;
          }
   if(operation =='*') {
     value*=next;
        } else {
     value /=next;
        }
    }
    return value;
}
long long expressionCal(const char *text,int *e,int *err,long long *errPos,long long startPos ){
      long long total=termCal(text,e,err,errPos,startPos);
  if(*err != 0) {
        return 0;
  }
   while(1) {
    spaceIgnore(text, e );
        if(text[*e]!= '+'&&text[*e]!='-') {
            break;
        }
  char operation =text[*e] ;
    (*e )++;
     spaceIgnore (text,e);
       long long next= termCal(text,e,err,errPos,startPos);
        if(*err !=0){
            return 0;
        }
   if(operation =='+') {
     total +=next;
        } else {
     total -=next;
        }
    }
    return total;
}

int main(int argc,char *argv[]){
    if (argc !=4){
     fprintf(stderr,"Invalid arguments\n" );
     return 1 ;
    }
    if(strcmp(argv[1],"-o") !=0){
     fprintf(stderr,"Invalid option\n") ;
     return 1;
    }
    /* AI (ChatGPT):prompt "I finished the part where I check argc and use
 strcmp(argv[1], "-o").I understand argv[2] should be the output folder
 and argv[3] the input file. Am I doing it correctly and what should I do next?"
 I used the guidance to get the output directory and input filename from argv
 link:https://chatgpt.com/share/6ade861c-e638-83eb-a33a-afd501f931d3*/
 const char *output_dir=argv[2];//For that part
  const char *input_file= argv [3];
   FILE *input=fopen(input_file,"r");// and for that one
    if(input== NULL){
     fprintf(stderr,"Cannot open input file\n");
       return 1;
    }

 if (mkdir(output_dir,0777) !=0 && errno !=EEXIST) {
    /*https://man7.org/linux/man-pages/man2/mkdir.2.html?utm_source
    that open resource was used for 
    understanding part with 0777 */
  fprintf(stderr,"Can't create output dir\n");
   fclose (input);
    return 1;
 }
 /*https://man7.org/linux/man-pages/man3/dirname.3.html?utm_source
https://www.cppreference.com/index.php?title=c/io/fprintf&utm_source
https://www.cppreference.com/c/string/byte/strrchr?utm_source
these 3 open sources were used for understanding and writing 
lower part of the code until 
fprintf( stderr,"Can't open output file\n") ;*/
const char *file_name=strrchr(input_file,'/');
if(file_name==NULL){
    file_name =input_file;
}else{
 file_name++;
}
char name[256];
snprintf(name,sizeof(name),"%s",file_name);
char *extension =strrchr(name,'.') ;
if(extension !=NULL){
    *extension= '\0' ;
}
char output[512] ;
snprintf(output,sizeof(output),
  "%s/%s_elkhan_ibrahimov_251adb061.txt",
      output_dir, name);
FILE *result =fopen(output,"w");
if(result == NULL){
fprintf( stderr,"Can't open output file\n") ;
 fclose (input);
  return 1;
}
/* AI (ChatGPT): prompt "I didn't get again, is there something to do for that part?"
   ChatGPT gave me a hint to research fgets().
   I used the hint and the resource to understand how to read each expression
   from the input file.
   link:https://chatgpt.com/share/6ade861c-e638-83eb-a33a-afd501f931d3 */
char text[256] ;
long long pos = 1;
 while(fgets(text,sizeof(text),input) !=NULL){
long long startPos = pos;


  /*https://beej.us/guide/bgc/html/split/strings.html?utm_source
https://en.cppreference.com/c/string/byte/isdigit?utm_source
https://en.cppreference.com/c/string/byte/isspace?utm_source
these sources was used for lower part until fprintf(result, "%lld\n", total);*/
int e = 0;
int err= 0;
long long errPos =0;
spaceIgnore(text,&e) ;
if(text[e]=='#'){
    pos += strlen(text);
    continue;
}

if(text[e] == '\n' || text[e]=='\0') {
    pos +=strlen(text);
    continue;
}
long long total=expressionCal(text,&e,&err,&errPos,startPos);
if(err == 0) {
    spaceIgnore(text,&e) ;
    if(text[e]!='\n'&&text[e]!='\0') {
  errPos=startPos+e;
     err= 1 ;
    }
}
#if 0 
when I learned about #if 0 and #endif I thought I will never use it :/

/* AI (ChatGPT): prompt "For the operator precedence part from the task,
can you give me  good open resource where I can understand how 
to handle * and / before + and - in a simple C parser? and 
can u explain that part  and with examples not the full code."
  I used the hint from AI to understand using separate term for * and / so they
   are calculated before adding or subtracting from the total
   Link: https://chatgpt.com/share/6adf6d47-b224-83eb-9c6a-ad4ce79860c0 */
long long total=0;
long long term=number;
char sign ='+';
if (err==0){
while(text[e] !='\0' && text[e] !='\n'){
    while(text[e]==' '|| text[e]=='\t'){
        e++;
   }
 if(text[e]== '\0'||text[e] =='\n') {
    break ;
  }
  if(text[e] !='+' && text[e] !='-' && text[e] !='*' && text[e]!= '/') {
   errPos=startPos+e;
    err = 1;
    break ;
}
char operation=text[e] ;
   e++;
  while(text[e] ==' '|| text[e]=='\t'){
      e++;
    }
 if (text[e] == '\n' ||text[e]=='\0') {
  errPos = startPos+e;
    err=1 ;
     break ;
}
if(!isdigit((unsigned char)text[e])){
    errPos= startPos + e;
    err=1 ; 
    break;
}

    /*AI (ChatGPT): prompt:i did until there but i am struggling 
    with part after can u help me a little bit.I used ChatGPT's example to understand how to read the next number
   and update the total for + and - operations
   link:https://chatgpt.com/share/6ade861c-e638-83eb-a33a-afd501f931d3*/
   // I saved the start position of the right operand so division by zero
   // and it reports the correct error position
   long long nextPos=startPos+e;
   long long nextNumber =0;
    while(isdigit((unsigned char)text[e])) {
      nextNumber= nextNumber*10+(text[e]-'0');
        e++;
  }
if(operation=='/' &&nextNumber== 0){
 errPos=nextPos;
  err =1;
    break;
}
 if(operation=='*') {
    term *=nextNumber;
}else if(operation=='/' ){
    term /=nextNumber;
} else{
  if (sign=='+' ) {
     total += term;
 } else if(sign=='-') {
        total -=term;
    }
    sign=operation;
    term =nextNumber ;
    }
  }
}
if(err ==0){
 if(sign=='+') {
    total +=term ;
}else if(sign =='-'){
        total -=term ;
    }
}
    #endif
 if(err !=0){
  fclose (result);
   result=fopen(output,"w") ;
     if(result ==NULL){
  fprintf(stderr,"Can't open output file\n");
      fclose( input );
       return 1; }
   fprintf(result,"ERROR:%lld\n",errPos);
      break;
 }
 fprintf(result, "%lld\n",total);
    pos += strlen(text);
}
fclose (input);
  fclose (result);
    return 0;
}