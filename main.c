#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>


void main()
{
   char *a=(char *)malloc(sizeof(char)+1);
   strcpy(a,"b");
   char b[10]=a;
   printf("%s\n",b);
}
    
