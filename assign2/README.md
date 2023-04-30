### ASSIGNMENT - 2: BUFFER MANAGER  TEAM 9 ###

# MEMBERS OF TEAM 9 #
* DIKSHITHA VARMAN ARULMOZHIVARMAN (A20521565)
* MADHUSHALINI MURALI (A20513784)
* JAYASURYA RATHINAGIRI (A20513480)

# TEAM CONTRIBUTION
* DIKSHITHA VARMAN  (A20521565) - BUFFER POOL FUNCTIONS
* JAYASURYA (A20513480) - PAGE MANAGEMENT FUNCTIONS
* MADHUSHALINI  (A20513784) - STATISTICS FUNCTIONS


### Purpose of this repository ###
-> This assignment was helpful for us to learn the implementation of buffer manager. The buffer manager is used to manage a fixed number of pages in memory that is used to represent the pages from a page file managed by the storage manager. (from assignment1). The buffer manager manages the memory pages called as the page frames. 
-> This repository consists of C HEADER FILES, C PROGRAM FILES, C TESTCASE FILES, FINAL C OUTPUT FILES, MAKEFILE - (THE FUNCTION OF MAKEFILE IS TO SPECIFY THE ORDER IN WHICH THE PROGRAM WILL EXECUTE ITSELF), README TEXT FILE WHICH IS LIKE AN INFORMATION MANUAL TO THE PROGRAM.
-> The necessary structures and interfaces are provided which will be helpful to implement this project.

### List of files implemented for buffer manager ###
Makefile
buffer_mgr.c
buffer_mgr.h
buffer_mgr.o
buffer_mgr_stat.c
buffer_mgr_stat.h
buffer_mgr_stat.o
dberror.c
dberror.h
dberror.o
dt.h
storage_mgr.c
storage_mgr.h
storage_mgr.o
test_assign2_1.c
test_assign2_1.o
test_assign2_2.c
test_helper.h


### List of functions used to implement buffer manager ###
### A. Interface Pool Handling functions ###
1. RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName,
          const int numPages, ReplacementStrategy strategy,
          void *stratData);
    ->This creates a new buffer pool and initializes the buffer pool objects. Returns RC_OK if the function is implemented successfully.

2. RC shutdownBufferPool(BM_BufferPool *const bm);
    ->This destroys the buffer pool and writes all the dirty pages back to the disc again before destroying. At last, it frees all the page data in the frame.

3. RC forceFlushPool(BM_BufferPool *const bm);
    ->This checks if the page file is dirty. If found to be dirty, then it writes the page back to the disk and closed the page file.


### B. Interface Access Page functions ###
1. RC markDirty (BM_BufferPool *const bm, BM_PageHandle *const page);
    ->This is used to mark the page as dirty if the page is already found in the buffer pool.

2. RC unpinPage (BM_BufferPool *const bm, BM_PageHandle *const page);
    -> This is used to mark the page as unpinPage if the page is not in use and if the page reading is completed.

3. RC forcePage (BM_BufferPool *const bm, BM_PageHandle *const page);
    ->This function writes the current page content to the existing page file on the disk.

4. RC pinPage (BM_BufferPool *const bm, BM_PageHandle *const page,
        const PageNumber pageNum);
    ->This function loads the page frame to the pool using the page replacement strategies.


### C. Statistics Interface ###
1. PageNumber *getFrameContents (BM_BufferPool *const bm);
    ->It initializes the array of page numbers in which each page in the array is stored in its respective page frame.

2. bool *getDirtyFlags (BM_BufferPool *const bm);
    ->It generated an boolean value array in which the particular element of the array is marked true if the page frame is dirty.

3. int *getFixCounts (BM_BufferPool *const bm);
    ->This functions gives an integer array that counts the number of times a particular page is been accessed by multiple users.

4. int getNumReadIO (BM_BufferPool *const bm);
    ->This function is used to give the total number of all the read block operations performed.

5. int getNumWriteIO (BM_BufferPool *const bm);
    ->->This function is used to give the total number of all the write block operations performed.