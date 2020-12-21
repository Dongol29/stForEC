//Donggeon Kim, Assignment5, ish.c
#include "dynarray.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pwd.h>

/*--------------------------------------------------------------------*/

enum {MAX_LINE_SIZE = 1024};

enum {FALSE, TRUE};

//When Token is not |, TOKEN_NORMAL
//when Token is |, TOKEN_SPECIAL
enum TokenType {TOKEN_NORMAL, TOKEN_SPECIAL};

/*--------------------------------------------------------------------*/

/* A Token is either a number or a word, expressed as a string. */

struct Token
{
  enum TokenType eType;
  /* The type of the token. */

  char *pcValue;
  /* The string which is the token's value. */
};

/*--------------------------------------------------------------------*/

struct Command

{
  // The string which is the name of command. ex)setenv, cd, etc
  char *command_name;

  /*
    The dynamic array that stores command in the first storage and
    each arguments in the storage sequentially, and null in the last
    storage
  */
  DynArray_T oArguments;
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

static void freeCommand(void *pvItem, void *pvExtra)

/* Free command pvitem. pvExtra is unused. */
{
  struct Command *psCommand = (struct Command*)pvItem;
  DynArray_free(psCommand->oArguments);
  free(psCommand->command_name);
  free(psCommand);
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

static struct Command *makeCommand(char *command_name, DynArray_T oArguments)

/*
  Create and return a Command whose command name is command_name and
  whose oArguments consists of dynamic array oArguments. Return NULL if 
  insufficient memory is available.  The caller owns the Command.
*/
  
{
  struct Command *psCommand;
  psCommand = (struct Command*)malloc(sizeof(struct Command));
  if (psCommand == NULL)
    return NULL;
  psCommand->command_name = (char*)malloc(strlen(command_name) +1);
  if(psCommand->command_name == NULL)
    {
      free(psCommand);
      return NULL;
    }
  strcpy(psCommand->command_name, command_name);
  psCommand->oArguments = oArguments;
  return psCommand;
}

/*--------------------------------------------------------------------*/


static int synLine(DynArray_T oTokens, DynArray_T oCommands)

/* Syntatically analyze dynarray oTokens.  Populate oCommands with the
   commands that oTokens contains.  Return 1 (TRUE) if successful, or
   0 (FALSE) otherwise.  In the latter case, oCommands may contain
   commands that were discovered before the error. The caller owns the
   commands placed in oCommands. */
  
{
  enum SynState {STATE_COMMAND, STATE_ARGUMENT, STATE_SPECIAL};
  enum SynState eState = STATE_COMMAND;

  DynArray_T psArguments, oArguments;
  struct Command *psCommand;
  struct Token *psToken;
  char* c;
  int num, i, p = 0, tmp;
  int j;
  int k;

  while(DynArray_getLength(oTokens) != 0){
    psArguments = DynArray_new(0);
    num = DynArray_getLength(oTokens);
    psToken = DynArray_get(oTokens, 0);
    //check if first token is |
    if (psToken->eType == TOKEN_SPECIAL)
      {
	fprintf(stderr,
	    "./ish: Pipe or redirection destination not specified\n");
	return FALSE;
      }
    //check if last token is |
    psToken = DynArray_get(oTokens, num-1);
    if(psToken->eType == TOKEN_SPECIAL)
      {
	fprintf(stderr,
	    "./ish: Pipe or redirection destination not specified\n");
	return FALSE;
      }
    //check if there is no command between two |
    //p is 0 when the i-1th token is normal
    //p is 1 when the i-1th token is |
    for (i=0; i<num; i++)
      {
	psToken = DynArray_get(oTokens, i);
	if(psToken->eType == TOKEN_SPECIAL)
	  {
	    if(p==1)
	      {
		fprintf(stderr,
	    "./ish: Pipe or redirection destination not specified\n");
		return FALSE;
	      }
	    p = 1;
	  }
	else
	  p = 0;
      }
    
    //since there is no pipe error, make oCommands
    for(i=0 ; i<num; i++)
      {
	psToken = DynArray_get(oTokens, i);
	switch(eState)
	  {
	    /*
	      When it is the first token, initialize psCommand
	      and add the pcValue of token in psArguments
	      change eState to State_Argument
	    */
	  case STATE_COMMAND:
	    c = psToken->pcValue;
	    psCommand = makeCommand(c, psArguments);
	    DynArray_add(psArguments, c);
	    eState = STATE_ARGUMENT;
	    break;

	    /*
	      When it is the token that might be an argument, check if
	      the token is special or not. If token is special, change
	      eState to STATE_SPECIAL. Else, add the token's pcValue to
	      psArguments
	    */
	  case STATE_ARGUMENT:
	    if(psToken->eType == TOKEN_SPECIAL)
	      eState = STATE_SPECIAL;
	    else
	      DynArray_add(psArguments, psToken->pcValue);
	    break;

	    /*
	      When token was | in the last loop, add null to psArguments
	      and make a Command. Then, add the Command to oCommands.
	      Since the token in the current loop is the command name,
	      initialize psArguments and psCommand. Then change eState
	      to STATE_ARGUMENT
	    */
	  case STATE_SPECIAL:
	    DynArray_add(psArguments, '\0');
	    psCommand = makeCommand(c, psArguments);
	    if (! DynArray_add(oCommands, psCommand))
	      {
		fprintf(stderr,
		"./ish: Cannot allocate memory in syntatic analysis\n");
		return FALSE;
	      }
	    psArguments = DynArray_new(0);
	    c = psToken->pcValue;
	    DynArray_add(psArguments, c);
	    psCommand = makeCommand(c, psArguments);
	    eState = STATE_ARGUMENT;
	    break;
	    
	  default:
	    assert(FALSE);
	  }
      }

    /*
      The loop above does not make Command for the last command.
      This part of the code makes a Command for last command.
    */
    DynArray_add(psArguments, '\0');
    psCommand = makeCommand(c, psArguments);
    if (! DynArray_add(oCommands, psCommand))
      {
	fprintf(stderr,
	     "./ish: Cannot allocate memory in syntatic analysis\n");
	return FALSE;
      }

    return TRUE;
  }
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
  enum LexState {STATE_START, STATE_IN_NORMAL, STATE_IN_SPECIAL};

  enum LexState eState = STATE_START;

  int iLineIndex = 0;
  int iValueIndex = 0;
  int numofquo = 0;
  char c;
  char acValue[MAX_LINE_SIZE];
  struct Token *psToken;

  assert(pcLine != NULL);
  assert(oTokens != NULL);

  for (;;)
    {
      /* "Read" the next character from pcLine. */
      c = pcLine[iLineIndex++];

      //if the quotation is paired
      if(numofquo % 2 == 0)
	{
	  switch (eState)
	    {
	    case STATE_START:
	      if ((c == '\n') || (c == '\0'))
		return TRUE;
	      else if (c == '"')
		{
		  eState = STATE_START;
		  numofquo ++;
		}
	      else if (c == '|')
		{
		  acValue[iValueIndex++] = c;
		  eState = STATE_IN_SPECIAL;
		}
	      else if (c != '|')
		{
		  if ((c == ' ') || (c == '\t'))
		    eState = STATE_START;
		  else
		    {
		      acValue[iValueIndex++] = c;
		      eState = STATE_IN_NORMAL;
		    }
		}
		  break;
	    
	    case STATE_IN_NORMAL:
	      if(c == '"')
		  numofquo++;
	      else if ((c == '\n') || (c == '\0'))
		{
		  /* Create a normal token. */
		  acValue[iValueIndex] = '\0';
		  psToken = makeToken(TOKEN_NORMAL, acValue);
		  if (psToken == NULL)
		    {
		      fprintf(stderr,
	      "./ish: Cannot allocate memory in lexical analysis\n");
		      return FALSE;
		    }
		  if (! DynArray_add(oTokens, psToken))
		    {
		      fprintf(stderr,
	      "./ish: Cannot allocate memory in lexical analysis\n");
		      return FALSE;
		    }
		  iValueIndex = 0;
		  return TRUE;
		}
	      else if ((c == ' ') || (c == '\t'))
		{
		  /* Create a normal token. */
		  acValue[iValueIndex] = '\0';
		  psToken = makeToken(TOKEN_NORMAL, acValue);
		  if (psToken == NULL)
		    {
		      fprintf(stderr,
     "./ish: Cannot allocate memory in lexical analysis\n");
		      return FALSE;
		    }
		  if (! DynArray_add(oTokens, psToken))
		    {
		      fprintf(stderr,
		"./ish: Cannot allocate memory in lexical analysis\n");
		      return FALSE;
		    }
		  iValueIndex = 0;
		  
		  eState = STATE_START;
		}
	      else if (c == '|')
		{
		  acValue[iValueIndex] = '\0';
		  psToken = makeToken(TOKEN_NORMAL, acValue);
		  if (psToken == NULL)
		    {
		      fprintf(stderr,
		  "./ish: Cannot allocate memory in lexical analysis\n");
		      return FALSE;
		    }
		  if (! DynArray_add(oTokens, psToken))
		    {
		      fprintf(stderr,
		 "./ish: Cannot allocate memory in lexical analysis\n");
		      return FALSE;
		    }
		  iValueIndex = 0;
		  
		  acValue[iValueIndex++] = c;
		  eState = STATE_IN_SPECIAL;
		}
	      else
		{
		  acValue[iValueIndex++] = c;
		  eState = STATE_IN_NORMAL;
		}
	      break;
	      
	    case STATE_IN_SPECIAL:
	      if ((c == '\n') || (c == '\0'))
		{
		  /* Create a special token. */
		  acValue[iValueIndex] = '\0';
		  psToken = makeToken(TOKEN_SPECIAL, acValue);
		  if (psToken == NULL)
		    {
		      fprintf(stderr,
	       "./ish: Cannot allocate memory in lexical analysis\n");
		      return FALSE;
		    }
		  if (! DynArray_add(oTokens, psToken))
		    {
		      fprintf(stderr,
	       "./ish: Cannot allocate memory in lexical analysis\n");
		      return FALSE;
		    }
		  iValueIndex = 0;
		  
		  return TRUE;
		}
	      else
		{
		  /* Create a special token. */
		  acValue[iValueIndex] = '\0';
		  psToken = makeToken(TOKEN_SPECIAL, acValue);
		  if (psToken == NULL)
		    {
		      fprintf(stderr,
	       "./ish: Cannot allocate memory in lexical analysis\n");
		      return FALSE;
		    }
		  if (! DynArray_add(oTokens, psToken))
		    {
		      fprintf(stderr,
	       "./ish: Cannot allocate memory in lexical analysis\n");
		      return FALSE;
		    }
		  iValueIndex = 0;
		  
		  if ((c == ' ') || (c == '\t'))
		    eState = STATE_START;
		  else if (c == '|')
		    {
		      acValue[iValueIndex++] = c;
		      eState = STATE_IN_SPECIAL;
		    }
		  else if (c == '"')
		    {
		      numofquo ++;
		      eState = STATE_START;
		    }
		  else
		    {
		      acValue[iValueIndex++] = c;
		      eState = STATE_IN_NORMAL;
		    }
		}
	      break;
	      
	    default:
	      assert(FALSE);
	    }
	}
    
  
      //if quotation is not paired
      else
	{
	  switch (eState)
	    {
	    case STATE_START:
	      if ((c == '\n') || (c == '\0'))
		return TRUE;
	      else if (c == '"')
		{
		  eState = STATE_START;
		  numofquo ++;
		}
	      else if ((c == ' ') || (c == '\t'))
		eState = STATE_START;
	      else
		{
		  acValue[iValueIndex++] = c;
		  eState = STATE_IN_NORMAL;
		}
	      break;

	    case STATE_IN_NORMAL:
	      if ((c == '\n') || (c == '\0'))
		{
		  /* Create a normal token. */

		  fprintf(stderr, "./ish: Could not find quote pair\n");
		  return FALSE;
		}
	      else if (c == '"')
		{
		  numofquo ++;
		  eState = STATE_IN_NORMAL;
		}
	      else
		{
		  acValue[iValueIndex++] = c;
		  eState = STATE_IN_NORMAL;
		}
	      break;

	    default:
	      assert(FALSE);
	    }
	}
    }
}

/*--------------------------------------------------------------------*/

static void SignalHandler1 (int iSignal)
{
  exit(0);
}

/*--------------------------------------------------------------------*/

static void SignalHandler2 (int iSignal)
{
  void (*tmp) (int);
  printf("\nType Ctrl-\\ again within 5 seconds to exit.\n");
  alarm(5);
  tmp = signal(SIGQUIT, SignalHandler1);
  assert(tmp != SIG_ERR);

}

/*--------------------------------------------------------------------*/

static void SignalHandler3 (int iSignal)
{
  void (*tmp) (int);
  tmp = signal(SIGQUIT, SignalHandler2);
  assert (tmp != SIG_ERR);
}

/*--------------------------------------------------------------------*/

static int Execution(DynArray_T oCommands)
//In execution, we execute the commands in oCommands.
{
  int num;
  int status;
  int i;
  int j;
  pid_t pid;
  char* oArguments[1024];
  num = DynArray_getLength(oCommands);
  struct Command *psCommand;
  char *arg;
  int fds[num-1][2];
  int tmp[2];
  // void (*pfRet1) (int);
  //  void (*pfRet2) (int);


  for (i = 0; i < num; i++)
    {
      psCommand = (struct Command *)DynArray_get(oCommands, i);

      //if the command_name is built in command, execute the commands
      //else, fork a child process and use execvp.
      if (strcmp(psCommand->command_name, "setenv") == 0)
	{
	  //check if setenv takes one or two parameters
	  j = DynArray_getLength(psCommand->oArguments);
	  if(j < 3 || j > 4)
	    {
	      fprintf(stderr, "./ish: setenv takes one or two parameters\n");
	      return FALSE;
	    }
	  setenv((char *)DynArray_get(psCommand->oArguments, 1), (char *)DynArray_get(psCommand->oArguments, 2), 1);

	}
      else if (strcmp(psCommand->command_name, "unsetenv") == 0)
	{
	  //check if unsetenv takes one parameter
	  j = DynArray_getLength(psCommand->oArguments);
	  if(j != 3)
	    {
	      fprintf(stderr, "./ish: unsetenv takes one parameter\n");
	      return FALSE;
	    }
	  unsetenv((char *)DynArray_get(psCommand->oArguments, 1));

	}
      else if (strcmp(psCommand->command_name, "cd") == 0)
	{
	  j = DynArray_getLength(psCommand->oArguments);
	  if(j != 2 && j != 3)
	    {
	      //check if cd takes one or no parameter
	      fprintf(stderr, "./ish: cd takes one parameter\n");
	      return FALSE;
	    }
	  arg = (char *)DynArray_get(psCommand->oArguments, 1);
	  if(arg == NULL)
	    chdir(getpwuid(getuid())->pw_dir);
	  chdir(arg);

	}
      else if (strcmp(psCommand->command_name, "exit") == 0)
	{
	  //check if exit takes no parameter
	  j = DynArray_getLength(psCommand->oArguments);
	  if(j != 2)
	    {
	      //check if cd takes one parameter
	      fprintf(stderr, "./ish: cd takes one parameter\n");
	      return FALSE;
	    }	  
	  exit(0);
	}
      else //external commands
	{
	  //if not last command, create a pipe
	  if(i!=num-1)
	    if(pipe(fds[i]) == -1)
	      exit(-1);
	  fflush(NULL);
	  pid = fork();
	  if (pid == 0) //child
	    {
	      //in child, handle SIGINT with SignalHandler1
	      signal(SIGINT, SignalHandler1);

	      // handle previous pipe
	      if (i > 0) // unless it is the first command,
		{
		  dup2(fds[i-1][0], 0);
		  close(fds[i-1][0]);
		  close(fds[i-1][1]);
		}
	      
	      // handle next pipe
	      if (i != num-1) // unless it is the last command
		dup2(fds[i][1], 1);

	      DynArray_toArray(psCommand->oArguments, (void**) oArguments);
	      execvp(psCommand->command_name, oArguments);
	      fprintf(stderr, "%s: No such file or directory\n", psCommand->command_name);
	      exit(EXIT_FAILURE);
	    }
	  else{  //parent
	    if(i>0)
	      {
		close(fds[i-1][0]);
		close(fds[i-1][1]);
	      }
	    pid = wait(&status);
	  }
	}
    }
  return TRUE;
}



/*--------------------------------------------------------------------*/

int main(void)

/* Read a line from stdin, and write to stdout each number and word
   that it contains.  Repeat until EOF.  Return 0 iff successful. */

{
  char acLine[MAX_LINE_SIZE];
  DynArray_T oCommands;
  DynArray_T oTokens;
  int iSuccessful;
  void (*pfRet1) (int);
  void (*pfRet2) (int);
  void (*pfRet3) (int);
  sigset_t set;

  sigaddset(&set, SIGINT);
  sigaddset(&set, SIGQUIT);
  sigaddset(&set, SIGALRM);
  sigprocmask(SIG_UNBLOCK, &set, NULL);

  pfRet1 = signal(SIGINT, SIG_IGN);
  pfRet2 = signal(SIGQUIT, SignalHandler2);
  pfRet3 = signal(SIGALRM, SignalHandler3);
  assert(pfRet1 != SIG_ERR);
  assert(pfRet2 != SIG_ERR);
  assert(pfRet3 != SIG_ERR);

  printf("%% ");
  //read from stdin till eof
  while (fgets(acLine, MAX_LINE_SIZE, stdin) != NULL)
    {
      //initialize oTokens and oCommands
      oTokens = DynArray_new(0);
      oCommands = DynArray_new(0);
      if (oTokens == NULL)
	{
	  fprintf(stderr,
		  "./ish: Cannot allocate memory in lexical analysis\n");
	  exit(EXIT_FAILURE);
	}


      iSuccessful = lexLine(acLine, oTokens);
      if (iSuccessful) //if lexline success
	{
	  synLine(oTokens, oCommands);
	  Execution(oCommands);
	}

      printf("%% ");

      //free all memory
      DynArray_map(oTokens, freeToken, NULL);
      DynArray_free(oTokens);
      DynArray_map(oCommands, freeCommand, NULL);
      DynArray_free(oCommands);
    }

  return 0;
}
