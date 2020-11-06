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
  int id_hash;                  //나누기 안 한 hash값
  int name_hash;
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

static int hash(const char *pcKey) //ibucketcount 뻼
{
   int i;
   unsigned int uiHash = 0U;
   for (i = 0; pcKey[i] != '\0'; i++)
      uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
               + (unsigned int)pcKey[i];
   return (int)uiHash ;
}

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
/*--------------------------------------------------------------------*/
void
DestroyCustomerDB(DB_T d)
{
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
/*--------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
  if(NULL==d||NULL==id||NULL==name||purchase<=0) return (-1); //1,2

  USERINFO *p;
  /*if(NULL==p){
      fprintf(stderr,"Can't allocate a memory for p\n");
      return (-1);
  }*/

  int h_id_O=hash(id);    //나누기 안 한 해시 id결과값

  for(p=d->id_bucket[h_id_O&(d->curBuckSize-1)];p!=NULL;p=p->id_next){
    if(p->id!=NULL){
      if(h_id_O==p->id_hash&&strcmp(p->id,id)==0){
          fprintf(stderr,"Same id exists\n");
          return (-1);
      }
    }
  }       
  int h_name_O=hash(name); //origin값

  for(p=d->name_bucket[h_name_O&(d->curBuckSize-1)];p!=NULL;p=p->name_next){
    if(p->name!=NULL){
      if(h_name_O==p->name_hash&&strcmp(p->name,name)==0){
          fprintf(stderr,"Same name exists\n");
          return (-1);
      }}
  } 
  /*중복된 거 없다는 게 확인됨*/
  p=(USERINFO *)calloc(1,sizeof(USERINFO));

  p->id=strdup(id);
  p->name=strdup(name);
  p->purchase=purchase;
  p->id_hash=h_id_O;
  p->name_hash=h_name_O;

  p->id_next=d->id_bucket[h_id_O&(d->curBuckSize-1)];
  d->id_bucket[h_id_O&(d->curBuckSize-1)]=p;

  p->name_next=d->name_bucket[h_name_O&(d->curBuckSize-1)];   //
  d->name_bucket[h_name_O&(d->curBuckSize-1)]=p;

  p->next=d->first;
  d->first=p;
  d->count++;

  /* 75%이상 찼으면 expansion 후 재할당*/
  #ifdef TEST_FEATURE_X
  if(d->count>=0.75*d->curBuckSize&&d->max_size==0){  
      d->curBuckSize*=2;

      d->id_bucket=(USERINFO **)realloc(d->id_bucket,
                    sizeof(USERINFO *)*d->curBuckSize);
      memset(d->id_bucket,0,sizeof(USERINFO *)*(d->curBuckSize));

      d->name_bucket=(USERINFO **)realloc(d->name_bucket,
                    sizeof(USERINFO *)*d->curBuckSize);
      memset(d->name_bucket,0,sizeof(USERINFO *)*(d->curBuckSize));  //id table, name table의 포인터들 ==null
      /*재할당*/
      USERINFO *ptr;
      for(ptr=d->first;ptr!=NULL;ptr=ptr->next){

        //d->id_bucket[ptr->id_hash&(d->curBuckSize/2-1)]=NULL;
        //d->name_bucket[ptr->name_hash&(d->curBuckSize/2-1)]=NULL;

        ptr->id_next=NULL;
        ptr->name_next=NULL; //기존 두 해시 테이블의 연결 삭제
        
        ptr->id_next=d->id_bucket[ptr->id_hash&(d->curBuckSize-1)];
        d->id_bucket[ptr->id_hash&(d->curBuckSize-1)]=ptr;

        ptr->name_next=d->name_bucket[ptr->name_hash&(d->curBuckSize-1)];
        d->name_bucket[ptr->name_hash&(d->curBuckSize-1)]=ptr;  //새로 할당

      }
      
      if(pow(2,20)==d->curBuckSize) d->max_size=1;
  }
  #endif

  return 0;
  
}
/*--------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  /*1. deallocate 2. name=NULL */
  if(NULL==d||NULL==id) return (-1);
  

  int h1=hash(id),count=0;
  char *name;
  int name_hash,n=0;
  USERINFO *p,*q;

  
  if(d->first&&d->first->id_hash==h1&&strcmp(d->first->id,id)==0){
    name_hash=d->first->name_hash;
    free(d->first->id);
    free(d->first->name);
    d->first=d->first->next;

    //name_hash=d->id_bucket[h1&(d->curBuckSize-1)]->name_hash;
    //free(d->id_bucket[h1&(d->curBuckSize-1)]->id);   
    d->id_bucket[h1&(d->curBuckSize-1)]=
                    d->id_bucket[h1&(d->curBuckSize-1)]->id_next;

    //free(d->name_bucket[name_hash&(d->curBuckSize-1)]->name);  
    d->name_bucket[name_hash&(d->curBuckSize-1)]=
                    d->name_bucket[name_hash&(d->curBuckSize-1)]->name_next;             
    return 0;
  }

  p=(USERINFO *)malloc(sizeof(USERINFO));
  p->next=d->first;
  USERINFO *pf0=p;
  
  for(;p->next!=NULL;p=p->next){
    printf("sibal\n");
    if(p->next->id_hash==h1&&strcmp(id,p->next->id)==0){
      count++;
      p->next=p->next->next;  
      //printf("%d\n",p->next->next==NULL);
      //printf("%s\n",d->first->name);
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
    if(h1==p->id_next->id_hash&&strcmp(id,p->id_next->id)==0){
      count++;
      free(p->id_next->id);
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
    if(name_hash==p->name_next->name_hash&&strcmp(name,p->name_next->name)==0){
      count++;
      free(p->name_next->name);
      q=p->name_next;
      p->name_next=p->name_next->name_next;
      free(q);
      n=3;
    }
    if(n==3) break;
  }
  free(pf2);
  
  if(count==3)  return 0;

  assert(0);
}

/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  if(NULL==d||NULL==name) return (-1);
  
  int h1=hash(name),count=0;
  char *id;
  int id_hash,n=0;
  USERINFO *p,*q;

  if(d->first&&d->first->name_hash==h1&&strcmp(d->first->name,name)==0){
    id_hash=d->first->id_hash;
    printf("a");
    free(d->first->name);
    free(d->first->id);
    printf("b");
    d->first=d->first->next;

    //id_hash=d->name_bucket[h1&(d->curBuckSize-1)]->id_hash;
    //free(d->name_bucket[h1&(d->curBuckSize-1)]->name);   
    d->name_bucket[h1&(d->curBuckSize-1)]=
                    d->name_bucket[h1&(d->curBuckSize-1)]->name_next;

    //free(d->id_bucket[id_hash&(d->curBuckSize-1)]->id);  
    d->id_bucket[id_hash&(d->curBuckSize-1)]=
                    d->id_bucket[id_hash&(d->curBuckSize-1)]->id_next;             

    return 0;
  }

  p=(USERINFO *)malloc(sizeof(USERINFO));
  p->next=d->first;
  USERINFO *pf0=p;

  for(;p->next!=NULL;p=p->next){
    if(h1==p->next->name_hash&&strcmp(name,p->next->name)==0){
      count++;
      p->next=p->next->next;
      n=1;
    }
    if(n==1) break;
  }
  if(n==0) return (-1);

  free(pf0);

  p=(USERINFO *)malloc(sizeof(USERINFO));
  p->name_next=d->name_bucket[h1&(d->curBuckSize-1)];
  USERINFO *pf1=p;

  for(;p->name_next!=NULL;p=p->name_next){
    if(h1==p->name_next->name_hash&&strcmp(name,p->name_next->name)==0){
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
      printf("s");
      free(p->id_next->id);
      p->id_next->id=NULL;
      printf("t");
      q=p->id_next;
      p->id_next=p->id_next->id_next;
      printf("p");
      free(q);
      printf("b");
      n=3;
    }
    if(n==3) break;
  }
  free(pf2);
  if(count==3)  return 0;

  printf("hola");
  return (-1);
  //assert(0);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
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
/*--------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{
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
/*--------------------------------------------------------------------*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  if(NULL==d||NULL==fp) return (-1);
  
  int sum=0; 
  USERINFO *p;
  for(p=d->first;p!=NULL;p=p->next){
    sum+=(*fp)(p->id,p->name,p->purchase);
  }
  return sum;
  assert(0);
}
