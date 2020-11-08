/* 20190617 조동올, assignment 3, customer_manager2.c */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h> //to use pow()
#include "customer_manager.h"

/*----------------------------------------------------------------------
Uncomment and use the following code if you want*/

enum {HASH_MULTIPLIER = 65599, UNIT_BUCKET_SIZE=1024};

typedef struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;           
  int id_hash;               // undivided hash value
  int name_hash;             // undivided hash value
  struct UserInfo *id_next;
  struct UserInfo *name_next;
  struct UserInfo *next;
}USERINFO;

struct DB {
    int curBuckSize;
    USERINFO **id_bucket;
    USERINFO **name_bucket;
    USERINFO *first;
    int count;
    int max_size;
};

static int hash(const char *pcKey) 
/* modified hash function */
{
   int i;
   unsigned int uiHash = 0U;
   for (i = 0; pcKey[i] != '\0'; i++)
      uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
               + (unsigned int)pcKey[i];
   return (int)uiHash ;
}

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
  d->curBuckSize=UNIT_BUCKET_SIZE;

  d->id_bucket=(USERINFO **)calloc(d->curBuckSize,sizeof(USERINFO *));
  if (d->id_bucket == NULL) {
    fprintf(stderr, "Can't allocate a memory for id_bucket of size %d\n",
	    d->curBuckSize);
    free(d);
    return NULL;
  }
  
  d->name_bucket=(USERINFO **)calloc(d->curBuckSize,sizeof(USERINFO *));  
  if (d->id_bucket == NULL) {
    fprintf(stderr, "Can't allocate a memory for name_bucket of size %d\n",
	    d->curBuckSize);
    free(d);
    return NULL;
  }
  
  d->first=NULL;

  return d;
}

void
DestroyCustomerDB(DB_T d)
{
  /* destory db and its associated memory */

  if(NULL!=d){
    USERINFO *p,*nextp;
    for(p=d->first;p!=NULL;p=nextp){
      nextp=p->next;
      free(p->id);
      free(p->name);
      free(p);
    }
    free(d->id_bucket);
    free(d->name_bucket);
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

  USERINFO *p;
  int id_hash=hash(id);    //undivided hash value: 

  for(p=d->id_bucket[id_hash&(d->curBuckSize-1)];p!=NULL;p=p->id_next){
      if(id_hash==p->id_hash&&strcmp(p->id,id)==0){
          fprintf(stderr,"Same id exists\n");
          return (-1);
      }
  } 

  int name_hash=hash(name); //undivided hash value

  for(p=d->name_bucket[name_hash&(d->curBuckSize-1)];p!=NULL;p=p->name_next){
      if(name_hash==p->name_hash&&strcmp(p->name,name)==0){
          fprintf(stderr,"Same name exists\n");
          return (-1);
      }
  } 

  p=(USERINFO *)calloc(1,sizeof(USERINFO));

  p->id=strdup(id);
  p->name=strdup(name);
  p->purchase=purchase;
  p->id_hash=id_hash;
  p->name_hash=name_hash;

  p->id_next=d->id_bucket[id_hash&(d->curBuckSize-1)];
  d->id_bucket[id_hash&(d->curBuckSize-1)]=p;

  p->name_next=d->name_bucket[name_hash&(d->curBuckSize-1)];   
  d->name_bucket[name_hash&(d->curBuckSize-1)]=p;

  p->next=d->first;
  d->first=p;
  d->count++;

  /* If more than 75% full, reset value after expansion */
  
  if(d->max_size==0&&d->count>=0.75*d->curBuckSize){  
      d->curBuckSize*=2;

      d->id_bucket=(USERINFO **)realloc(d->id_bucket,
                    sizeof(USERINFO *)*d->curBuckSize);
      memset(d->id_bucket,0,sizeof(USERINFO *)*(d->curBuckSize));

      d->name_bucket=(USERINFO **)realloc(d->name_bucket,
                    sizeof(USERINFO *)*d->curBuckSize);
      memset(d->name_bucket,0,sizeof(USERINFO *)*(d->curBuckSize)); 

      /*Re-set*/
      USERINFO *ptr;
      for(ptr=d->first;ptr!=NULL;ptr=ptr->next){
        ptr->id_next=d->id_bucket[ptr->id_hash&(d->curBuckSize-1)];
        d->id_bucket[ptr->id_hash&(d->curBuckSize-1)]=ptr;

        ptr->name_next=d->name_bucket[ptr->name_hash&(d->curBuckSize-1)];
        d->name_bucket[ptr->name_hash&(d->curBuckSize-1)]=ptr;  
      } 
      if(pow(2,20)==d->curBuckSize) d->max_size=1;
  }

  return 0;
  
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
  if(NULL==d||NULL==id) return (-1);
  
  int h1=hash(id),name_hash,count=0,n=0;
  char *name;
  USERINFO *p,*q;
  
  /* Handle the case when d->first has the same id */
  if(d->first&&d->first->id_hash==h1&&strcmp(d->first->id,id)==0){
    name_hash=d->first->name_hash;
    free(d->first->id);
    d->first->id=NULL;
    free(d->first->name);
    d->first->name=NULL;
    d->first=d->first->next;
  
    d->id_bucket[h1&(d->curBuckSize-1)]=
                    d->id_bucket[h1&(d->curBuckSize-1)]->id_next;
    d->name_bucket[name_hash&(d->curBuckSize-1)]=
                    d->name_bucket[name_hash&(d->curBuckSize-1)]->name_next;             
    return 0;
  }

  p=(USERINFO *)malloc(sizeof(USERINFO));
  p->next=d->first;
  USERINFO *pf0=p;
  
  for(;p->next!=NULL;p=p->next){
    if(p->next->id_hash==h1&&strcmp(p->next->id,id)==0){
      count++;
      p->next=p->next->next;  
      n=1;
    }
    if(n==1) break;
  }
  if(n==0) {
    fprintf(stderr,"NO such id exists\n");
    return (-1);
  }
  free(pf0);
  

  p=(USERINFO *)malloc(sizeof(USERINFO));
  p->id_next=d->id_bucket[h1&(d->curBuckSize-1)];
  USERINFO *pf1=p;

  for(;p->id_next!=NULL;p=p->id_next){
    if(p->id_next->id!=NULL);
    if(p->id_next->id_hash==h1&&strcmp(p->id_next->id,id)==0){
      count++;
      free(p->id_next->id);
      p->id_next->id=NULL;

      name=p->id_next->name;
      name_hash=p->id_next->name_hash;
      p->id_next=p->id_next->id_next;
      n=2;
    }
    if(n==2) break;
  }
  free(pf1);


  p=(USERINFO *)malloc(sizeof(USERINFO));
  p->name_next=d->name_bucket[name_hash&(d->curBuckSize-1)];
  USERINFO *pf2=p;

  for(;p->name_next!=NULL;p=p->name_next){
    if(p->name_next->name!=NULL);
    if(name_hash==p->name_next->name_hash&&strcmp(name,p->name_next->name)==0){
      count++;
      free(p->name_next->name);
      p->name_next->name=NULL;

      q=p->name_next;
      p->name_next=p->name_next->name_next;
      free(q);
      n=3;
    }
    if(n==3) break;
  }
  free(pf2);
  
  if(count==3)  return 0;

  fprintf(stderr,"reaches end of the function\n");
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
  if(NULL==d||NULL==name) return (-1);
  
  int h1=hash(name),id_hash,count=0,n=0;
  char *id;
  USERINFO *p,*q;

  /* Handle the case when d->first has the same name */
  if(d->first&&d->first->name_hash==h1&&strcmp(d->first->name,name)==0){
    id_hash=d->first->id_hash;
    free(d->first->name);
    d->first->name=NULL;
    free(d->first->id);
    d->first->id=NULL;
    d->first=d->first->next;
  
    d->name_bucket[h1&(d->curBuckSize-1)]=
                    d->name_bucket[h1&(d->curBuckSize-1)]->name_next;
    d->id_bucket[id_hash&(d->curBuckSize-1)]=
                    d->id_bucket[id_hash&(d->curBuckSize-1)]->id_next;             
    return 0;
  }

  p=(USERINFO *)malloc(sizeof(USERINFO));
  p->next=d->first;
  USERINFO *pf0=p;

  for(;p->next!=NULL;p=p->next){
    if(p->next->name!=NULL);
    if(p->next->name_hash==h1&&strcmp(p->next->name,name)==0){
      count++;
      p->next=p->next->next;
      n=1;
    }
    if(n==1) break;
  }
  if(n==0) {
    fprintf(stderr,"NO such name exists\n");
    return (-1);
  }
  free(pf0);


  p=(USERINFO *)malloc(sizeof(USERINFO));
  p->name_next=d->name_bucket[h1&(d->curBuckSize-1)];
  USERINFO *pf1=p;

  for(;p->name_next!=NULL;p=p->name_next){
    if(p->name_next->name_hash==h1&&strcmp(p->name_next->name,name)==0){
      count++;
      free(p->name_next->name);
      p->name_next->name=NULL;
      
      id=p->name_next->id;
      id_hash=p->name_next->id_hash;
      p->name_next=p->name_next->name_next;
      n=2;
    }
    if(n==2) break;
  }
  free(pf1);


  p=(USERINFO *)malloc(sizeof(USERINFO));
  p->id_next=d->id_bucket[id_hash&(d->curBuckSize-1)];
  USERINFO *pf2=p;

  for(;p->id_next!=NULL;p=p->id_next){
      if(id_hash==p->id_next->id_hash&&strcmp(id,p->id_next->id)==0){
      count++;
      free(p->id_next->id);
      p->id_next->id=NULL;

      q=p->id_next;
      p->id_next=p->id_next->id_next;
      free(q);
      n=3;
    }
    if(n==3) break;
  }
  free(pf2);


  if(count==3)  return 0;

  fprintf(stderr,"reaches end of the function\n");
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
  if(NULL==d||NULL==id) return (-1);
  int h=hash(id);
  USERINFO *p;
  for(p=d->id_bucket[h&(d->curBuckSize-1)];p!=NULL;p=p->id_next){
      if(h==p->id_hash&&strcmp(id,p->id)==0){
          return p->purchase;
      }
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
  if(NULL==d||NULL==name) return (-1);
  int h=hash(name);
  USERINFO *p;
  for(p=d->name_bucket[h&(d->curBuckSize-1)];p!=NULL;p=p->name_next){
      if(h==p->name_hash&&strcmp(name,p->name)==0){
          return p->purchase;
      }
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
  
  int sum=0; 
  USERINFO *p;
  for(p=d->first;p!=NULL;p=p->next){
    sum+=(*fp)(p->id,p->name,p->purchase);
  }
  return sum;
  assert(0);
}
