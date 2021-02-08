#ifndef _DYNAMIC_ARRAYS_H_
#define _DYNAMIC_ARRAYS_H_

/* Growth Policy : Constant Size */
#define GROWTH_AMOUNT (100) 
#define MAX_STR_LEN   (256) 

/**************************  modify as needed *************************/
/** Data typemark to store payload: Modify according to application  ***/
/** In this case the payload consists of a sequence number and a word **/
   
typedef struct {
   char User[MAX_STR_LEN];  /* string MUST BE < MAX_STR_LEN*/
   char Pass[MAX_STR_LEN];  /* string MUST BE < MAX_STR_LEN*/
  } Data;
/************ Nothing else in the module needs to be modified **********/



/******* ***** Nothing else in the module needs to be modified **********/

/**************************************************************************
* DArray - Dynamic Array Header structure to keep track of its "state" 
*          The "state" of the Dynamic Array is: Capacity and EntriesUsed
**************************************************************************/
typedef struct {
   unsigned int Capacity;    /* Max Number of elements array can hold */
   unsigned int EntriesUsed; /* Number of array elements "used"       */
   Data *Payload;   /* Pointer to array that actually holds the data  */
  } DArray;

/***************************************************************************
 function prototypes to create, add and destroy data arrays
 **************************************************************************/
void CreateDArray( DArray *DArrayPtr, unsigned int InitialSize );
unsigned int PushToDArray( DArray *DArrayPtr, Data *PayloadPtr );
void DestroyDArray(DArray *DArrayPtr);
int SearchDArray(DArray *DArrayHead, char *userCheck, char *passCheck);

#endif /* _DYNAMIC_ARRAYS_H_ */
