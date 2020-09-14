
#include <stdio.h>

void main()
{
    char a=0x12, b=0x34;
    short c=0x5678;
    int t;
    void *p=&t;
    *(short *)p=c;
    *(char *)(p+2)=b;
    *(char *)(p+3)=a;
    printf("%x",t);
}
