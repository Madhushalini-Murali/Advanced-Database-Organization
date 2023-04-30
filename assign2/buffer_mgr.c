#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "buffer_mgr.h"
#include "storage_mgr.h"

// This structure is used for storing the buffer data
typedef struct BufferFrame
{
	struct BufferFrame *Framebefore, *Frameafter;
	int dirtyflagcount;
	int pgCount;
	int calculate;
	char *data;
} BufferFrame;

// This structure is used for Managing the buffer data, the number of reads and writes done in the page
typedef struct BufferManager
{
	BufferFrame *beginning, *current, *end;
	SM_FileHandle *smFileHandle;
	int numberOFread;
	int numberOFwrite;
	int calculate;
	void *strategyData;
} BufferManager;


// BUFFER POOL FUNCTIONS
//Assigning the positions of Buffer Manager Values input to this function is bufferManager and bufferFrame
void AssignBufferManagerValues(BufferManager *bufferManager, BufferFrame *frame)
{	
	bufferManager->beginning = bufferManager->current;
	if (frame != NULL)
	{
     if (bufferManager->beginning == NULL)
	{	
		{
			bufferManager->beginning = frame;
			bufferManager->end = bufferManager->beginning;
			bufferManager->current = bufferManager->end;
			
		}
	}
	else
	{
		{
			bufferManager->end->Frameafter = frame;
			frame->Framebefore = bufferManager->end;
			bufferManager->end = bufferManager->end->Frameafter;
		}
	}
	}
	bufferManager->end->Frameafter = bufferManager->beginning;
	bufferManager->beginning->Framebefore = bufferManager->end;
}

// This is an external method to be used in INITBUFFERPOOL, to create a buffer frame from the buffer manager.
void createBufferFrame(BufferManager *bufferData)
{
	int index, pgno;
	index = -1;
	pgno = 0;
	BufferFrame *frame = (BufferFrame *)malloc(sizeof(BufferFrame));
	char *calldata = calloc(PAGE_SIZE, sizeof(char *));
	frame->data = calldata;

	frame->dirtyflagcount = pgno;
	frame->calculate = pgno;
	frame->pgCount = index;

	AssignBufferManagerValues(bufferData, frame);
}

// This method is used for freeing the memory of the buffer pool.
void CleanBufferPool(BufferManager *bufferData, BM_BufferPool *dataofbufferpool)
{
	int ind;
	ind = 0;
	char value;
	value = NULL;
	if (ind ==0 || value == NULL)
	{
		bufferData->current = value;
		bufferData->beginning = value;
		bufferData->end = value;
		dataofbufferpool->pageFile = ind;
		dataofbufferpool->mgmtData = ind;
		dataofbufferpool->numPages = ind;
		
	}

	free(bufferData);
}

//This creates a new buffer pool and initializes the buffer pool objects. Returns RC_OK if the function is implemented successfully.
RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName, const int numPages, ReplacementStrategy strategy, void *stratData)
{
	int i;
	if (bm->mgmtData != NULL)
	{
	  return RC_BUFFER_POOL_EXIST;
	}
	BufferManager *bufferManager = (BufferManager *)malloc(sizeof(BufferManager));
	bufferManager->smFileHandle = (SM_FileHandle *)malloc(sizeof(SM_FileHandle));	
	RC OpenPageReturnCode = openPageFile((char *)pageFileName, bufferManager->smFileHandle);
	if (OpenPageReturnCode != RC_OK)
	{	
		return OpenPageReturnCode;
	}
	else
	{
		for (i = 0; i < numPages; i++)
		{	
			createBufferFrame(bufferManager);
		}
		bufferManager->numberOFwrite = 0;
		bufferManager->end = bufferManager->beginning;
		bufferManager->numberOFread = 0;
		bufferManager->strategyData = stratData;
		bufferManager->calculate = 0;
		bm->strategy = strategy;
		bm->numPages = numPages;
		bm->mgmtData = bufferManager;
		bm->pageFile = (char *)pageFileName;
		closePageFile(bufferManager->smFileHandle);

		return RC_OK;
	}	
}

//This checks if the page file is dirty. If found to be dirty, then it writes the page back to the disk and closed the page file.
RC forceFlushPool(BM_BufferPool *const bm)
{
	BufferManager *bufferManager = bm->mgmtData;
	BufferFrame *frame = bufferManager->beginning;

	bufferManager->smFileHandle = (SM_FileHandle *)malloc(sizeof(SM_FileHandle));
	RC openpageReturnCode = openPageFile((char *)(bm->pageFile), bufferManager->smFileHandle);
	if(openpageReturnCode != RC_OK)
	{
		return openpageReturnCode;
	}
	else
	{
		do
		{	
			if (frame->dirtyflagcount != 0 && frame->calculate == 0)
			{
					int pn = frame->pgCount;
					SM_FileHandle *smhandle = bufferManager->smFileHandle;
					
					RC writeBlockReturnCode = writeBlock(pn, smhandle, frame->data);
					if (writeBlockReturnCode != RC_OK)
					{
						closePageFile(bufferManager->smFileHandle);
						return writeBlockReturnCode;
					}
					
					else
					{
						frame->dirtyflagcount = 0;
						bufferManager->numberOFwrite++;	
					}
				
			}
			frame = frame->Frameafter;
		} while (frame != bufferManager->beginning);
	}
	closePageFile(bufferManager->smFileHandle);
	return RC_OK;
}

//This destroys the buffer pool and writes all the dirty pages back to the disc again before destroying. At last, it frees all the page data in the frame.
RC shutdownBufferPool(BM_BufferPool *const bm)
{
	int i=0;
	if (bm->mgmtData == NULL)
	{
	  return RC_BUFFER_POOL_EXIST;
	}
	BufferManager *bufferManager = bm->mgmtData;
	BufferFrame *frame = bufferManager->beginning;
	forceFlushPool(bm);
	frame = frame->Frameafter;
	while (frame != bufferManager->beginning)
	{
		free(frame->data);
		frame = frame->Frameafter;
		i++;
	}
	free(frame);
	CleanBufferPool(bufferManager, bm);
	return RC_OK;
}


//PAGE MANAGEMENT FUNCTIONS
//This is used to mark the page as dirty if the page is already found in the buffer pool.
RC markDirty(BM_BufferPool *const bm, BM_PageHandle *const page)
{
	int value = 0;
	value +=1;
	if (bm->mgmtData == NULL)
	{
	  return RC_BUFFER_POOL_NOT_INIT;
	}
	BufferManager *bufferManager = bm->mgmtData;
	BufferFrame *frame = bufferManager->beginning;
	do
	{
		
		if (frame->pgCount != page->pageNum)
		{
			
			frame = frame->Frameafter;
		}
		else
		{
			frame->dirtyflagcount = value;
			return RC_OK;
		}
			

	} while (frame != bufferManager->beginning);

	return RC_OK;
}

//This is used to mark the page as unpinPage if the page is not in use and if the page reading is completed.
RC unpinPage(BM_BufferPool *const bm, BM_PageHandle *const page)
{
	BufferManager *bufferManager = bm->mgmtData;
	BufferFrame *frame = bufferManager->beginning;
	int iteration=0;
	while ((bufferManager->beginning->pgCount) == (page->pageNum))
	{
		bufferManager->beginning->calculate--;
		return RC_OK;
	}
	frame = frame->Frameafter;
	
	while(frame != bufferManager->beginning)
	{
		if (frame->pgCount == page->pageNum)
		{
			frame->calculate--;
			return RC_OK;
		}
		frame = frame->Frameafter;
		iteration++;
	}

	return RC_OK;
}

//This function writes the current page content to the existing page file on the disk.
RC forcePage(BM_BufferPool *const bm, BM_PageHandle *const page)
{
	BufferManager *bufferManager = bm->mgmtData;
	BufferFrame *frame = bufferManager->beginning;

	bufferManager->smFileHandle = (SM_FileHandle *)malloc(sizeof(SM_FileHandle));
	RC openpageReturnCode = openPageFile((char *)(bm->pageFile), bufferManager->smFileHandle);
	if (openpageReturnCode != RC_OK)
	{
		return openpageReturnCode;
	}
	else
	{
		do
		{
			if ( frame->pgCount == page->pageNum)
			{
				if(frame->dirtyflagcount == 1)
				{
					RC writeBlockReturnCode = writeBlock(frame->pgCount, bufferManager->smFileHandle, frame->data);
					if (writeBlockReturnCode != RC_OK)
					{
						closePageFile(bufferManager->smFileHandle);
						return writeBlockReturnCode;
					}
					else
					{
						frame->dirtyflagcount = 0;
						bufferManager->numberOFwrite++;
					}
				}
			}
			frame = frame->Frameafter;
		} while (frame != bufferManager->beginning);
	}	
	closePageFile(bufferManager->smFileHandle);
	return RC_OK;
}

// This method receives the stratergy, page number to check if the required page exists and returns RC_OK if the page exists already.
RC CheckIfPageExists(int algo, const PageNumber pageNum, 
BufferManager *bufferManager, BM_PageHandle *const page)
{
	int number;
	number =0;
	BufferFrame *frame = bufferManager->beginning;
	do
	{	
		if (frame->pgCount == pageNum)
		{
			page->data = frame->data;
			page->pageNum = pageNum;
			frame->pgCount = pageNum;
			frame->calculate++;
			if (algo == RS_LRU)
			{
				bufferManager->end = bufferManager->beginning->Frameafter;
				bufferManager->beginning = frame;
			}
			number = 1;
			return RC_OK;
		}
		else
		{	
			frame = frame->Frameafter;
		}
	} while (frame != bufferManager->beginning);
	while (number == 0)
	{
		return RC_IM_KEY_NOT_FOUND;
	}
	return RC_OK;
}

// This method is used to check whether the buffer pool is empty or not.
void CheckIfBufferPoolIsEmpty(const PageNumber pageNumber, 
BM_BufferPool *const bufferPool)
{
	BufferManager *bufferManager = bufferPool->mgmtData;
	BufferFrame *frame = bufferManager->beginning;
	if ((bufferPool->numPages) > (bufferManager->calculate))
	{
		frame = bufferManager->beginning;
		frame->pgCount = pageNumber;
		if ((frame->Frameafter) != (bufferManager->beginning))
		{
			bufferManager->beginning = frame->Frameafter;
		}
		frame->calculate = frame->calculate + 1;
		bufferManager->calculate = bufferManager->calculate + 1;
	}
}

//This method is used for closing an existing file and return the code.
RC CheckReturnCode(SM_FileHandle sm_fileHandle, RC returnCode)
{
	closePageFile(&sm_fileHandle);
	return returnCode;
}

//This method is used for pinning the recently used frame from the buffer frame.
//The function returns RC_OK if the read and write block functions have been executed successfully.
RC LRU(SM_FileHandle sm_FileHandle, BM_PageHandle *const page, const PageNumber pageNumber,
	   BufferFrame *frame, BM_BufferPool *const bm, BufferManager *bufferManager)
{
	if (bm->numPages <= bufferManager->calculate)
	{
		frame = bufferManager->end;
		do
		{
			if (frame->calculate != 0)
				frame = frame->Frameafter;
			else
			{
				if (frame->dirtyflagcount != 0) 
				{
					int pageno = frame->pgCount;
					ensureCapacity(pageno, &sm_FileHandle);
					RC writeBlockReturnCode = writeBlock(pageno, &sm_FileHandle, frame->data);
					if (writeBlockReturnCode != RC_OK)
					{
						return CheckReturnCode(sm_FileHandle, writeBlockReturnCode);
					}
						
					else
					{
						bufferManager->numberOFwrite++;
					}	
				}

				if (bufferManager->end != bufferManager->beginning)
				{
					PageNumber pageno = pageNumber;
					frame->pgCount = pageno;
					frame->calculate++;
					bufferManager->end = frame->Frameafter;
					break;
				}
				else
				{
					PageNumber pageno = pageNumber;
					frame = frame->Frameafter;
					frame->pgCount = pageno;
					frame->calculate++;
					bufferManager->end = frame;
					bufferManager->beginning = bufferManager->end;
					bufferManager->end = frame->Framebefore;
					break;
				}
			}
		} while (frame != bufferManager->end);
	}
	else
		CheckIfBufferPoolIsEmpty(pageNumber, bm);
	ensureCapacity((pageNumber + 1), &sm_FileHandle);
	RC readBlockReturnCode = readBlock(pageNumber, &sm_FileHandle, frame->data);
	if (readBlockReturnCode != RC_OK)
	{
		return readBlockReturnCode;
	}
		
	else
	{
		bufferManager->numberOFread++;
	}
		
	page->pageNum = pageNumber;
	page->data = frame->data;
	closePageFile(&sm_FileHandle);
	return RC_OK;
}

//This method is used for pinning the First In First Out (FIFO) frame from the buffer frame.
//The function returns RC_OK if the read and write block functions have been executed successfully.
RC FIFO(SM_FileHandle sm_FileHandle, BM_PageHandle *const page, const PageNumber pageNumber,
		BufferFrame *bufferFrame, BM_BufferPool *const bm, BufferManager *mgmt)
{
	if (bm->numPages >= mgmt->calculate)
	{
		bufferFrame = mgmt->end;
		do
		{
			if (bufferFrame->calculate == 0)
			{
				if (bufferFrame->dirtyflagcount != 0)
				{
					int pn = bufferFrame->pgCount;
					ensureCapacity(pn, &sm_FileHandle);
					RC writeBlockReturnCode = writeBlock(pn, &sm_FileHandle, bufferFrame->data);
					if (writeBlockReturnCode != RC_OK)
					{
						return CheckReturnCode(sm_FileHandle, writeBlockReturnCode);
					}
						
					else
					{
						mgmt->numberOFwrite++;
					}
						
				}

				mgmt->end = bufferFrame->Frameafter;
				bufferFrame->pgCount = pageNumber;
				mgmt->beginning = bufferFrame;
				bufferFrame->calculate++;
				break;
			}
				
			else
			{
				bufferFrame = bufferFrame->Frameafter;
			}

		} while (bufferFrame != mgmt->beginning);
	}

	else
	{
		CheckIfBufferPoolIsEmpty(pageNumber, bm);
	}
		
	ensureCapacity((pageNumber + 1), &sm_FileHandle);
	RC readBlockReturnCode = readBlock(pageNumber, &sm_FileHandle, bufferFrame->data);
	if (readBlockReturnCode != RC_OK)
	{
		return CheckReturnCode(sm_FileHandle, readBlockReturnCode);
	}
		
	else
	{
		mgmt->numberOFread++;
	}
		
	page->pageNum = pageNumber;
	page->data = bufferFrame->data;
	closePageFile(&sm_FileHandle);
	return RC_OK;
}

//This Method is used for verifying the replacement stratergy, it also pins the LRU and FIFO algorithm and returns RC_OK if LRU and FIFO are executed successfully.

RC CheckReplacementStrategy(BM_PageHandle *const page, BufferManager *bufferManager, const PageNumber pageNum,
							ReplacementStrategy strategy, BufferFrame *frame, SM_FileHandle sm_FileHandle, BM_BufferPool *const bufferPool)
{
	RC IsPageExistsReturnCode;
	if (bufferPool->strategy == RS_FIFO)
	{
		IsPageExistsReturnCode = CheckIfPageExists(RS_FIFO, pageNum, bufferManager, page);
		if (IsPageExistsReturnCode != RC_OK)
		{
			FIFO(sm_FileHandle, page, pageNum, frame, bufferPool, bufferManager);
		}
			
		else
		{
			return RC_OK;
		}
			
	}
	if (bufferPool->strategy == RS_LRU)
	{
		IsPageExistsReturnCode = CheckIfPageExists(RS_LRU, pageNum, bufferManager, page);
		if (IsPageExistsReturnCode != RC_OK)
		{
			LRU(sm_FileHandle, page, pageNum, frame, bufferPool, bufferManager);
		}
		else
		{
			return RC_OK;
		}
			
	}
	
	return RC_OK;
}

// This function passes the data through the CheckReplacementStrategy method to pin which algorithm the program will use.
RC pinPage(BM_BufferPool *const bm, BM_PageHandle *const page, const PageNumber pageNum)
{
	if (bm->mgmtData == NULL)
	{
	  return RC_BUFFER_POOL_EXIST;
	}
	RC IsPageExistsReturnCode;

	SM_FileHandle sm_FileHandle;
	BufferManager *bufferManager = bm->mgmtData;
	BufferFrame *frame = bufferManager->beginning;

	RC openPageReturnCode = openPageFile((char *)bm->pageFile, &sm_FileHandle);
	if (openPageReturnCode != RC_OK)
	{
		printf("\n");
	}
	else
	{
		CheckReplacementStrategy(page, bufferManager, pageNum, bm->strategy, frame, sm_FileHandle, bm);
	}
	return RC_OK;
}

//STATISTICS FUNCTION
//This method is used to retrieve the content in the frames from the buffer pool.
PageNumber *getFrameContents(BM_BufferPool *const bm)
{
	int count_ofthe_page = bm->numPages;
	int iteration = 0;
	PageNumber *frameContent = (PageNumber *)malloc(sizeof(PageNumber) * count_ofthe_page);
	BufferFrame *allFrames = ((BufferManager *)bm->mgmtData)->current;
	if (frameContent != NULL)
	{
		while (iteration < count_ofthe_page)
		{
			frameContent[iteration] = allFrames->pgCount;
			allFrames = allFrames->Frameafter;
			iteration++;

		}
	}
	return frameContent;
}

//This method is used for retrieving the dirty flags from all the pages in the program. 
bool *getDirtyFlags(BM_BufferPool *const bm)
{
	int count_ofthe_page = bm->numPages;
	int iteration = 0;
	BufferFrame *frame_Present = ((BufferManager *)bm->mgmtData)->current;
	bool *dirtyFlag = (bool *)malloc(sizeof(bool) * count_ofthe_page);
	if (dirtyFlag != NULL)
	{
		while (iteration < count_ofthe_page)
		{
			//iterates through and displays which are dirty 
			dirtyFlag[iteration] = frame_Present->dirtyflagcount;
			frame_Present = frame_Present->Frameafter;
			iteration++;
		}
	}
	return dirtyFlag;
}

//This method retrives the count of stored pages in the page frames.
int *getFixCounts(BM_BufferPool *const bm)
{
	int count_ofthe_page = bm->numPages;
	int iteration=0;

	BufferFrame *frame_Present = ((BufferManager *)bm->mgmtData)->current;

	int *fixCountResult = (int *)malloc(sizeof(int) * count_ofthe_page);

	if (fixCountResult != NULL)
	{
		
		while (iteration < count_ofthe_page)
		{
			fixCountResult[iteration] = frame_Present->calculate;
			frame_Present = frame_Present->Frameafter;
			iteration++;
		}
	}
	return fixCountResult;
}

//This function is used for retrieving the number of reads in the buffer pool.
int getNumReadIO(BM_BufferPool *const bm)
{
	if (bm->mgmtData != NULL)
	{
	  return ((BufferManager *)bm->mgmtData)->numberOFread;
	  
	}
    else
	{
		return RC_BUFFER_POOL_NOT_INIT;
	}
}


//This function is used for retrieving the number of write in the buffer pool.
int getNumWriteIO(BM_BufferPool *const bm)
{
	if (bm->mgmtData != NULL)
	{
	  return ((BufferManager *)bm->mgmtData)->numberOFwrite;
	  
	}
    else
	{
		return RC_BUFFER_POOL_NOT_INIT;
	}
	
}