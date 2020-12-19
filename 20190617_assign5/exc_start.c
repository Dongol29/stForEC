/*--------------------------------------------------------------------*/
/* dfa.c                                                              */
/* Original Author: Bob Dondero                                       */
/* Illustrate lexical analysis using a deterministic finite state     */
/* automaton (DFA)                                                    */
/*--------------------------------------------------------------------*/

#include "dynarray.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*--------------------------------------------------------------------*/

enum {MAX_LINE_SIZE = 1024};

enum {FALSE, TRUE};

enum TokenType {TOKEN_WORD, TOKEN_PIPE, END};

/*--------------------------------------------------------------------*/

/* A Token is either a number or a word, expressed as a string. */

struct Token
{
   enum TokenType eType;
   /* The type of the token. */

   char *pcValue;
   /* The string which is the token's value. */
};

struct DynArray
{
	/* The number of elements in the DynArray from the client's
	   point of view. */
	int iLength;
	
	/* The number of elements in the array that underlies the
	   DynArray. */
	int iPhysLength;
	
	/* The array that underlies the DynArray. */
	const void **ppvArray;
};

/*--------------------------------------------------------------------*/

static void freeToken(void *pvItem, void *pvExtra)

/* Free token pvItem.  pvExtra is unused. */

{
   struct Token *psToken = (struct Token*)pvItem;
   free(psToken->pcValue);
   free(psToken);
}

/*--------------------------------------------------------------------*/

static void printToken(void *pvItem, void *pvExtra)

/* Print token pvItem to stdout iff it is a word.  pvExtra is
   unused. */

{
   struct Token *psToken = (struct Token*)pvItem;
   printf("%s\n", psToken->pcValue);
}

/*--------------------------------------------------------------------*/

static struct Token *makeToken(enum TokenType eTokenType,
   char *pcValue)

/* Create and return a Token whose type is eTokenType and whose
   value consists of string pcValue.  Return NULL if insufficient
   memory is available.  The caller owns the Token. */

{
   struct Token *psToken;

   psToken = (struct Token*)malloc(sizeof(struct Token));
   if (psToken == NULL)
      return NULL;

   psToken->eType = eTokenType;

   psToken->pcValue = (char*)malloc(strlen(pcValue) + 1);
   if (psToken->pcValue == NULL)
   {
      free(psToken);
      return NULL;
   }

   strcpy(psToken->pcValue, pcValue);

   return psToken;
}

/*--------------------------------------------------------------------*/

static int lexLine(const char *pcLine, DynArray_T oTokens)

/* Lexically analyze string pcLine.  Populate oTokens with the
   tokens that pcLine contains.  Return 1 (TRUE) if successful, or
   0 (FALSE) otherwise.  In the latter case, oTokens may contain
   tokens that were discovered before the error. The caller owns the
   tokens placed in oTokens. */

/* lexLine() uses a DFA approach.  It "reads" its characters from
   pcLine. */

{
   enum LexState {STATE_START, STATE_WORD, STATE_QUOTE};

   enum LexState eState = STATE_START;

   int iLineIndex = 0;
   int iValueIndex = 0;
   char c;
   char acValue[MAX_LINE_SIZE]={0,};
   struct Token *psToken;

   assert(pcLine != NULL);
   assert(oTokens != NULL);

   for (;;)
   {
      /* "Read" the next character from pcLine. */
      c = pcLine[iLineIndex++];

      switch (eState)
      {
         case STATE_START:
            if ((c == '\n') || (c == '\0'))
               return TRUE;
            else if (!isspace(c)&&c!='|'&&c!='"')
            {
               acValue[iValueIndex++] = c;
               eState = STATE_WORD;
            }
            else if ((c == ' ') || (c == '\t'))
               eState = STATE_START;
            else if (c=='|')
            {
                acValue[iValueIndex++]=c;
                acValue[iValueIndex]='\0';
                psToken=makeToken(TOKEN_PIPE,acValue);
                if (psToken == NULL)
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               iValueIndex=0;
               eState=STATE_START;
            }
            else if (c=='"')
            {
                printf("hola\n");
                eState=STATE_QUOTE;
            }

            else
            {
               fprintf(stderr, "Invalid line\n");
               return FALSE;
            }
            break;

         case STATE_WORD:
            if ((c == '\n') || (c == '\0'))
            {
               /* Create a NUMBER token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(TOKEN_WORD, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               iValueIndex = 0;

               return TRUE;
            }
            else if (!isspace(c)&&c!='|'&&c!='"')
            {
               acValue[iValueIndex++] = c;
               eState = STATE_WORD;
            }
            else if ((c == ' ') || (c == '\t'))
            {
               /* Create a word token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(TOKEN_WORD, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               iValueIndex = 0;

               eState = STATE_START;
            }
            else if (c=='|')
            {
                //First, create word token
                acValue[iValueIndex]='\0';
                psToken = makeToken(TOKEN_WORD, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               iValueIndex = 0;

               //Next, create pipe token
               acValue[iValueIndex++]=c;
               acValue[iValueIndex]='\0';
               psToken = makeToken(TOKEN_WORD, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               iValueIndex = 0;
               eState=STATE_START;
            }
            else if (c=='"')
            {
                eState=STATE_QUOTE;
            }
            else
            {
               fprintf(stderr, "Invalid line\n");
               return FALSE;
            }
            break;

         case STATE_QUOTE:
            if((c == '\n') || (c == '\0'))
            {
                fprintf(stderr,"./ish: Could not find quote pair\n");
                return FALSE;
            }
            else if (c=='"')
            {
                eState=STATE_WORD;
            }
            else
            {
                acValue[iValueIndex++]=c;
            }
            break;

         default:
            assert(FALSE);
      }
      //printf("step:%s\n",acValue);
   }
}

static int synLine(DynArray_T oTokens)

/* lexline 이랑 비슷한 comment */
{
    enum SynState {STATE_START, STATE_WORD, STATE_PIPE};

    enum SynState eState = STATE_START;

    assert(oTokens != NULL);

    struct Token *Token;
    
    enum TokenType type;

    int i=0, count=0; //count: pipe의 개수 셈;개수+1만큼 fork?
    for (;;)
    {
   
        Token=(struct Token *)DynArray_get(oTokens,i);
        if(NULL==Token) type=END;
        else type=Token->eType;

        switch(eState)
        {
            case STATE_START:
                if(type==TOKEN_WORD)
                {
                    eState=STATE_WORD;
                }
                else if(type==TOKEN_PIPE)
                //error
                {
                    fprintf(stderr,"./ish: Missing command name\n");
                    return (-1);
                }
                else  //이 때 더이상 pstoken없는게 맞나?
                {
                    return count;
                }
                break;

            case STATE_WORD:
                if(type==TOKEN_WORD)
                {
                    eState=STATE_WORD;
                }
                else if(type==TOKEN_PIPE)
                {
                    count++;
                    eState=STATE_PIPE;
                }
                else //이 때 더이상 pstoken없는게 맞나?
                {
                    return count;
                }
                break;

            case STATE_PIPE:
                if(type==TOKEN_WORD) //count++해야하나?
                {
                    eState=STATE_WORD;
                }
                else 
                {
                    fprintf(stderr,"./ish: Pipe or redirection destination not specified\n");
                    return (-1);
                }
                break;

            default:
                assert(FALSE);
        }  
        i++;
    }
}

char *** make_Cmd(DynArray_T oTokens,int num_pipe)
{
   //printf("NUM PIPE: %d\n",num_pipe);
   char ***cmds=(char ***)calloc(num_pipe+1,sizeof(char **));
   if(cmds==NULL){
         fprintf(stderr,"Memory allocation error!!\n");
         return NULL;
      }
   int i,k,j=0;
   //int m;
   struct Token *Token;

   for(i=0;i<=num_pipe;i++){  //cmd 수는 num_pipe+1개
      cmds[i]=(char **)calloc(1024,sizeof(char *)); //cmd하나에 존재하는 토큰수
      if(cmds[i]==NULL){
         fprintf(stderr,"Memory allocation error!!\n");
         return NULL;
      }
      k=0;
      while(1){
         Token=(struct Token *)DynArray_get(oTokens,j++);
         if(NULL==Token||Token->eType==TOKEN_PIPE) {
            if(k==0){
               free(cmds[0]); free(cmds);
               return NULL;
            }
            break;  
         }
         cmds[i][k]=Token->pcValue;
         k++;
      }

   }
   /*  i=0,k=0으로 끝나면 여기서 segfault나옴
    for(m=0;m<=num_pipe;m++){
      printf("%s\n",cmds[m][0]);
    }
   */
   return cmds;
}

int exc1_Line(char ***cmds)
/* pipe 없는 경우 */
{
   char *name=cmds[0][0];
   int result;
   if(strcmp(name,"setenv")==0){
      if(cmds[0][1]==NULL){
         fprintf(stderr,"./ish: setenv takes one or two parameters\n");
         return (-1);
      }
      else if(cmds[0][2]==NULL){
         result=setenv(cmds[0][1],"",1);
         if(result==-1){
            fprintf(stderr,"setenv failed\n");
            return (-1);
         }
      }
      else{
         result=setenv(cmds[0][1],cmds[0][2],1);
         if(result==-1){
            fprintf(stderr,"setenv failed\n");
            return (-1);
         }
      }
   }

   else if(strcmp(name,"unsetenv")==0){

   }

   else if(strcmp(name,"cd")==0){

   }

   else if(strcmp(name,"exit")==0){

   }
}
int exc2_Line(char ***cmds,int num_pipe)
{

}




/*--------------------------------------------------------------------*/

int main(void)

/* Read a line from stdin, and write to stdout each number and word
   that it contains.  Repeat until EOF.  Return 0 iff successful. */

{
   char acLine[MAX_LINE_SIZE];
   DynArray_T oTokens;
   int iSuccessful;
   int num_pipe;

   char ***cmds;

   printf("------------------------------------\n");
   while (fgets(acLine, MAX_LINE_SIZE, stdin) != NULL)
   {
      oTokens = DynArray_new(0);
      if (oTokens == NULL)
      {
         fprintf(stderr, "Cannot allocate memory\n");
         exit(EXIT_FAILURE);
      }

      iSuccessful = lexLine(acLine, oTokens);
      if (iSuccessful)
      {
          //printf("Tokens:  ");
          DynArray_map(oTokens, printToken, NULL);
          printf("\n");
         /*
          if(synLine(oTokens))
          {
              printf("Valid\n");
          }
         */
      }
      num_pipe = synLine(oTokens);
      if(num_pipe>=0) 
      // 1.char ***만듬 2.prm으로 token 받고, pipe토큰 전까지의 pcvalue를 element로 하는
      //char **argv생성--> 
      {
         cmds=make_Cmd(oTokens,num_pipe);
         if(num_pipe==0) exc1_Line(cmds);
         else exc2_Line(cmds,num_pipe);
      }
      
      printf("------------------------------------\n");

      DynArray_map(oTokens, freeToken, NULL);
      DynArray_free(oTokens);
   }

   return 0;
}