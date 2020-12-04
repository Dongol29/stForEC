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

enum TokenType {TOKEN_WORD, TOKEN_PIPE};

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

    int i=0;
    for (;;)
    {
        Token=(struct Token *)DynArray_get(oTokens,i);
        type=Token->eType;

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
                    return FALSE;
                }
                else  //이 때 더이상 pstoken없는게 맞나?
                {
                    return TRUE;
                }
                break;
            case STATE_WORD:
                if(type==TOKEN_WORD)
                {
                    eState=STATE_WORD;
                }
                else if(type==TOKEN_PIPE)
                {
                    eState=STATE_PIPE;
                }
                else //이 때 더이상 pstoken없는게 맞나?
                {
                    return TRUE;
                }
                break;
            case STATE_PIPE:
                if(type=TOKEN_WORD) //count++해야하나?
                {
                    eState=STATE_WORD;
                }
                else 
                {
                    fprintf(stderr,"./ish: Pipe or redirection destination not specified\n");
                    return FALSE;
                }
                break;
            default:
                assert(FALSE);
        }  
        i++;
    }
}





/*--------------------------------------------------------------------*/

int main(void)

/* Read a line from stdin, and write to stdout each number and word
   that it contains.  Repeat until EOF.  Return 0 iff successful. */

{
   char acLine[MAX_LINE_SIZE];
   DynArray_T oTokens;
   int iSuccessful;

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

          if(synLine(oTokens))
          {
              printf("Valid\n");
          }
      }
      printf("------------------------------------\n");

      DynArray_map(oTokens, freeToken, NULL);
      DynArray_free(oTokens);
   }

   return 0;
}