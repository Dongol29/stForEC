/* 20190617 조동올, assignment 2, str.h */

#ifndef _STR_H_
#define _STR_H_
#include <unistd.h> /* for typedef of size_t */

/* Part 1 */
size_t StrGetLength(const char* pcSrc);

char *StrCopy(char *pcDest, const char* pcSrc);
/*
   This function copies the string pointed to by pcSrc, including the
  terminating null byte ('\0'), to the buffer pointed to by pcDest. 
   The strings may not overlap, and the destination string dest must 
  be large enough to receive the copy.
   Return value is a pointer to the starting destination of buffer.
   Assert that each parameter is not NULL.
  */

int StrCompare(const char* pcS1, const char* pcS2);
/*
   This function compares the two strings pointed to by 
  pcS1 and pcS2.  
   The return value is an integer less than, equal to, or greater
  than zero if string 1 is found, respectively, to be less than, 
  to match, or be greater than string 2.
   Assert that each parameter is not NULL.
*/
char *StrSearch(const char* pcHaystack, const char *pcNeedle);
/* 
   This function finds the first occurrence of the substring 
  pointed to by pcNeedle in the string pointed to by pcHaystack.
  The terminating null bytes ('\0') are not compared.
   Returns a pointer to the beginning of the substring,  or  
  NULL  if  the substring is not found. 
   Returns haystack when needle(substring) is empty.
   Assert that each parameter is not NULL.
*/ 
char *StrConcat(char *pcDest, const char* pcSrc);
/*
   This function  appends  the src string to the dest string, 
  overwriting the terminating null byte ('\0') at the end of dest,
  and then adds  a  terminating  null byte. 
   Each string is pointed to by pcDest and pcSrc.
   The  strings may not overlap, and the dest string must have 
  enough space for the result.
   Returns a pointer to the resulting string dest.
   Assert that each parameter is not NULL.
*/
#endif /* _STR_H_ */