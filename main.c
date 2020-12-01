#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


void main()
{
    char *st="";
    char *ss={"\0"};
    int a=strcmp(st,ss);
    printf("%d\n",a);
}