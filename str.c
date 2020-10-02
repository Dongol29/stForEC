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
    If  the  programmer  knows that the size of dest is greater 
    than the length of src, then strcpy() can be used.
    the  program  first  needs  to  check that there's enough space.  
  */
    int i;
    assert(NULL!=pcDest&&NULL!=pcSrc);
    
    //size_t length1=sizeof(pcDest);
    size_t length2=StrGetLength(pcSrc);
    //if(length1<=length2) fprintf(stderr, "Illegal instruction: 4");


    while(*pcSrc){
        *pcDest=*pcSrc;
        pcDest++; pcSrc++;
    }
    *pcDest=0;
    for(i=0;i<length2;i++){
        pcDest--;
    }
    return pcDest;
  
  //return strcpy(pcDest, pcSrc);
}

/*------------------------------------------------------------------------*/
int StrCompare(const char* pcS1, const char* pcS2)
{
    /*
        The  strcmp() function compares the two strings s1 and s2.  It returns an integer less
        than, equal to, or greater than zero if s1 is found, respectively, to be less than, to
        match, or be greater than s2.
    */
    assert(NULL!=pcS1&&NULL!=pcS2);
    while(1){
        if(0==*pcS1&&0==*pcS2) return 0;
        else if(0==*pcS1&&0!=*pcS2) return -1;
        else if(0!=*pcS1&&0==pcS2) return 1;
        if(*pcS1==*pcS2){
            pcS1++; pcS2++;
            continue;
        }
        else return (*pcS1>*pcS2)? 1:-1;
    }
    return 0;
    //return strcmp(pcS1, pcS2);
}
/*------------------------------------------------------------------------*/
char *StrSearch(const char* pcHaystack, const char *pcNeedle)
{
    /* 
        The terminating null bytes ('\0') are not compared.
        These functions return a pointer to the beginning of the substring,  or  NULL  if  the
        substring is not found.
        return haystack when needle is empty.
    */ 
    assert(NULL!=pcHaystack&&NULL!=pcNeedle);
    int number=0;
    int state=0;
    const char *initial_address=pcNeedle;
    const char *occur_address;
    size_t length=StrGetLength(pcNeedle);
    
    if(0==*pcNeedle) return (char *)pcHaystack;
    while(1){
        if(*pcHaystack==0) return NULL;         //못찾음
        if(number>length-1) break;      //찾음
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


    //return strstr(pcHaystack, pcNeedle);
}
/*------------------------------------------------------------------------*/
char *StrConcat(char *pcDest, const char* pcSrc)
{
    /*
        overwriting theterminating null byte ('\0') at the end of dest
        adds  a  terminating  null byte.
        string must have enough space for the result.
        return a pointer to the resulting string dest.
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