#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


void main()
{
    char *t=(char *)malloc(sizeof(char));
    *t='a';
    char *s=t;
    free(t);
    printf("%c\n",*s);
}