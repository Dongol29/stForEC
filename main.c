#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void main()
{
    char buffer[20]={'a',};
    printf("%s\n",&buffer[0]);
    scanf("%s",buffer);
    printf("%s\n",&buffer[0]);
}