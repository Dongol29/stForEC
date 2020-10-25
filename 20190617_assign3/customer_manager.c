#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

/*----------------------------------------------------------------------
Uncomment and use the following code if you want*/

#define UNIT_ARRAY_SIZE 64
#define NDEBUG

typedef struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
}USERINFO;

struct DB {
  struct UserInfo *pArray;   // pointer to the array
  int curArrSize;            // current array size (max # of elements)
  int numItems;              // # of stored items, needed to determine
	//int count;		     // # whether the array should be expanded  count: element 추가 시마다
	int time;   // # or not      time: expand 횟수
};

/*----------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
DB_T
CreateCustomerDB(void)
{
  /* Uncomment and use the following implementation if you want*/
  DB_T d;

  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->curArrSize = UNIT_ARRAY_SIZE; // start with 64 elements
  d->pArray = (USERINFO *)calloc(d->curArrSize,
               sizeof(USERINFO));
  if (d->pArray == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n",
	    d->curArrSize);
    free(d);
    return NULL;
  }
  return d;
}
/*--------------------------------------------------------------------*/
void
DestroyCustomerDB(DB_T d)
{
  int i;
  if(NULL!=d){
    for(i=0;i<d->curArrSize;i++){
      if(d->pArray[i].name==NULL) continue;
      free(d->pArray[i].name);
      free(d->pArray[i].id);
    }
    free(d->pArray);
    free(d);
  }
}
/*--------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
  /* 1. parameter validation 2. array에서 empty element 찾고 store data
     2-1. copy를 저장 3. full->realloc으로 expand(64)*/
  printf("hola0\n");
  if(NULL==d||NULL==id||NULL==name||purchase<=0) return (-1); //1,2

  int i;
  for(i=0;i<d->curArrSize;i++){
    printf("hola00\n");
    if(!strcmp((d->pArray[i]).id,id)||!strcmp((d->pArray[i]).name,name)){
        printf("hola1\n");
        return (-1);
    }
  } 

  /* empty 판별-by checking each name */
  for(i=0;i<d->curArrSize;i++){
    if((d->pArray[i]).name==NULL) {
      printf("hola2\n");
      /* 3 */  
      (d->pArray[i]).name=strdup(name);
      //(d->pArray[i]).name=(char *)malloc(strlen(name)+1);  
      //strcpy((d->pArray[i]).name,name);
      //(d->pArray[i]).id=(char *)malloc(strlen(id)+1);
      //strcpy((d->pArray[i]).id,id);
      printf("hola3\n");
      (d->pArray[i]).id=strdup(id);
      printf("hola4\n");
      (d->pArray[i]).purchase=purchase;
      //d->count++;

      return 0; //추가성공
    }
  }
  // 다 차있어서-->추가못한상태
  printf("hola5\n");
  d->time++; //첫 추가 시 time=1
  d->pArray=(USERINFO *)realloc(d->pArray,sizeof(USERINFO)*UNIT_ARRAY_SIZE*(d->time+1));
  memset(d->pArray+64*(d->time),0,UNIT_ARRAY_SIZE*sizeof(USERINFO)); 
  d->curArrSize=UNIT_ARRAY_SIZE*(d->time+1);
  /*이 때 i==d->curArraysize*/
  //(d->pArray[i]).name=(char *)malloc(strlen(name)+1);  
  //strcpy((d->pArray[i]).name,name);
  (d->pArray[i]).name=strdup(name);
  //(d->pArray[i]).id=(char *)malloc(strlen(id)+1);
  //strcpy((d->pArray[i]).id,id);
  d->pArray[i].id=strdup(id);
  d->pArray[i].purchase=purchase;

  return 0;
            
  assert(0);
}
/*--------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  /*1. deallocate 2. name=NULL */
  int i;
  if(NULL==d||NULL==id) return (-1);

  for(i=0;i<d->curArrSize;i++){
    if(strcmp((d->pArray[i]).id,id)==0){
      free(d->pArray[i].name);
      free(d->pArray[i].id);

      d->pArray[i].name=NULL;  //struct로 이루어진 array
    }
  }
  return (-1); // 존재x
}

/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  int i;
  if(NULL==d||NULL==name) return (-1);

  for(i=0;i<d->curArrSize;i++){
    if(strcmp((d->pArray[i]).name,name)==0){
      free(d->pArray[i].name);
      free((d->pArray[i]).id);

      d->pArray[i].name=NULL;  //struct로 이루어진 array
    }
  }
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
  int i;
  if(NULL==d||NULL==id) return (-1);

  for(i=0;i<d->curArrSize;i++){
    if(strcmp((d->pArray[i]).id,id)==0){
      return d->pArray[i].purchase;
    }
  }
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{
  
  int i;
  if(NULL==d||NULL==name) return (-1);

  for(i=0;i<d->curArrSize;i++){
    if(strcmp((d->pArray[i]).name,name)==0){
      return d->pArray[i].purchase;
    }
  }
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  if(NULL==d||NULL==fp) return (-1);
  
  int i,sum=0; //구조체 따로 저장?
  for(i=0;i<d->curArrSize;i++){
    if(NULL!=d->pArray[i].name) {
      sum+=(*fp)(d->pArray[i].id,d->pArray[i].name,
                 d->pArray[i].purchase); }
  }
  return sum;
  assert(0);
}
