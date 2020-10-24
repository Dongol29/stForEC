#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


void myabs(int *p)
{
    if(*p<0) *p=(*p)*(-1);
}

int sumabs(int a, int b, void (*pa)(int *),void (*pb)(int *));
{
    if(pa!=NULL) (*pa)(&a);
    myabs(&b);

    return a+b;
}