#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void StrConcat(char *pcDest, const char* pcSrc)
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
    //return (char *)initial_address;
}

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
size_t StrGetLength(const char* pcSrc)
{
  const char *pcEnd;
  assert(pcSrc); /* NULL address, 0, and FALSE are identical. */
  pcEnd = pcSrc;
	
  while (*pcEnd) /* null character and FALSE are identical. */
    pcEnd++;

  return (size_t)(pcEnd - pcSrc);
}

void main()
{
    char data[]="dongol";
    char *p=data;
    size_t a=StrGetLength(p);
    printf("%zu\n",a);
}