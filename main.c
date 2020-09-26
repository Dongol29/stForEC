#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

size_t StrGetLength(const char* pcSrc)
{
  const char *pcEnd;
  assert(pcSrc); /* NULL address, 0, and FALSE are identical. */
  pcEnd = pcSrc;
	
  while (*pcEnd) /* null character and FALSE are identical. */
    pcEnd++;

  return (size_t)(pcEnd - pcSrc);
}

int StrSearch(const char* pcHaystack, const char *pcNeedle)
{
    int number=0;
    int state=0;
    const char *initial_address1=pcHaystack;
    const char *initial_address2=pcNeedle;

    const char *occur_address;
    size_t length=StrGetLength(pcNeedle);
    printf("%d\n",length);
    while(1){
        if(number>length-1) break;      //찾음
        if(*pcHaystack=='\0'&&number<length) { printf("fail to find\n"); return NULL;}      //못찾음
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
                else { state=0; pcNeedle=initial_address2; number=0;}
                break;
            default:
                assert(0); /*error*/
                break;
        }
    }
    return occur_address-initial_address1;
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
        if(0==*pcS1&&0!=*pcS2) return -1;
        printf("ddsf\n");
        if(*pcS1==*pcS2){
            pcS1++; pcS2++;
            continue;
        }
        else return (*pcS1>*pcS2)? 1:-1;
    }
    return 0;
    //return strcmp(pcS1, pcS2);
}

void main()
{
    char d1[]="dongol is great";
    char d2[]=" is f";
    
    int data=StrSearch(d1,d2);
    printf("%d\n",data);
}