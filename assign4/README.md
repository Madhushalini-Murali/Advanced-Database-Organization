# Assignment 4 - B+ Tree #

## README file for B+ Tree ##

### Execute the below commands to run the B+ Tree program in linux server ###

Open the terminal under the assign 4 folder and start executing the commands in order:

Clean Command
$ make clean

Make command:
$ make  

Run command: 
$ ./test_expr
$ ./test_assign4

# TEAM CONTRIBUTION
* DIKSHITHA VARMAN  (A20521565) -   Index Access Functions
* JAYASURYA (A20513480) - Access functions and 2 functions of Index Access Functions
* MADHUSHALINI MURALI (A20513784) - B+ Tree Functions

## Purpose of the repository ##

* This repository stores the necessary C programming code files to perform operations such as creating , opening, closing, deleting, inserting key , deleting key  and similar functions of a btree.

### B+ Tree Functions ###

#### RC initIndexManager (void *mgmtData)  ####
    1. This function is used to initialize the btree. 

#### RC shutdownIndexManager () #### 
    1. The purpose of this function is to end or close the Index manager.

#### RC createBtree (char *idxId, DataType keyType, int n) #### 
    1. The role of this function is to assign memory to each of the elements in the Btree structure.
    2. It creates a file named idxId that serves as a page file. 

#### RC openBtree (BTreeHandle **tree, char *idxId) #### 
    1. The role of this function is to open the idxID page file. 

#### RC closeBtree (BTreeHandle *tree) #### 
    1. The role of this function is to free all the allocated memory. 

#### RC deleteBtree (char *idxId) #### 
    1. The role of this function is to delete all the records from the given Btree.

### Access Functions ###

#### RC getNumNodes (BTreeHandle *tree, int *result) ####
    1. The role of this function is to return the number of nodes in a Btree.

#### RC getNumEntries (BTreeHandle *tree, int *result)  ####
    1. The role of this function is to return the total number of entries in a Btree.

#### RC getKeyType (BTreeHandle *tree, DataType *result) ####
    1. The role of this function is to return the key type, such as Integer, String and other similar key types in a Btree.

###  Index Access Functions ###

#### RC findKey (BTreeHandle *tree, Value *yrk, RID *result) ####
    1. The role of this function is to return the key that is found.

#### RC insertKey (BTreeHandle *tree, Value *yrk, RID rid) ####
    1. The role of this function is to insert the specified key into the Btree.

#### RC deleteKey (BTreeHandle *tree, Value *yrk) ####
    1. The role of this function is to delete the specified key from a Btree. 

#### RC openTreeScan (BTreeHandle *tree, BT_ScanHandle **handle) ####
    1. The role of this function is to traverse the entire Btree. 

#### RC nextEntry (BT_ScanHandle *handle, RID *result) ####
    1. The role of this function is to read the next entry in the BTree.

#### RC closeTreeScan (BT_ScanHandle *handle) ####
    1. The role of this function is to end or terminate the tree element through scanning.

#### char *printTree(BTreeHandle *tree) ####
    1. This code defines a function named printTree that takes a pointer to a BTreeHandle struct as its parameter and returns a char pointer to print the Btree.

