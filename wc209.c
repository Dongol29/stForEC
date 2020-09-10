#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum DFAstate {IN,OUT,BAR_1,COMMENT,BAR_3};

int main()
{
    int line_count=1,word_count=0,char_count=0,c,comment_line; //line_count는 1로 설정해둠
    enum DFAstate state=OUT;
    while((c=getchar())!=EOF){
        switch(state){
            case OUT:
                if(c=='/') {state=BAR_1; char_count++; word_count++;}
                else if(!isspace(c)&&c!='/') {state=IN; char_count++; word_count++;}
                else if(isspace(c)&&c!='\n') char_count++;
                else if(c=='\n') {char_count++; line_count++;}
                break;
            case IN:
                if(c=='/') {state=BAR_1; char_count++;}
                else if(!isspace(c)&&c!='/') char_count++;
                else if(isspace(c)&&c!='\n') {state=OUT; char_count++;}
                else if(c=='\n') {state=OUT; char_count++; line_count++;}
                break;
            case BAR_1:
                if(c=='/') char_count++;
                else if(c=='\n') {state=IN; char_count++; line_count++;}
                else if(c!='\n'&&c!='*') {state=IN; char_count++;}
                else if(c=='*') {state=COMMENT; char_count--; comment_line=line_count;} //COMMENT상태 line 번호 저장
                break;
            case COMMENT:
                if(c=='\n') {char_count++; line_count++;}
                else if(c!='\n'&&c!='*') ;
                else if(c=='*') state=BAR_3;
                break;
            case BAR_3:
                if(c=='\n') {state=COMMENT; char_count++; line_count++;}   //'\n'같은게 */에(COMMENT가 되는지) 영향 주나??
                else if(c!='\n'&&c!='*') state=COMMENT;
                else if(c=='*') ;
                else if(c=='/') {state=OUT; char_count++;}
                break;
        }
    }
    if(char_count==0) line_count=0; //빈 파일-->line수=0

    if(state==COMMENT||state==BAR_3) {
        printf("Error: line %d: unterminated comment\n",comment_line);
        return EXIT_FAILURE;}
    else {
        printf("%d %d %d\n",char_count,word_count,line_count);
        return EXIT_SUCCESS;}  
}