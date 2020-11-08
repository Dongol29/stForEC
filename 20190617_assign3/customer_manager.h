/* 20190617 조동올, assignment 3, customer_manager.h */

#ifndef CUSTOMER_MANAGER_H
#define CUSTOMER_MANAGER_H

/**********************
 * EE209 Assignment 3 *
 **********************/
/* customer_manager.h */

/* forward type definition for DB_T */
/* "struct DB" should be defined in customer_manager1.c,
    customer_manager2.c, and customer_manager3.c */
typedef struct DB* DB_T;

/* function pointer type definition */
typedef int (*FUNCPTR_T)(const char* id, const char* name,
						 const int purchase);

/* create and return a db structure
   return NULL if fails to allocate the memory */
DB_T CreateCustomerDB(void);

/* destory db and its associated memory */
void DestroyCustomerDB(DB_T d);

/* register a customer with (name, id, purchase)
   on success, return 0. Otherwise, return (-1) */
int  RegisterCustomer(DB_T d, const char *id,
		      const char *name, const int purchase);

/* unregister a customer with 'id'
   on success, return 0. Otherwise(failure), return (-1) 
   1) If d or id is NULL, it is a failure. 
   2) If no such item exists, it is a failure. */
int UnregisterCustomerByID(DB_T d, const char *id);

/* unregister a customer with 'name'
   on success, return 0. Otherwise(failure), return (-1)
   1) If d or name is NULL, it is a failure. 
   2) If no such item exists, it is a failure. */
int UnregisterCustomerByName(DB_T d, const char *name);

/* get the purchase amount of a user whose ID is 'id' 
   On success, it should return the purchase amount.
   Otherwise, it should return -1
   1) If d or id is NULL, it is a failure. 
   2) If there is no customer whose ID matches the given one,
   it is a failure. */
int GetPurchaseByID(DB_T d, const char* id);

/* get the purchase amount of a user whose name is 'name' 
   On success, it should return the purchase amount.
   Otherwise, it should return -1
   1) If d or name is NULL, it is a failure. 
   2) If there is no customer whose name matches the given one,
   it is a failure. */
int GetPurchaseByName(DB_T d, const char* name);

/* iterate all valid user items once, evaluate fp for each valid user
   and return the sum of all fp function calls.
   On success, GetCustomerPurchase should return the sum of all numbers 
   returned by fp by iterating each user item in d.
   Otherwise,if d or fp is NULL, it should return -1. */
int GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp);

#endif /* end of CUSTOMER_MANAGER_H */