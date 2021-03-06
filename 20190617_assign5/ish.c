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
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <pwd.h>
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
                /* Create a pipe token. */
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
               psToken = makeToken(TOKEN_PIPE, acValue);
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
                fprintf(stderr,"./ish: Could not find quote pair");
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
   }
}

/*--------------------------------------------------------------------*/

static int synLine(DynArray_T oTokens)

/* Syntatically analyze oTokens.
   Return the number of pipe if successful,(-1) otherwise.  */
{
    enum SynState {STATE_START, STATE_WORD, STATE_PIPE};

    enum SynState eState = STATE_START;

    assert(oTokens != NULL);

    struct Token *Token;
    
    enum TokenType type;

    int i=0, count=0; 
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
                else  
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
                else 
                {
                    return count;
                }
                break;

            case STATE_PIPE:
                if(type==TOKEN_WORD) 
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

/*--------------------------------------------------------------------*/

char *** make_Cmd(DynArray_T oTokens,int num_pipe)
/*
  Return triple pointer 'Cmd' if successful, which indicates commands which were
  separated based on pipe tokens. Otherwise return NULL. 
*/
{
   char ***cmds=(char ***)calloc(num_pipe+1,sizeof(char **));
   if(cmds==NULL){
         fprintf(stderr,"Memory allocation error!!\n");
         return NULL;
      }
   int i,k,j=0;
   struct Token *Token;

   for(i=0;i<=num_pipe;i++){  // the number of commands=num_pipe+1
      cmds[i]=(char **)calloc(1024,sizeof(char *)); 
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
               return NULL; // if input is either '\n' or '\0'
            }
            break;  
         }
         cmds[i][k]=Token->pcValue;
         k++;
      }
   }
   return cmds;
}

/*--------------------------------------------------------------------*/

static void quitHandler(int isig)
/* signal handler of SIGQUIT */
{  
   printf("\nType Ctrl-\\ again within 5 seconds to exit.\n");
   alarm(5);
   signal(SIGQUIT,SIG_DFL);
}

/*--------------------------------------------------------------------*/

static void alarmHandler(int isig)
/* signal handler of SIGALRM */
{
   signal(SIGQUIT,quitHandler);
}

/*--------------------------------------------------------------------*/

int exc1_Line(char ***cmds)
/*
  Execute when there is no pipe redirection. 
  Return True if it is successful, otherwise False.
*/
{
   if(NULL==cmds) return TRUE;

   char *name=cmds[0][0];
   int result;

   /* check whether it is built-in command */
   //1)sentenv
   if(strcmp(name,"setenv")==0){
      if(cmds[0][3]!=NULL){
         fprintf(stderr,"./ish: setenv takes one or two parameters\n");
         return FALSE;
      }
      else if(cmds[0][1]==NULL){
         fprintf(stderr,"./ish: setenv takes one or two parameters\n");
         return FALSE;
      }
      else if(cmds[0][2]==NULL){
         result=setenv(cmds[0][1],"",1);
         if(result==-1){
            fprintf(stderr,"./ish: setenv failed\n");
            return FALSE;
         }
      }
      else{
         if(setenv(cmds[0][1],cmds[0][2],1)==-1){
            fprintf(stderr,"./ish: setenv failed\n");
            return FALSE;
         }
         return TRUE;
      }
   }
   //2)unsentenv
   else if(strcmp(name,"unsetenv")==0){
      if(cmds[0][2]!=NULL){
         fprintf(stderr,"./ish: unsetenv takes one parameter\n");
         return FALSE;
      }
      else if(cmds[0][1]==NULL){
         fprintf(stderr,"./ish: unsetenv takes one parameter\n");
         return FALSE;
      }
      else{
         if(unsetenv(cmds[0][1])==-1){
            fprintf(stderr,"./ish: unsetenv failed\n");
            return FALSE;
         }
         return TRUE;
      }
   }
   //3)cd
   else if(strcmp(name,"cd")==0){
      if(cmds[0][2]!=NULL){
         fprintf(stderr,"./ish: cd takes one parameter\n");
         return FALSE;
      }
      
      else if(cmds[0][1]==NULL){
         struct passwd *pw = getpwuid(getuid());
         const char *homedir = pw->pw_dir;
         if(chdir(homedir)<0){
            fprintf(stderr,"cd failed\n");
            return FALSE;
         }
         return TRUE;
      }
      
      else{
         if(chdir(cmds[0][1])<0){
            /* cd failed */
            fprintf(stderr,"./ish: No such file or directory\n");
            return FALSE;
         }
         return TRUE;
      }
   }
   //4)exit
   else if(strcmp(name,"exit")==0){
      if(cmds[0][1]!=NULL){
         fprintf(stderr,"./ish: exit does not take any parameters\n");
         return FALSE;
      }
      exit(0);
   }

   /* Not Built-in command */
   else{
      fflush(NULL);

      int pid=fork(),status;
      if(pid==0){
         /* in child */
         signal(SIGQUIT,SIG_DFL);

         execvp(cmds[0][0],cmds[0]);
         fprintf(stderr, "%s: No such file or directory\n",cmds[0][0]);
         exit(EXIT_FAILURE);
      }
      /* in parent */
      pid = wait(&status);
   }
   return TRUE;
}

/*--------------------------------------------------------------------*/

int exc2_Line(char ***cmds,int num_pipe)
/*
  Execute when there are at least one pipe redirection. 
  Return True if it is successful, otherwise False.
*/
{
   /* check whether there is file redirection with built-in command */
   char *name=cmds[0][0];
   if(!strcmp(name,"setenv")||!strcmp(name,"unsetenv")||!strcmp(name,"cd")||!strcmp(name,"exit"))
   {
      fprintf(stderr,"./ish: buit-in command redirection error\n");
      return FALSE;
   }

   /* not built-in command */
   int i;
   int **p=(int **)calloc(num_pipe+1,sizeof(int *));
   if(NULL==p){
      fprintf(stderr,"./ish: Memory allocation error!!\n");
      return FALSE;
   }

   for(i=0;i<num_pipe+1;i++){
      p[i]=(int *)calloc(2,sizeof(int));
      if(NULL==p[i]){
         fprintf(stderr,"./ish: Memory allocation error!!\n");
         return FALSE;
      }
   }

   for(i=0;i<num_pipe+1;i++){
      int pid,status;

      if(i<num_pipe){
         if (pipe(p[i]) == -1) exit(1);
      }
      fflush(NULL);
      pid=fork();
      
      if(pid<0){ 
         fprintf(stderr,"./ish: fork failed\n");
      }
      else if(pid==0){ /* child process */
         signal(SIGQUIT,SIG_DFL);

         if(i>0){ 
            close(p[i-1][1]);
            dup2(p[i-1][0],0);
            close(p[i-1][0]); 
         }
         if(i!=num_pipe){
            dup2(p[i][1],1);
         }
         execvp(cmds[i][0],cmds[i]);
         fprintf(stderr, "exec failed\n");
         exit(EXIT_FAILURE);
      }

      else{ /* parent process */   
         if(i>0){
            close(p[i-1][0]);
            close(p[i-1][1]);
         }
         pid = wait(&status);
      }
   }
   for(i=0;i<num_pipe+1;i++) free(p[i]);
   free(p);
   return TRUE;
}
/*--------------------------------------------------------------------*/

void surpress_unusedVariableError(void (*pfret)(int))
/* 
  I set this function to prevent error coming from unused variable: pfret.
  Pfret is declared at main function.
*/
{
}
/*--------------------------------------------------------------------*/

int main(void)

/* Read a line from stdin, and write to stdout each number and word
   that it contains.  Repeat until EOF.  Return 0 iff successful. */

{
   char acLine[MAX_LINE_SIZE];
   DynArray_T oTokens;
   int num_pipe;

   /* make sure signals are not blocked */
   sigset_t sSet;
   sigemptyset(&sSet);
   sigaddset(&sSet, SIGINT);
   sigaddset(&sSet, SIGQUIT);
   sigaddset(&sSet, SIGALRM);
   sigprocmask(SIG_UNBLOCK, &sSet, NULL);

   char ***cmds;
   void (*pfret)(int);

   /* handle signals */
   pfret=signal(SIGINT, SIG_IGN);  
   assert(pfret!=SIG_ERR);   
   pfret=signal(SIGQUIT, quitHandler);
   assert(pfret!=SIG_ERR);
   pfret=signal(SIGALRM, alarmHandler);
   assert(pfret!=SIG_ERR);

   surpress_unusedVariableError(pfret);
   
   /* operate with commands from ./ishrc */

   char *pathname=getenv("HOME");
   strcat(pathname,"/.ishrc");
   
   FILE *fp=fopen(pathname,"r");
   if(fp==NULL) goto STDIN;

   while (fgets(acLine, MAX_LINE_SIZE, fp)!=NULL)
   {
      printf("%% ");
      //if(fgets(acLine, MAX_LINE_SIZE, fp) == NULL) break;
      printf("%s",acLine);
      oTokens = DynArray_new(0);
      if (oTokens == NULL)
      {
         fprintf(stderr, "Cannot allocate memory\n");
         exit(EXIT_FAILURE);
      }
      lexLine(acLine, oTokens);
   
      num_pipe = synLine(oTokens);
      if(num_pipe>=0) 
      {
         cmds=make_Cmd(oTokens,num_pipe);
         if(num_pipe==0) exc1_Line(cmds);
         else exc2_Line(cmds,num_pipe);
      }

      DynArray_map(oTokens, freeToken, NULL);
      DynArray_free(oTokens);
   }

   /*--------------------------------------------------------------------*/

   /* operate with commands from stdin */
STDIN:
   while (1)
   {
      printf("%% ");
      if(fgets(acLine, MAX_LINE_SIZE, stdin) == NULL) break;

      oTokens = DynArray_new(0);
      if (oTokens == NULL)
      {
         fprintf(stderr, "Cannot allocate memory\n");
         exit(EXIT_FAILURE);
      }

      lexLine(acLine, oTokens);
      
      num_pipe = synLine(oTokens);
      if(num_pipe>=0) 
      {
         cmds=make_Cmd(oTokens,num_pipe);
         if(num_pipe==0) exc1_Line(cmds);
         else exc2_Line(cmds,num_pipe);
      }

      DynArray_map(oTokens, freeToken, NULL);
      DynArray_free(oTokens);
   }

   return 0;
}