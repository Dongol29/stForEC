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

void main()
{
    char d1[20]="dongol ";
    char d2[]="donghwi";
    StrConcat(d1,d2);

    printf("%s\n",d1);
}