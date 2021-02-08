/**
 ** DynamicArrays.c  - private implementations - student file
 **
 ** Implementation of a (constant growth) Dynamic Array data type             
 **   Modify struc Data in DynamicArrays.h for payload according application  
 **   To change growth policy modify PushToDArray as needed                   
 **                                                                           
 ** Written by: Greg Semeraro,Ph.D.                                           
 ** Modified: Dr. Juan C. Cockburn (jcck@ieee.org)                            
 ** 02/01/2014 JCCK     
 ** 10/02/2015 R Repka 
 ** 03/03/2017 R Repka
 ** 07/21/2017 R.Repka     - added class errors include file
 ** 09/25/2017 R.Repka     - Minor pseudo code correction
 ** 09/15/2019 R. Repka    - Added example pointer usage
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "userDatabase.h"

/***************************************************************************
 CreateDArray -- Allocates memory and initializes DArray state             
        void CreateDArray(DArray *DArrayHead, unsigned int InitialSize)
 Where:
    DArray *DArrayHead       - Pointer to a storage block which holds the
                               data structure entry 
    unsigned int InitialSize - The initial data block size, could be 0
    returns: void            - nothing is returned.
    errors:                  - This routine will print an error message to 
                               stderror and exit with an error code
 **************************************************************************/
void CreateDArray(DArray *DArrayHead, unsigned int InitialSize) {
   /************************************************************************
   Pseudo code: 
   Initialize array with 0 entries used 
   Initialize capacity (you decide, could be zero)
   Based on capacity allocate memory storage, if necessary 
      If initial capacity is zero, no allocation needed 
      else Allocate storage space for payload according to struc Data
   if memory not allocated print error to stderr and exit with a code 
   
   Access examples:    DArrayHead->Capacity  = something
                       DArrayHead->Payload   = some malloc code
  ************************************************************************/
  
	DArrayHead->EntriesUsed = 0; //Initialize the amount of entries used to 0
	if(InitialSize == 0){
		DArrayHead->Capacity = InitialSize;
	}
	else {
		DArrayHead->Capacity = InitialSize; //Initalize the capacity to the initial size requested
		DArrayHead->Payload = (Data*) malloc(InitialSize * sizeof(Data)); //Allocate space according to the requested size
		if(DArrayHead->Payload == NULL) { //If the allocation was unsuccessful then print error to stderr and exit with an error code
			fprintf(stderr, "ERROR");
			exit(-99);
		}
	}
  
} /* CreateDArray() */





/***************************************************************************
  PushToDArray -- Adds data at the end of the dynamic array and updates its 
                  state in the array header.  If full, reallocates memory 
                  space according to growth policy                          
        unsigned int PushToDArray(DArray *DArrayHead, Data *PayloadPtr)     
Where:
    DArray *DArrayHead      - Pointer to a storage block which holds the
                              data structure entry 
    Data * PayloadPtr       - 
    returns: unsigned int   - The index of the last element inserted
    errors:                 - This routine will print an error message to 
                              stderror and exit with an error code 
 **************************************************************************/
unsigned int PushToDArray(DArray *DArrayHead, Data *PayloadPtr)
{
   /************************************************************************
    Pseudo code: 
    Check if the array needs to be made larger 
    If yes, increase the size of the dynamic array by GROWTH_AMOUNT
    Re-allocate storage for the array elements using larger size 
    if memory not re-allocated print error to stderr and exit 
    Copy the Data in PlayloadPtr into the Darray
    Increment the number of elements used in Darray header 
    Return the array index of last element inserted 
   ************************************************************************/
	if ((DArrayHead->Capacity ==  DArrayHead->EntriesUsed) ){
		DArrayHead->Capacity += GROWTH_AMOUNT; //Incrememnt by growth amount to hold more data
		DArrayHead->Payload = realloc(DArrayHead->Payload, sizeof(Data)*(DArrayHead->Capacity)); //Reallocate the storage by making it bigger
		if (DArrayHead->Payload == NULL){  //If reallocation failed, print to stderror and exit with an error code
			fprintf( stderr, "error");
			exit(-101);
		}
	} 
	memcpy(&DArrayHead->Payload[DArrayHead->EntriesUsed], PayloadPtr,sizeof(Data));
	DArrayHead->EntriesUsed = DArrayHead->EntriesUsed + 1;
	return (DArrayHead->EntriesUsed - 1);
} /* PushToDArray() */






/*************************************************************************
 DestroyDArray -- Returns the memory back to the heap and updates the   
                  state in the array header      
        void DestroyDArray(DArray *DArrayHead)
  Where:
    DArray *DArrayHead  - Pointer to a storage block which holds the
                          data structure entry 
    returns: void       - nothing is returned.
    errors:             - No errors are reported
*************************************************************************/
void DestroyDArray(DArray *DArrayHead)
{ 
  /**********************************************************************
    Pseudo code: 
    Set the number of entries used to zero in Darray header 
    Set the capacity to zero in Darray header
    De-allocate the storage for the array elements 
 *************************************************************************/
	
	DArrayHead->EntriesUsed = 0;  //Reset the entries used by removing contents
	DArrayHead->Capacity = 0;     //Reset the capacity 
	free(DArrayHead->Payload);    //Reset the space in memory and restore to heap
	DArrayHead->Payload = NULL;   //Set pointer to NULL

} /* DestroyDArray() */





/**************************************************************************
 Searches the dynamic array for a provided word and returns the
 pointer to the data if found or NULL if not found.

 Where: DArray *DArrayHead - Pointer to the dynamic array to search
        char *userCheck       - The username to search for
        char *passCheck       - The password to search for
 Returns: 1 if username and password were found, 0 if the password was incorrect
         -1 if invalid username
 Errors: none
***************************************************************************/
int SearchDArray(DArray *DArrayHead, char *userCheck, char *passCheck) {
   
   //printf("The number of entries is: %d\n", DArrayHead->EntriesUsed); //Error Checking
   for(int i = 0; i<DArrayHead->EntriesUsed; i++){ //Recurse until the number of entries is reached
    
    //printf("The username currently is: %s\n", DArrayHead->Payload[i].User); //Error Checking
    //printf("The password currently is: %s\n", DArrayHead->Payload[i].Pass); //Error Checking
	  
    if(!(strncmp(DArrayHead->Payload[i].User, userCheck, sizeof(char) * MAX_STR_LEN))){ //Compare 
	   if(!(strncmp(DArrayHead->Payload[i].Pass, passCheck, sizeof(char) * MAX_STR_LEN))){ //Compare 
	     return 1;}
     else{
       return 0;}
	  }
   }  
   printf("Username: %s not found. \n", userCheck); 
   return -1;

} /* SearchDArray() */
