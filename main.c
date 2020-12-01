#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


void main()
{
    char s[10];
    fgets(s,10,stdin);
    printf("%c\n",s[7]);
}