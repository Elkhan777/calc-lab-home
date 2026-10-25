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
https://en.cppreference.com/c/string/byte/isspace?utm_source*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

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
 while(fgets(text,sizeof(text),input) !=NULL){
  /*https://beej.us/guide/bgc/html/split/strings.html?utm_source
https://en.cppreference.com/c/string/byte/isdigit?utm_source
https://en.cppreference.com/c/string/byte/isspace?utm_source
these sources was used for lower part until fprintf(result, "%lld\n", total);*/
int e=0;
long long number=0;
while (text[e]==' ' || text[e]== '\t'){
  e++;
}
while (isdigit((unsigned char)text[e]) ){
   number = number*10+(text[e]-'0');
    e++;
}
long long total=number;
while(text[e] !='\0' && text[e] !='\n'){
    while(text[e]==' ' || text[e]=='\t'){
        e++;
   }
 if(text[e] == '\0' || text[e] == '\n') {
    break ;
  }
  char operation=text[e] ;
    e++;
  while(text[e] ==' ' || text[e]=='\t'){
      e++;
    }

    /*AI (ChatGPT): prompt:i did until there but i am struggling 
    with part after can u help me a little bit.I used ChatGPT's example to understand how to read the next number
   and update the total for + and - operations
   link:https://chatgpt.com/share/6ade861c-e638-83eb-a33a-afd501f931d3*/
   long long next_number =0;
    while(isdigit((unsigned char)text[e])) {
      next_number = next_number * 10 + (text[e] - '0');
        e++;
  }
    if (operation =='+'){
      total += next_number;
    } else if (operation=='-'){
       total -=next_number;
    }
}
fprintf(result, "%lld\n", total);
}
  fclose (input);
  fclose (result);
    return 0;
}