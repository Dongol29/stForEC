#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


void main()
{
    char *p=(char *)malloc(sizeof(char));
    strcpy(p,"c");
    char *q=p;
    strcmp(p,q);
}