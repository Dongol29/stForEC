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

void main()
{
    char d1[]="dongol is great";
    char d2[]=" is f";

    int data=StrSearch(d1,d2);
    printf("%d\n",data);
}