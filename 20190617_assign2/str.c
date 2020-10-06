/* 20190617 조동올, assignment 2, str.c */

#include <assert.h> /* to use assert() */
#include <stdio.h>
#include "str.h"

/* Your task is: 
   1. Rewrite the body of "Part 1" functions - remove the current
      body that simply calls the corresponding C standard library
      function.
   2. Write appropriate comment per each function
*/

/* Part 1 */
/*------------------------------------------------------------------------*/
size_t StrGetLength(const char* pcSrc)
{
  const char *pcEnd;
  assert(pcSrc); /* NULL address, 0, and FALSE are identical. */
  pcEnd = pcSrc;
	
  while (*pcEnd) /* null character and FALSE are identical. */
    pcEnd++;

  return (size_t)(pcEnd - pcSrc);
}

/*------------------------------------------------------------------------*/
char *StrCopy(char *pcDest, const char* pcSrc)
{
  /*
     This function copies the string pointed to by pcSrc, including the
    terminating null byte ('\0'), to the buffer pointed to by pcDest. 
     The strings may not overlap, and the destination string dest must 
    be large enough to receive the copy.
     Return value is a pointer to the starting destination of buffer.
     Assert that each parameter is not NULL.
  */
    assert(NULL!=pcDest&&NULL!=pcSrc);

    int i;
    size_t length2=StrGetLength(pcSrc);

    while(*pcSrc){
        *pcDest=*pcSrc;
        pcDest++; pcSrc++;
    }
    *pcDest=0;

    for(i=0;i<length2;i++){
        pcDest--;
    }
    return pcDest;
  
}

/*------------------------------------------------------------------------*/
int StrCompare(const char* pcS1, const char* pcS2)
{
    /*
       This function compares the two strings pointed to by 
      pcS1 and pcS2.  
       The return value is an integer less than, equal to, or greater
      than zero if string 1 is found, respectively, to be less than, 
      to match, or be greater than string 2.
       Assert that each parameter is not NULL.
    */
    assert(NULL!=pcS1&&NULL!=pcS2);

    while(1){
        if(0==*pcS1&&0==*pcS2) return 0;
        else if(0==*pcS1&&0!=*pcS2) return -256;  
        else if(0!=*pcS1&&0==pcS2) return 256;
        if(*pcS1==*pcS2){
            pcS1++; pcS2++;
            continue;
        }
        else return (*pcS1>*pcS2)? *pcS1-*pcS2:*pcS1-*pcS2;
    }
    return 0;
}
/*------------------------------------------------------------------------*/
char *StrSearch(const char* pcHaystack, const char *pcNeedle)
{
    /* 
       This function finds the first occurrence of the substring 
      pointed to by pcNeedle in the string pointed to by pcHaystack.
       The terminating null bytes ('\0') are not compared.
       Returns a pointer to the beginning of the substring,  or  
      NULL  if  the substring is not found. 
       Returns haystack when needle(substring) is empty.
       Assert that each parameter is not NULL.
    */ 
    assert(NULL!=pcHaystack&&NULL!=pcNeedle);

    int number=0;
    int state=0;
    const char *initial_address=pcNeedle;
    const char *occur_address;
    size_t length=StrGetLength(pcNeedle);
    
    if(0==*pcNeedle) return (char *)pcHaystack;
    while(1){
        if(*pcHaystack==0) return NULL;        /* not found */
        if(number>length-1) break;             /* found */
        switch(state){
            case 0: 
                if(*pcNeedle==*pcHaystack){ 
                    state=1; 
                    occur_address=pcHaystack;
                    number++; pcNeedle++; pcHaystack++;
                    }
                else pcHaystack++; 
                break;
            case 1:
                if(*pcNeedle==*pcHaystack) { number++; pcNeedle++; pcHaystack++;}
                else { state=0; pcNeedle=initial_address; number=0;}
                break;
            default:
                assert(0); /*error*/
                break;
        }
    }
    return (char *)occur_address;
}
/*------------------------------------------------------------------------*/
char *StrConcat(char *pcDest, const char* pcSrc)
{
    /*
       This function  appends  the src string to the dest string, 
      overwriting the terminating null byte ('\0') at the end of dest,
      and then adds  a  terminating  null byte. 
       Each string is pointed to by pcDest and pcSrc.
       The  strings may not overlap, and the dest string must have 
      enough space for the result.
       Returns a pointer to the resulting string dest.
       Assert that each parameter is not NULL.
    */
    assert(NULL!=pcDest&&NULL!=pcSrc);

    const char *initial_address=pcDest;

    while(*pcDest) pcDest++;
    while(1){
        if(*pcSrc==0) break;
        *pcDest=*pcSrc;
        pcDest++; pcSrc++;
    }
    *pcDest=0;
    return (char *)initial_address;
}