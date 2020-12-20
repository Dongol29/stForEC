#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

void main()
{
    int a[2];
    pipe(a);
    pipe(a);
}
    
