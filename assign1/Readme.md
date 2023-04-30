### ASSIGNMENT - 1: STORAGE MANAGER  TEAM 9 ###

# MEMBERS OF TEAM 9 #
* DIKSHITHA VARMAN ARULMOZHIVARMAN (A20521565)
* MADHUSHALINI MURALI (A20513784)
* JAYASURYA RATHINAGIRI (A20513480)

# TEAM CONTRIBUTION

-> OPEN FILE, CLOSE FILE AND FLIE MANIPULATION - MADHUSHALINI MURALI
-> READ BLOCK - DIKSHITHA VARMAN ARULMOZHIVARMAN
-> WRITE BLOCK - JAYASURYA RATHINAGIRI


### Purpose of this repository ###
-> This assignment we learnt how to implement a simple storage manager - which is capable of reading blocks from a file on disk into the memory and for writing in the blocks taking information from the memory and writing it on the disk.
-> We were provided with structures and interfaces needed for implementation of the project.
-> This repository consists of C HEADER FILES, C PROGRAM FILES, C TESTCASE FILES, FINAL C OUTPUT FILES, MAKEFILE - (THE FUNCTION OF MAKEFILE IS TO SPECIFY THE ORDER IN WHICH THE PROGRAM WILL EXECUTE ITSELF), README TEXT FILE WHICH IS LIKE AN INFORMATION MANUAL TO THE PROGRAM.

# FUNCTIONS IN THE REPOSITORY #

* 1. FILE MANIPULATION
    # (i) void initStorageManager(void)
          This function is used inorder to initialize the storage manager.

    # (ii) RC createPageFile (char *fileName)
           This function is used for opening a file, appending and writing the file to an empty block.
    
    # (iii) RC openPageFile(char *fileName, SM_FileHandle *fHandle)
            This function opens an already existing file, if the file needed is not found then the function throws error that the file is not found, if the file is found then it initializes the attributes of the file opened. 

    # (iv) RC openPageFile(char *fileName, SM_FileHandle *fHandle)
            This function opens an already existing file, if the file is not found the function throws FILE NOT FOUND ERROR, once the file is found the function closes the file and handles it.

    # (v) RC closePageFile (SM_FileHandle *fHandle)
           This function opens an already existing file, if the file is not found the function throws FILE NOT FOUND ERROR, once the file is found the function destroys the page file.
           

* 2. FUNCTIONS FOR READING THE BLOCKS
    # (i) RC readBlock(int pageNum, SM_FileHandle* fHandle, SM_PageHandle memPage)
          Verify fHandle's validity; if it has not been initialized, return error (RC FILE HANDLE NOT INIT). If the page number entered is zero or otherwise invalid, return error code (RC FILE HANDLE NOT INIT). Verify memPage's validity; if it's NULL, return error (RC FILE NOT FOUND). fseek is used to shift the curser to the beginning of the blck in the appropriate page if all the aforementioned requirements are met. Return rror (RC Error) if the read position from fseek is not zero. If the above mentioned requirement is met, fread will read the file and store a pointer location in memPage. If fread fails (RC READ Failed), retry. If not, update the handle with the current position and return success with the code RC OK.

    # (ii) int getBlockPos(SM_FileHandle* fHandle)
           Verify that the fHandle has been initialized, and if not, return the error RC FILE HANDLE NOT INIT. Verify MgMtInfo's validity; if it is not, return the RC FILE NOT FOUND error. If the above mentioned criteria are met, return the current page position.
    
    # (iii) RC readFirstBlock(SM_FileHandle* fHandle, SM_PageHandle memPage)
            Verify that the fHandle has been initialized, and if not, return the error RC FILE HANDLE NOT INIT. Verify MgMtInfo's validity; if it is not, return the RC FILE NOT FOUND error. Pass 0 to the readBlock method and return the outcome if the aforementioned criteria are met. If everything goes well, return RC OK.

    # (iv) RC readPreviousBlock(SM_FileHandle* fHandle, SM_PageHandle memPage)
            Utilize the getBlockPos function to obtain the current position from fHandle. To read the preceding block, deduct one from the current position value and provide it to the readblock function. If everything works, return RC OK.

    # (v) RC readCurrentBlock(SM_FileHandle* fHandle, SM_PageHandle memPage)
           Utilize the getBlockPos function to obtain the current position from fHandle. To read the current block, pass it to the readblock function. If everything is successful, return RC OK.   

    # (vi) RC readNextBlock(SM_FileHandle* fHandle, SM_PageHandle memPage)
            Utilize the getBlockPos function to obtain the current position from fHandle. To read the following block, subtract one from the current position value and provide it to the readblock function. If everything works, return RC OK.

    # (vii) RC readLastBlock(SM_FileHandle* fHandle, SM_PageHandle memPage)
           fHandle should return the total amount of pages. To read the final block, deduct one from the number of pages value and provide it to the readblock function. If everything works, return RC OK.                   


* 3. FUNCTIONS FOR WRITING ON THE BLOCKS
    # (i) RC writeBlock (int pageNum , SM_FileHandle * fHandle , SM_PageHandle memPage)
          Open a file To determine the precise location of where we will write, Fseek uses the offset from memPage as input. We can write to the block using the fwrite function.

    # (ii) RC writeCurrentBlock (SM_FileHandle * fHandle , SM_PageHandle memPage)
           This function is used for opening a file, now using the fwrite function we can right on the current block.
    
    # (iii) RC appendEmptyBlock (SM_FileHandle * fHandle)
            Use a new char* to create a new page in newBlock. Repeat this process until the page is filled with 4096 0 chars. By increasing totalNumPages, the metadata are updated. Use the writeBlock function to add the provided newBlock to a new file.

    # (iv) RC ensureCapacity (int numberOfPages , SM_FileHandle * fHandle )
            Check to check if the number of pages on the disk and in memory differs. Write one empty file to the disk if there are additional memory pages available.
   

### HOW TO EXECUTE THE FILE ###

-> RUN THE MAKEFILE INITIALLY TO SET THE ORDER ON HOW TO EXECUTE THE PROGRAMS AND IN WHAT ORDER IT SHOULD BE EXECUTED
(command) $ make
-> THEN RUN THE TEST FILE
(command)  $ ./test_assign1
 
 Now the program executes the header files, C files and then followed by the test files generating the output files.


