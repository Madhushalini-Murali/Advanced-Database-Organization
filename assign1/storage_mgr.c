#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dberror.h"
#include "storage_mgr.h"

FILE *file;

//Writing this function to initialize the storage manager
//Adding print statements to display the initialization of the storage manager
extern void initStorageManager (void)
{
    printf("This function initializes the storage manager");
    printf("\n Initializing storage manager");
	printf("\n Creating new Page");
}

//Done by Madhushalini Murali (A20513784)
// This function is used to create a new page
// The new Page will be created in the file
extern RC createPageFile (char *fileName)
{
	int value;
	SM_PageHandle new_Page[PAGE_SIZE];
    
	// the fopen function is used to open the file that is given by the fileName
	// this function open the files to perform various operations 
	// If the given file is present then that particular file is opened
	// IF the file is not present then a new file will be created
	file = fopen(fileName, "w");

    char *mem = malloc(PAGE_SIZE * sizeof(char));
	
    if(file != NULL ){
        // fwrite is the function that helps in writing to the FILE *file
        fwrite(mem, 1, PAGE_SIZE, file); 
        //the fclose function is used to close the file
        fclose(file);
        // this will return 0
        free(mem);
        printf("\n Created new Page");
        return RC_OK;
    }
    else{
        free(mem);
        return RC_FILE_NOT_FOUND;

    }
}


//This function is used to open a page from the file
extern RC openPageFile (char *fileName, SM_FileHandle *fHandle)
{
	
    //writing the condition to check if the file exist 
	file = fopen(fileName, "r+");
	if(file != NULL)
    {
            //fseek is used here to move the file to the given particular location
			//SEEK_END is used to move the file position to the end of the file.
			fseek(file, 0, SEEK_END);
            //init the attributes
			(*fHandle).fileName = fileName; 
            //ftell function gives the current file position corresponding to the start of the file		
			(*fHandle).totalNumPages = ftell(file)/PAGE_SIZE;
            // assigning the current page position as 0
			(*fHandle).curPagePos = 0;					
			(*fHandle).mgmtInfo = file;
            //through ftell function, we can find the total number of pages
			//hence we are moving the file position back to the beginning of the file
			//to implement this, we are using the SEEK_SET 
			//SEEK_SET is used to move te file position to the beginning of the file
			fseek(file, 0, SEEK_SET);
            // return 0
            printf("\n opened new Page");

			return RC_OK;
	}
    else
    {
        printf("\n For non-existing file");

        //will return file not found
        return RC_FILE_NOT_FOUND;
    }
}

extern RC closePageFile (SM_FileHandle *fHandle)
{	
    //check if an open page file is closed or not closed
	switch  (fclose(fHandle->mgmtInfo) == 0)
	{		
        // checks if the open page file is closed and if closed returns 0
		case true:
		{
            printf("\n closed new Page");

			return RC_OK;
			break;
		}	
        //if the open page file is not closed , then return file not found	
		case false:
		{
			return RC_FILE_NOT_FOUND;
			break;
		}
	}
}

extern RC destroyPageFile (char *fileName)
{	
    char *memo = malloc(PAGE_SIZE * sizeof(char));
    //this function deletes or destroys a page file

    free(memo);
	
    //if deleted, then return 0
    if(remove(fileName) == 0)
    {
        printf("\n destroyed new Page");

        return RC_OK;
    }
	
    // if the page file is not deleted, then return file not found
	else
	{
		return RC_FILE_NOT_FOUND;
	}	
}

//Done by Jaysurya Rathinagiri (A20513480)
// the writeBlock function is used to write the data from memory into the disk
//this writes the data to the specific location using the specified page number
extern RC writeBlock (int pageNum , SM_FileHandle * fHandle , SM_PageHandle memPage ) 
{
    int totalpagecount, writeonblock, pagemgmtinfo, count,fileseek,filewrite;
    //fHandle -> mgmtInfo = pagemgmtinfo;
    totalpagecount = fHandle -> totalNumPages;
    // calculating the offset value through the multiplication of page number and page size
    count = pageNum * PAGE_SIZE;
    //checking if the specified page number exists within the total number of pages
    if(pageNum < totalpagecount)
    {
    //fseek is seeking the page number using the count variable , so that data can be written in that particular page
     fileseek = fseek(fHandle -> mgmtInfo, count,SEEK_SET);
     //fwrite is used to write data into the file
     filewrite = fwrite(memPage, 1, PAGE_SIZE, fHandle -> mgmtInfo);
     //updating the current page position to the written file
     pageNum = fHandle -> curPagePos;
     // to append an empty block
    appendEmptyBlock(fHandle);
    // return 0
    return RC_OK;
    }
    
    else
        RC_FILE_NOT_FOUND;
}


//the write current block function is similar to the write block function , but the page is not seeked here
// this function write the data from memory into the disk to the current page that is pointed by the page handle pointer
extern RC writeCurrentBlock ( SM_FileHandle * fHandle , SM_PageHandle memPage ) 
{
    int positionofpage, totpagecount, writeonblock;
    positionofpage = fHandle -> curPagePos;
    totpagecount = fHandle -> totalNumPages;
    //checking if the specified page number or block exists within the total number of pages
    while(positionofpage < totpagecount)
    {
         //fwrite is used to write data into the current page (fseek is not used here)
        writeonblock = fwrite(memPage, 1, PAGE_SIZE,fHandle -> mgmtInfo);
        //return 0
        return RC_OK;
    }
    // return read non existing page
    return RC_READ_NON_EXISTING_PAGE;
}


//this function is used to increase/increment the number of blocks or pages by 1
// and after that creates a file where the size of the page is zero characters
extern RC appendEmptyBlock (SM_FileHandle * fHandle) 
{
    // Initializing a new page/block of 4096 
    SM_PageHandle createBlock[PAGE_SIZE];
    int blockposition,writeblockposition,incrementedpgcount;
    blockposition = 0;
    // assigning each and every page/block as null
    do{
        createBlock[blockposition] = '\0';
        blockposition = blockposition + 1;
    }
    
    while(blockposition < PAGE_SIZE);
    incrementedpgcount = (fHandle -> totalNumPages);
    // to increment the count of the page in the meta data
    incrementedpgcount++;
    writeblockposition = writeBlock(incrementedpgcount,fHandle,createBlock[PAGE_SIZE]);
    //return OK
    return RC_OK;
}


// this function is used to ensure the capacity of the disk
//checks if the number of pages in the memory and the number of pages in the disk are same
// if it is less, then increase the page size and the meta data
//if it is more, then write the pages to the next disk as an empty file
extern RC ensureCapacity (int numberOfPages , SM_FileHandle * fHandle ) 
{
	//if the pages in the memory and disk are not equal, then create the extra number of pages until they become equal
    do
    {
        appendEmptyBlock(fHandle);
    }while(numberOfPages > fHandle->totalNumPages);
    // if the number of pages are equal, then return OK
    if(numberOfPages == fHandle->totalNumPages)
    {
        return RC_OK;
    }
}    

//Done by Dikshitha Varman (A20521565)
// This function is used for reading a page from the memory.
extern RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    /*Read the contents of the file present in specified page.*/
    int seekpage=pageNum * PAGE_SIZE;
    if(file == NULL)                                           
        return RC_FILE_NOT_FOUND;             
    else
    {
        if (pageNum < 0 && pageNum > fHandle->totalNumPages )           
            return RC_READ_NON_EXISTING_PAGE;                         
        else 
        {
            fseek(file, seekpage, SEEK_SET);                     
            fread(memPage, sizeof(char), PAGE_SIZE, file);  
            fHandle->curPagePos = pageNum;                         
            return RC_OK;                                         
        }
    }
   
}

// This function is used for getting the position of the page from the memory.
extern int getBlockPos(SM_FileHandle *fHandle)
{
    if (fHandle != NULL)
    {
        return fHandle -> curPagePos;
    }
    else
    {
        return RC_FILE_NOT_FOUND;
    }
}   
// This function is used for reading the first page in the memory, getting the input: filehandle and page handle.
// Reading the file's page into memory and returns RC_OK there are no excpetion.    
extern RC readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
   int beginningpage,positionofblock;
   beginningpage=0;
   if(fHandle != NULL)
   {
     positionofblock = readBlock(beginningpage,fHandle,memPage);
   }
   if(positionofblock == 0){
        return positionofblock;
        RC_OK;
   }
   else
        return RC_FILE_NOT_FOUND;  
}	

// This function is used for reading the previous page in the memory, getting the input: filehandle and page handle.
extern RC readPreviousBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	int currentblockplace, readingcurrentblockvalue;
    currentblockplace = getBlockPos(fHandle);
    readingcurrentblockvalue = readBlock(currentblockplace,fHandle,memPage);
    return readingcurrentblockvalue;
}  

// This function is used for reading the current page in the memory, getting the input: filehandle and page handle.    
extern RC readCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	int currentblockplace, readingcurrentblockvalue;
    currentblockplace = getBlockPos(fHandle);
    readingcurrentblockvalue = readBlock(currentblockplace,fHandle,memPage);
    return readingcurrentblockvalue;
}

 // This function is used for reading the upcoming page in the memory, getting the input: filehandle and page handle. 
extern RC readNextBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	int currentBlockpos,toreadnextblock;
    currentBlockpos = getBlockPos(fHandle);
    if(fHandle != NULL)
    {
      toreadnextblock = readBlock(currentBlockpos + 1, fHandle, memPage);
    }
    return toreadnextblock;
}

// This function is used for reading the last page in the memory, getting the input: filehandle and page handle.
extern RC readLastBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	int endpagenumber;
    int lastblock;
    endpagenumber = fHandle->totalNumPages;
    lastblock = readBlock(endpagenumber -1, fHandle,memPage);
    return(lastblock);
}