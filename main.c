#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


int prime(int a) //a>=2
{
	int i,n;
	unsigned int count=0; //a이하의 소수는 무조건 존재함
	for(n=a;n>=2;n--){
		for(i=2;i<n;i++){
			if(n%i==0) break;
			if(i==n-1)  return n;
		}
	    continue;
	}
}

void main()
{
    printf("%d\n",prime(96));   
}