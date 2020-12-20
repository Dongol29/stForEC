#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

void main()
{
   char *pathname=getenv("HOME");
   strcat(pathname,"/.ishrc");
   printf("%s\n",pathname);
}
    
