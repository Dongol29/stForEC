/* 20190617 조동올, assignment 3, customer_manager1.c */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"


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
	int time;                  // number of expansions
};

DB_T
CreateCustomerDB(void)
{
  /*
     create and return a db structure
     return NULL if fails to allocate the memory 
  */

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

void
DestroyCustomerDB(DB_T d)
{
  /* destory db and its associated memory */

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

int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
  /* 
    register a customer with (name, id, purchase)
    on success, return 0. Otherwise, return (-1) 
  */
  if(NULL==d||NULL==id||NULL==name||purchase<=0) return (-1); 

  int i;
  for(i=0;i<d->curArrSize;i++){
    if(d->pArray[i].name!=NULL){
      if(!strcmp((d->pArray[i]).id,id)||!strcmp((d->pArray[i]).name,name)){
        return (-1);
    }}
  } 

  /* Find which element is empty by checking each element's name */
  for(i=0;i<d->curArrSize;i++){
    if((d->pArray[i]).name==NULL) {
      (d->pArray[i]).name=strdup(name);
      (d->pArray[i]).id=strdup(id);
      (d->pArray[i]).purchase=purchase;
      return 0; 
    }
  }
  
  /* Need expansion */
  d->time++;  
  d->pArray=(USERINFO *)realloc(d->pArray,sizeof(USERINFO)*UNIT_ARRAY_SIZE*(d->time+1));
  if(d->pArray==NULL) {
    fprintf(stderr,"Allocation error in realloc\n");
    return (-1);
  }
  memset(d->pArray+64*(d->time),0,UNIT_ARRAY_SIZE*sizeof(USERINFO)); 
  d->curArrSize=UNIT_ARRAY_SIZE*(d->time+1);
  (d->pArray[i]).name=strdup(name);
  (d->pArray[i]).id=strdup(id);
  (d->pArray[i]).purchase=purchase;

  return 0;
            
  assert(0);
}

int
UnregisterCustomerByID(DB_T d, const char *id)
{
  /* 
    unregister a customer with 'id'
    on success, return 0. Otherwise(failure), return (-1) 
    1) If d or id is NULL, it is a failure. 
    2) If no such item exists, it is a failure. 
  */
  int i;
  if(NULL==d||NULL==id) return (-1);

  for(i=0;i<d->curArrSize;i++){
    if(d->pArray[i].name!=NULL){
      if(strcmp((d->pArray[i]).id,id)==0){
        free(d->pArray[i].name);
        free(d->pArray[i].id);

        d->pArray[i].name=NULL;   // For checking states of elements
        return 0;
    }}
  }
  return (-1); 
}


int
UnregisterCustomerByName(DB_T d, const char *name)
{
  /* 
    unregister a customer with 'name'
    on success, return 0. Otherwise(failure), return (-1)
    1) If d or name is NULL, it is a failure. 
    2) If no such item exists, it is a failure. 
  */
  int i;
  if(NULL==d||NULL==name) return (-1);

  for(i=0;i<d->curArrSize;i++){
    if(d->pArray[i].name!=NULL){
      if(strcmp((d->pArray[i]).name,name)==0){
        free(d->pArray[i].name);
        free((d->pArray[i]).id);

        d->pArray[i].name=NULL;  // For checking states of elements
        return 0;
    }}
  }
  return (-1);
}

int
GetPurchaseByID(DB_T d, const char* id)
{
  /* 
    get the purchase amount of a user whose ID is 'id' 
    On success, it should return the purchase amount.
    Otherwise, it should return -1
    1) If d or id is NULL, it is a failure. 
    2) If there is no customer whose ID matches the given one,
    it is a failure. 
  */
  int i;
  if(NULL==d||NULL==id) return (-1);

  for(i=0;i<d->curArrSize;i++){
    if(d->pArray[i].name!=NULL){
      if(strcmp((d->pArray[i]).id,id)==0){
        return d->pArray[i].purchase;
    }}
  }
  return (-1);
}

int
GetPurchaseByName(DB_T d, const char* name)
{
  /* 
    get the purchase amount of a user whose name is 'name' 
    On success, it should return the purchase amount.
    Otherwise, it should return -1
    1) If d or name is NULL, it is a failure. 
    2) If there is no customer whose name matches the given one,
    it is a failure. 
  */
  int i;
  if(NULL==d||NULL==name) return (-1);

  for(i=0;i<d->curArrSize;i++){
    if(d->pArray[i].name!=NULL){
      if(strcmp((d->pArray[i]).name,name)==0){
        return d->pArray[i].purchase;
    }}
  }
  return (-1);
}

int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  /* 
    iterate all valid user items once, evaluate fp for each valid user
    and return the sum of all fp function calls.
    On success, GetCustomerPurchase should return the sum of all numbers 
    returned by fp by iterating each user item in d.
    Otherwise,if d or fp is NULL, it should return -1. 
  */
  if(NULL==d||NULL==fp) return (-1);
  
  int i,sum=0; 
  for(i=0;i<d->curArrSize;i++){
    if(NULL!=d->pArray[i].name) {
      sum+=(*fp)(d->pArray[i].id,d->pArray[i].name,
                 d->pArray[i].purchase); }
  }
  return sum;
  assert(0);
}
