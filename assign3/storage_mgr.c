#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dberror.h"
#include "storage_mgr.h"

FILE *file;
int conditioncheck =1;

//Writing this function to initialize the storage manager
//Adding print statements to display the initialization of the storage manager
extern void initStorageManager (void)
{
    printf("This function initializes the storage manager");
    printf("\n Initializing storage manager");
	
}

/*1. This method will assign values to management info and file seek
2. Accepts sm_filehandle, filename and file values*/
void assignFileHandle(SM_FileHandle *fHandle, char *fileName, FILE *file)
{
	int curPos = ftell(file) / PAGE_SIZE;
	int tnp = ftell(file) / PAGE_SIZE;
	if(conditioncheck)
	{
    fHandle->fileName = fileName;
	fHandle->curPagePos = curPos;
	fseek(file, 0, SEEK_END); 
	}
	if(conditioncheck)
	{
    fHandle->totalNumPages = tnp;
	fHandle->mgmtInfo = file;
	fseek(file, curPos * PAGE_SIZE, SEEK_SET);   
	}
}

// This method is used to create empty page handles as per the page size
SM_PageHandle getEmptyPageHandle()
{
	if(conditioncheck)
	{
	SM_PageHandle page = (SM_PageHandle)malloc(PAGE_SIZE);
	int i=0;
	while(i < PAGE_SIZE)
	{
		page[i] = '\0';
		i = ++i;
	}
	return page;
	}
	
}

char *createCharObject()
{
	if(conditioncheck)
	{
    char *page = (char *)calloc(PAGE_SIZE, sizeof(char));
	return page;
	}
	
}

void callFileWrite(char *page, FILE *file)
{
	if(conditioncheck)
	 fwrite(page, PAGE_SIZE, 1, file);
}

//Done by Madhushalini Murali (A20513784)
// Creating a page in the file
RC createPageFile(char *fileName)
{
	char *page1;
	char *pagetitle;
	file = fopen(fileName, "w");
	if (file == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}
	if(file != NULL)
	{
		if(conditioncheck)
		{
		page1 = createCharObject();
		pagetitle = createCharObject();
		fputs("1", file);
		callFileWrite(pagetitle, file);
		callFileWrite(page1, file);
		free(pagetitle);
		free(page1);
		fclose(file);
		return RC_OK; 

		}
          
	}
	return RC_OK;
		
}

bool checkValidfHandle(SM_FileHandle *fHandle)
{
	return (fHandle == NULL) ? false : true;
}

bool checkValidMgmtInfo(SM_FileHandle *fHandle)
{
	return (fHandle->mgmtInfo == NULL) ? false : true;
}


RC openPageFile(char *fileName, SM_FileHandle *fHandle)
{
	while (!checkValidfHandle(fHandle))
		return RC_FILE_HANDLE_NOT_INIT;
	file = fopen(fileName, "r+");
	if (file == NULL)
	{
		return RC_FILE_NOT_FOUND;
		
	}
	while(file != NULL)
	{
		if(conditioncheck)
		{
        fHandle->fileName = fileName;
		char *readHeader = createCharObject();
		fgets(readHeader, PAGE_SIZE, file);
		char *totalPage = readHeader;
		fHandle->totalNumPages = atoi(totalPage);
		fHandle->curPagePos = 0;
		fHandle->mgmtInfo = file;
		free(readHeader);
		return RC_OK;
		}
	}
	return RC_OK;
		
}

// closing the file

RC closePageFile(SM_FileHandle *fHandle)
{
	if (checkValidMgmtInfo(fHandle))
	{
		fclose(fHandle->mgmtInfo);
		return RC_OK;
	}
		
	else
	{
		return RC_FILE_NOT_FOUND;
	}
}

// destroying the page file
RC destroyPageFile(char *fileName)
{
	if (remove(fileName) != 0)
		return RC_FILE_NOT_FOUND;
	if(remove(fileName) == 0 || remove(fileName) >= 0 || remove(fileName) <= 0)
		return RC_OK;
}

//Done by Jaysurya Rathinagiri (A20513480)
// the writeBlock function is used to write the data from memory into the disk
//this writes the data to the specific location using the specified page number
RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	int pgnum = pageNum + 1;
	while (!checkValidfHandle(fHandle))
		return RC_FILE_HANDLE_NOT_INIT;
	while (!checkValidMgmtInfo(fHandle))
		return RC_FILE_NOT_FOUND;
	
	if (pageNum < fHandle->totalNumPages)
	{
		if(conditioncheck)
		{
		fseek(fHandle->mgmtInfo, (pgnum * PAGE_SIZE), SEEK_SET);
		fwrite(memPage, PAGE_SIZE, 1, fHandle->mgmtInfo);
		// Updates current page number to recently written file.
		fHandle->curPagePos = pageNum;
		}
		
	}
	if(pageNum > fHandle->totalNumPages || pageNum == fHandle->totalNumPages)
		return RC_WRITE_FAILED;
	return RC_OK;
}

//the write current block function is similar to the write block function , but the page is not seeked here
// this function write the data from memory into the disk to the current page that is pointed by the page handle pointer
RC writeCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	// If the page you're looking for exists, seek to the page and write.
	while (fHandle->curPagePos < fHandle->totalNumPages)
	{
		fwrite(memPage, PAGE_SIZE, 1, fHandle->mgmtInfo);
	}
	while (fHandle->curPagePos > fHandle->totalNumPages || fHandle->curPagePos == fHandle->totalNumPages)
	{
		return RC_READ_NON_EXISTING_PAGE;
	}
	return RC_OK;
}

// This method performs the file seek and write on the opened file and finally closes it
void fileSeekOperation(SM_PageHandle emptyPage, char *fn)
{
	FILE *fileInAppendMode = fopen(fn, "r+");
	fseek(fileInAppendMode, 0, SEEK_END);
	fwrite(emptyPage, PAGE_SIZE, 1, fileInAppendMode);
	fseek(fileInAppendMode, 0, SEEK_END);
	fclose(fileInAppendMode);
}

RC appendEmptyBlock(SM_FileHandle *fHandle)
{
	while (!checkValidfHandle(fHandle))
		return RC_FILE_HANDLE_NOT_INIT;
	while (!checkValidMgmtInfo(fHandle))
		return RC_FILE_NOT_FOUND;

	int value = 1;
	int tnp = fHandle->totalNumPages;
	int page = tnp + 1;
	char *pg = createCharObject();
	fseek(fHandle->mgmtInfo, page * PAGE_SIZE, SEEK_SET);
	if (!(fwrite(pg, PAGE_SIZE, 1, fHandle->mgmtInfo)))
	{
		free(pg);
		return RC_WRITE_FAILED;
		
	}
	while(fwrite(pg, PAGE_SIZE, 1, fHandle->mgmtInfo))
	{
		if(value)
		{
		fHandle->totalNumPages = fHandle->totalNumPages + 1;
		fHandle->curPagePos = fHandle->totalNumPages - 1;
		fseek(fHandle->mgmtInfo, 0L, SEEK_SET);
		fprintf(fHandle->mgmtInfo, "%d", fHandle->totalNumPages);
		int tnp1 = fHandle->totalNumPages;
		int page1 = tnp + 1;
		fseek(fHandle->mgmtInfo, page1 * PAGE_SIZE, SEEK_SET);
		free(pg);
		return RC_OK;

		}
		
	}
}

RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle)
{
	if(conditioncheck)
	{
    int totalPage = fHandle->totalNumPages;
	if (numberOfPages > totalPage)
	{
		while (numberOfPages > fHandle->totalNumPages)
		{
			appendEmptyBlock(fHandle);
		}
		return RC_OK;
	}
	if(numberOfPages == fHandle->totalNumPages)
    {
        return RC_OK;
    }
	}
	
}

bool checkValidPageNumber(int pageNum, int totalNumberOfPagesInTheFile)
{
	if(conditioncheck)
	 return ((pageNum < 0) & (pageNum > totalNumberOfPagesInTheFile)) ? false : true;
}

bool checkValidSeek(int resultOfseek)
{
	if(conditioncheck)
	 return (resultOfseek > 0 || resultOfseek < 0) ? false : true;
}

bool checkValidRead(int resultOfRead)
{
	if(conditioncheck)
	 return (PAGE_SIZE == resultOfRead) ? true : false;
}

void setCurrentPosition(int pageNumber, SM_FileHandle *fHandle)
{
	if(conditioncheck)
	 fHandle->curPagePos = pageNumber;
}

//Done by Dikshitha Varman (A20521565)
// This function is used for reading a page from the memory.
extern RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
   int resultOfSeek, resultOfRead, totalNumberOfPagesInTheFile,increPageNum;
	totalNumberOfPagesInTheFile = fHandle->totalNumPages;
	while (!checkValidfHandle(fHandle))
		return RC_FILE_HANDLE_NOT_INIT;
	while (!checkValidMgmtInfo(fHandle))
		return RC_FILE_NOT_FOUND;
	while (!checkValidPageNumber(pageNum, totalNumberOfPagesInTheFile))
		return RC_READ_NON_EXISTING_PAGE;
	increPageNum = pageNum + 1;
	resultOfSeek = fseek(fHandle->mgmtInfo, increPageNum * PAGE_SIZE, SEEK_SET);
	while (!checkValidSeek)
		return RC_ERROR;
	resultOfRead = fread(memPage, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo);
	while (!checkValidRead(resultOfRead))
		return RC_READ_FAILED;
	setCurrentPosition(pageNum, fHandle);
	return RC_OK;
}

// This function is used for getting the position of the page from the memory.
int getBlockPos(SM_FileHandle *fHandle)
{
	if (checkValidfHandle(fHandle))
	{
		while (checkValidMgmtInfo(fHandle))
		{
			return fHandle->curPagePos;
		}
		while(!checkValidMgmtInfo(fHandle))
		{
			return RC_FILE_NOT_FOUND;
		}	
	}
	else
	{
		return RC_FILE_HANDLE_NOT_INIT;
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
