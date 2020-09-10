//
//  main.c
//  stForEC
//
//  Created by dongol cho on 2020/08/31.
//  Copyright © 2020 dongol cho. All rights reserved.
//

#include <stdio.h>
#include <string.h>


void getmystring(char data[],int length)
{
    int i;
    char ch;
    for(i=0;i<length-1;i++){
        ch=getchar();
        if('\n'==ch) break;
        *(data+i)=ch;
    }
    rewind(stdin);
}


void main()
{
    char tmp[10]={0,};
    
    getmystring(tmp,sizeof(tmp));
    printf("%s\n",tmp);
    
}