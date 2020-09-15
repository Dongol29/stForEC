/* 20190617 조동올, assignment 1, wc209.c */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

enum DFAstate {IN,OUT,BAR_1,BAR_2,COMMENT,BAR_3};
/*
    IN: Inside word
    OUT: Outside word
    BAR_1: The state in which / was entered in the previous OUT state
    BAR_2: The state in which / was entered in the previous IN state
    COMMENT: Comment
    BAR_3: The state in which * was entered in the previous COMMENT state
*/
int comment_linenumber;


void treat_OUT(char c, enum DFAstate *ptr, int *p_lc, int *p_wc, int *p_cc )
{
    /*
          Implements the action on the OUT state. 
          Increase the number of lines, words, and characters or 
        change the state depending on the input. 
    */
    if(c=='/') { *ptr=BAR_1; (*p_cc)++; (*p_wc)++;}
    else if(!isspace(c)&&c!='/') {*ptr=IN; (*p_cc)++; (*p_wc)++; }
    else if(isspace(c)&&c!='\n') (*p_cc)++;
    else if(c=='\n') {(*p_cc)++; (*p_lc)++;}

}

void treat_IN(char c, enum DFAstate *ptr, int *p_lc, int *p_cc)
{
    /*
          Implements the action on the IN state. 
          Increase the number of lines, words, and characters or
        change the state depending on the input. 
    */
    if(c=='/') { *ptr=BAR_2; (*p_cc)++;}
    else if(!isspace(c)&&c!='/') (*p_cc)++;
    else if(isspace(c)&&c!='\n') { *ptr=OUT; (*p_cc)++;}
    else if(c=='\n') {*ptr=OUT; (*p_cc)++; (*p_lc)++;}
}

void treat_BAR1(char c, enum DFAstate *ptr, int *p_lc, int *p_wc, int *p_cc)
{
    /*
          Implements the action on the BAR_1 state. 
          Increase the number of lines, words, characters or
        change the state depending on the input. 
          When detecting state change to COMMENT, save the current line number.
    */
    if(c=='/') (*p_cc)++;
    else if(isspace(c)&&c!='\n') { *ptr=OUT; (*p_cc)++;}
    else if(c=='\n') { *ptr=OUT; (*p_cc)++; (*p_lc)++;}
    else if(!isspace(c)&&c!='*') { *ptr=IN; (*p_cc)++;}
    else if(c=='*') { *ptr=COMMENT; (*p_wc)--; (*p_cc)--; comment_linenumber=(*p_lc);} \
}

void treat_BAR2(char c, enum DFAstate *ptr, int *p_lc, int *p_cc)
{
    /*
          Implements the action on the BAR_2 state. 
          Increase the number of lines, words, characters or
        change the state depending on the input. 
          When detecting state change to COMMENT, save the current line number.
    */
    if(c=='/') (*p_cc)++;
    else if(isspace(c)&&c!='\n') { *ptr=OUT; (*p_cc)++;}
    else if(c=='\n') { *ptr=OUT; (*p_cc)++; (*p_lc)++;}
    else if(!isspace(c)&&c!='*') { *ptr=IN; (*p_cc)++;}
    else if(c=='*') { *ptr=COMMENT; (*p_cc)--; comment_linenumber=(*p_lc);} 
}

void treat_COMMENT(char c, enum DFAstate *ptr, int *p_lc, int *p_cc)
{
    /*
          Implements the action on the COMMENT state. 
          Increase the number of lines, words, characters or
        change the state depending on the input. 
    */
    if(c=='\n') { (*p_cc)++; (*p_lc)++;}
    else if(c!='\n'&&c!='*') ;
    else if(c=='*') *ptr=BAR_3;
}

void treat_BAR3(char c, enum DFAstate *ptr, int *p_lc, int *p_cc)
{
    /*
          Implements the action on the BAR_3 state. 
          Increase the number of lines, words, characters or
        change the state depending on the input. 
    */
    if(c=='\n') { *ptr=COMMENT; (*p_cc)++; (*p_lc)++;}  
    else if(c!='\n'&&c!='*'&&c!='/') *ptr=COMMENT;
    else if(c=='*') ;
    else if(c=='/') { *ptr=OUT; (*p_cc)++;}
}

int main()
{
    /*
          Reads characters from the input.
          Writes the number of lines, words and characters in the input text with
        each comment replaced by a space.
          When detect an unterminated comment, writes error and warning messages.
    */
    int line_count=1,word_count=0,char_count=0,c; 
    enum DFAstate state=OUT;
    while((c=getchar())!=EOF){
        switch(state){

            case OUT:
                treat_OUT(c, &state, &line_count, &word_count, &char_count);
                break;

            case IN:
                treat_IN(c, &state, &line_count, &char_count);
                break;

            case BAR_1:
                treat_BAR1(c, &state, &line_count, &word_count, &char_count);  
                break;

            case BAR_2:
                treat_BAR2(c, &state, &line_count, &char_count);
                break;

            case COMMENT:
                treat_COMMENT(c, &state, &line_count, &char_count);
                break;

            case BAR_3:
                treat_BAR3(c, &state, &line_count, &char_count);
                break;
            
            default:
                assert(0); /*error*/
                break;
        }
    }
    if(char_count==0) line_count=0; //When the file is empty

    if(state==COMMENT||state==BAR_3) {
        fprintf(stderr, "Error: line %d: unterminated comment\n", comment_linenumber);
        return EXIT_FAILURE;}
    else {
        printf("%d %d %d\n",line_count,word_count,char_count);
        return EXIT_SUCCESS;}  
}