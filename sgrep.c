/* 20190617 조동올, assignment 2, sgrep.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for skeleton code */
#include <unistd.h> /* for getopt */
#include "str.h"

#define FIND_STR        "-f"
#define REPLACE_STR     "-r"
#define DIFF_STR        "-d"

#define MAX_STR_LEN 1023

#define FALSE 0
#define TRUE  1

typedef enum {
  INVALID,
  FIND,
  REPLACE,
  DIFF
} CommandType;

/*
 * Fill out your functions here (If you need) 
 */

/*--------------------------------------------------------------------*/
/* PrintUsage()
   print out the usage of the Simple Grep Program                     */
/*--------------------------------------------------------------------*/
void 
PrintUsage(const char* argv0) 
{
  const static char *fmt = 
    "Simple Grep (sgrep) Usage:\n"
    "%s [COMMAND] [OPTIONS]...\n"
    "\nCOMMNAD\n"
    "\tFind: -f [search-string]\n"
    "\tReplace: -r [string1] [string2]\n"
    "\tDiff: -d [file1] [file2]\n";

  printf(fmt, argv0);
}
/*-------------------------------------------------------------------*/
/* DoFind()
   Your task:
   1. Do argument validation 
   - String or file argument length is no more than 1023
   - If you encounter a command-line argument that's too long, 
   print out "Error: argument is too long"
   
   2. Read the each line from standard input (stdin)
   - If you encounter a line larger than 1023 bytes, 
   print out "Error: input line is too long" 
   - Error message should be printed out to standard error (stderr)
   
   3. Check & print out the line contains a given string (search-string)
      
   Tips:
   - fgets() is an useful function to read characters from file. Note 
   that the fget() reads until newline or the end-of-file is reached. 
   - fprintf(sderr, ...) should be useful for printing out error
   message to standard error

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/
int
DoFind(const char *pcSearch)
{
  char buf[MAX_STR_LEN + 2]; 
  int len1;
  int len2;
  
  if((len1=StrGetLength(pcSearch)) > MAX_STR_LEN) {
      fprintf(stderr, "Error: argument is too long\n");
  }

   
  /* Read the line by line from stdin, Note that this is an example */
  while (fgets(buf, sizeof(buf), stdin)) {
    /* check input line length */
    if ((len2 = StrGetLength(buf)) > MAX_STR_LEN) {
      fprintf(stderr, "Error: input line is too long\n");
      return FALSE;
    }
    /* TODO: fill out this function */
    if(NULL!=StrSearch(buf,pcSearch)) printf("%s",buf);
  }

  return TRUE;
}
/*-------------------------------------------------------------------*/
/* DoReplace()
   Your task:
   1. Do argument validation 
      - String length is no more than 1023
      - If you encounter a command-line argument that's too long, 
        print out "Error: argument is too long"
      - If word1 is an empty string,
        print out "Error: Can't replace an empty substring"
      
   2. Read the each line from standard input (stdin)
      - If you encounter a line larger than 1023 bytes, 
        print out "Error: input line is too long" 
      - Error message should be printed out to standard error (stderr)

   3. Replace the string and print out the replaced string

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/
int
DoReplace(const char *pcString1, const char *pcString2)
{
  /* TODO: fill out this function */  
  int len1,len2,i,move,buf_length;
  char *p;

  /* 1 */
  if((len1=StrGetLength(pcString1)) > MAX_STR_LEN){
      fprintf(stderr,"Error: argument is too long\n" );
      return FALSE;
  }
  if((len1=StrGetLength(pcString1)) == 0){
      fprintf(stderr, "Error: Can't replace an empty substring\n");
      return FALSE;
  }
  if((len2=StrGetLength(pcString2)) > MAX_STR_LEN){
      fprintf(stderr,"Error: argument is too long\n" );
      return FALSE;
  }
  
  /* 2,3 */
  char buf[MAX_STR_LEN + 2];

  while (fgets(buf, sizeof(buf), stdin)) {
      if(StrGetLength(buf)>1022){
          fprintf(stderr,"Error: input line is too long\n");
          return FALSE;
      }
      while(NULL!= (p=StrSearch(buf,pcString1))){
          buf_length=StrGetLength(buf);
          if(len1>len2){
              move=len1-len2;
              for(i=p-buf+len1;i<buf_length;i++){
                  char tmp=buf[i];
                  buf[i]=0;
                  buf[i-move]=tmp; }
          }
          else if(len2>len1){
              move=len2-len1;
              for(i=buf_length-1;i>=p-buf+len1;i--) buf[i+move]=buf[i];
          }
          for(i=0;i<len2;i++) p[i]=pcString2[i];
        }
      printf("%s",buf);
      printf("|");
      memset(buf, 0, sizeof(buf));      /* reset buf */
    }

    /* for last line */
    fgets(buf, sizeof(buf), stdin);     

    if(StrGetLength(buf)>1022){
          fprintf(stderr,"Error: input line is too long\n");
          return FALSE; }
    while(NULL!= (p=StrSearch(buf,pcString1))){
          buf_length=StrGetLength(buf);
          if(len1>len2){
              move=len1-len2;
              for(i=p-buf+len1;i<buf_length;i++){
                  char tmp=buf[i];
                  buf[i]=0;
                  buf[i-move]=tmp; }
          }
          else if(len2>len1){
              move=len2-len1;
              for(i=buf_length-1;i>=p-buf+len1;i--) buf[i+move]=buf[i];
          }
          for(i=0;i<len2;i++) p[i]=pcString2[i];
        }
    printf("%s",buf);

  return TRUE;
}
/*-------------------------------------------------------------------*/
/* DoDiff()
   Your task:
   1. Do argument validation 
     - file name length is no more than 1023
     - If a command-line argument is too long, 
       print out "Error: argument is too long" to stderr

   2. Open the two files
      - The name of files are given by two parameters
      - If you fail to open either file, print out error messsage
      - Error message: "Error: failed to open file [filename]\n"
      - Error message should be printed out to stderr

   3. Read the each line from each file
      - If you encounter a line larger than 1023 bytes, 
        print out "Error: input line [filename] is too long" 
      - Error message should be printed out to stderr

   4. Compare the two files (file1, file2) line by line 

   5. Print out any different line with the following format
      file1@linenumber:file1's line
      file2@linenumber:file2's line

   6. If one of the files ends earlier than the other, print out an
      error message "Error: [filename] ends early at line XX", where
      XX is the final line number of [filename].

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/
int
DoDiff(const char *file1, const char *file2)
{
  /* TODO: fill out this function */  
  int len1,len2,line_num1=0,line_num2=0,count;
  char tmp1[1024],tmp2[1024];
  char *p1,*p2;

  /* 1 */
  if((len1=StrGetLength(file1)) > MAX_STR_LEN ){
      fprintf(stderr,"Error: argument is too long\n");
  }
  if((len2=StrGetLength(file1)) > MAX_STR_LEN ){
      fprintf(stderr,"Error: argument is too long\n");
  }

  /* 2 */
  FILE *p_file1=fopen(file1,"rt"),*p_file2=fopen(file2,"rt");

  if(NULL==p_file1) {
      fprintf(stderr,"Error: failed to open file %s\n",file1);
      return FALSE;
  }
  if(NULL==p_file2) {
      fprintf(stderr,"Error: failed to open file %s\n",file2);
      return FALSE;
  }
  
  while(1){
      count=0;

      if(NULL!=(p1=fgets(tmp1,sizeof(tmp1),p_file1))) line_num1++;
      else count++;
      if(NULL!=(p2=fgets(tmp2,sizeof(tmp2),p_file2))) line_num2++;
      else count++;

      if(count==2) ;    /* both file ends at same line number */
      /* 6 */
      else{
        if(line_num1>line_num2){
            fprintf(stderr,"Error: %s ends early at line %d\n",file2,line_num2);
            return FALSE;
        }
        if(line_num2>line_num1){
            fprintf(stderr,"Error: %s ends early at line %d\n",file1,line_num1);
            return FALSE;
        }
      }
      /* 3 */
      if(StrGetLength(tmp1)>1022){
          fprintf(stderr,"Error: input line %s is too long\n",file1);
          return FALSE;
      }
      if(StrGetLength(tmp2)>1022){
          fprintf(stderr,"Error: input line %s is too long\n",file2);
          return FALSE;
      }
      /* 4,5 */
      int result=StrCompare(tmp1,tmp2);
      if(result&&result!=256&&result!=256){
          printf("%s@%d:%s",file1,line_num1,tmp1);
          printf("%s@%d:%s",file2,line_num2,tmp2);
      }
    }
  return TRUE;
}
/*-------------------------------------------------------------------*/
/* CommandCheck() 
   - Parse the command and check number of argument. 
   - It returns the command type number
   - This function only checks number of argument. 
   - If the unknown function is given or the number of argument is 
   different from required number, this function returns FALSE.
   
   Note: You SHOULD check the argument rule later                    */
/*-------------------------------------------------------------------*/ 
int
CommandCheck(const int argc, const char *argv1)
{
  int cmdtype = INVALID;
   
  /* check minimum number of argument */
  if (argc < 3)
    return cmdtype;
   
  /* check command type */ 
  if (strcmp(argv1, FIND_STR) == 0) {
    if (argc != 3)
      return FALSE;    
    cmdtype = FIND;       
  }
  else if (strcmp(argv1, REPLACE_STR) == 0) {
    if (argc != 4)
      return FALSE;
    cmdtype = REPLACE;
  }
  else if (strcmp(argv1, DIFF_STR) == 0) {
    if (argc != 4)
      return FALSE;
    cmdtype = DIFF;
  }
   
  return cmdtype;
}
/*-------------------------------------------------------------------*/
int 
main(const int argc, const char *argv[]) 
{
  int type, ret;
   
  /* Do argument check and parsing */
  if (!(type = CommandCheck(argc, argv[1]))) {
    fprintf(stderr, "Error: argument parsing error\n");
    PrintUsage(argv[0]);
    return (EXIT_FAILURE);
  }
   
  /* Do appropriate job */
  switch (type) {
  case FIND:
    ret = DoFind(argv[2]);
    break;
  case REPLACE:
    ret = DoReplace(argv[2], argv[3]);
    break;
  case DIFF:
    ret = DoDiff(argv[2], argv[3]);
    break;
  } 

  return (ret)? EXIT_SUCCESS : EXIT_FAILURE;
}