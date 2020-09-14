#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum DFAstate {IN,OUT,BAR_1,BAR_2,COMMENT,BAR_3};
int comment_linenumber;

void treat_OUT(char c, enum DFAstate *ptr, int *p_lc, int *p_wc, int *p_cc )
{
    if(c=='/') { *ptr=BAR_1; (*p_cc)++; (*p_wc)++;}
    else if(!isspace(c)&&c!='/') {*ptr=IN; (*p_cc)++; (*p_wc)++; }
    else if(isspace(c)&&c!='\n') (*p_cc)++;
    else if(c=='\n') {(*p_cc)++; (*p_lc)++;}

}

void treat_IN(char c, enum DFAstate *ptr, int *p_lc, int *p_cc)
{
    if(c=='/') { *ptr=BAR_2; (*p_cc)++;}
    else if(!isspace(c)&&c!='/') (*p_cc)++;
    else if(isspace(c)&&c!='\n') { *ptr=OUT; (*p_cc)++;}
    else if(c=='\n') {*ptr=OUT; (*p_cc)++; (*p_lc)++;}
}

void treat_BAR1(char c, enum DFAstate *ptr, int *p_lc, int *p_wc, int *p_cc)
{
    if(c=='/') (*p_cc)++;
    else if(isspace(c)&&c!='\n') { *ptr=OUT; (*p_cc)++;}
    else if(c=='\n') { *ptr=OUT; (*p_cc)++; (*p_lc)++;}
    else if(!isspace(c)&&c!='*') { *ptr=IN; (*p_cc)++;}
    else if(c=='*') { *ptr=COMMENT; (*p_wc)--; (*p_cc)--; comment_linenumber=(*p_lc);} //COMMENT상태 line 번호 저장
}

void treat_BAR2(char c, enum DFAstate *ptr, int *p_lc, int *p_cc)
{
    if(c=='/') (*p_cc)++;
    else if(isspace(c)&&c!='\n') { *ptr=OUT; (*p_cc)++;}
    else if(c=='\n') { *ptr=OUT; (*p_cc)++; (*p_lc)++;}
    else if(!isspace(c)&&c!='*') { *ptr=IN; (*p_cc)++;}
    else if(c=='*') { *ptr=COMMENT; (*p_cc)--; comment_linenumber=(*p_lc);} 
}

void treat_COMMENT(char c, enum DFAstate *ptr, int *p_lc, int *p_cc)
{
    if(c=='\n') { (*p_cc)++; (*p_lc)++;}
    else if(c!='\n'&&c!='*') ;
    else if(c=='*') *ptr=BAR_3;
}

void treat_BAR3(char c, enum DFAstate *ptr, int *p_lc, int *p_cc)
{
    if(c=='\n') { *ptr=COMMENT; (*p_cc)++; (*p_lc)++;}  
    else if(c!='\n'&&c!='*'&&c!='/') *ptr=COMMENT;
    else if(c=='*') ;
    else if(c=='/') { *ptr=OUT; (*p_cc)++;}
}

int main()
{
    int line_count=1,word_count=0,char_count=0,c; //line_count는 1로 설정해둠
    enum DFAstate state=OUT;
    while((c=getchar())!=EOF){
        //printf("%c",c);
        switch(state){
            case OUT:
                treat_OUT(c, &state, &line_count, &word_count, &char_count);
                //if(c=='/') {state=BAR_1; char_count++; word_count++; }
                //else if(!isspace(c)&&c!='/') {state=IN; char_count++; word_count++; }
                //else if(isspace(c)&&c!='\n') char_count++;
                //else if(c=='\n') {char_count++; line_count++;}
                //printf("%d ",state);
                break;
            case IN:
                treat_IN(c, &state, &line_count, &char_count);
                //if(c=='/') {state=BAR_2; char_count++;}
                //else if(!isspace(c)&&c!='/') char_count++;
                //else if(isspace(c)&&c!='\n') {state=OUT; char_count++;}
                //else if(c=='\n') {state=OUT; char_count++; line_count++;}
                //printf("%d ",state);
                break;
            case BAR_1:
                treat_BAR1(c, &state, &line_count, &word_count, &char_count);
                //if(c=='/') char_count++;
                //else if(isspace(c)&&c!='\n') {state=OUT; char_count++;}
                //else if(c=='\n') {state=OUT; char_count++; line_count++;}
                //else if(!isspace(c)&&c!='*') {state=IN; char_count++;}
                //else if(c=='\n') {state=OUT; char_count++; line_count++;}
                //else if(c!='\n'&&c!='*') {state=IN; char_count++;}
                //else if(c=='*') {state=COMMENT; word_count--; char_count--; comment_line=line_count;} //COMMENT상태 line 번호 저장
                //printf("%d ",state);
                break;
            case BAR_2:
                treat_BAR2(c, &state, &line_count, &char_count);
                //if(c=='/') char_count++;
                //else if(c=='\n') {state=OUT; char_count++; line_count++;}
                //else if(c!='\n'&&c!='*') {state=IN; char_count++;}
                //else if(isspace(c)&&c!='\n') {state=OUT; char_count++;}
                //else if(c=='\n') {state=OUT; char_count++; line_count++;}
                //else if(!isspace(c)&&c!='*') {state=IN; char_count++;}
                //else if(c=='*') {state=COMMENT; char_count--; comment_line=line_count;} //COMMENT상태 line 번호 저장
                //printf("%d ",state);
                break;
            case COMMENT:
                treat_COMMENT(c, &state, &line_count, &char_count);
                //if(c=='\n') {char_count++; line_count++;}
                //else if(c!='\n'&&c!='*') ;
                //else if(c=='*') state=BAR_3;
                //printf("%d ",state);
                break;
            case BAR_3:
                treat_BAR3(c, &state, &line_count, &char_count);
                //if(c=='\n') {state=COMMENT; char_count++; line_count++;}   
                //else if(c!='\n'&&c!='*'&&c!='/') state=COMMENT;
                //else if(c=='*') ;
                //else if(c=='/') {state=OUT; char_count++;}
                //printf("%d ",state);
                break;
        }
    }
    if(char_count==0) line_count=0; //-->line수=0

    if(state==COMMENT||state==BAR_3) {
        fprintf(stderr, "Error: line %d: unterminated comment\n", comment_linenumber);
        //printf("Error: line %d: unterminated comment\n",comment_line);
        return EXIT_FAILURE;}
    else {
        printf("%d %d %d\n",line_count,word_count,char_count);
        return EXIT_SUCCESS;}  
}