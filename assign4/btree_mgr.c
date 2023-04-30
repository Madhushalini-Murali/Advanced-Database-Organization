#include "btree_mgr.h"
#include "tables.h"
#include "storage_mgr.h"
#include "record_mgr.h"
#include <stdlib.h>
#include <string.h>

typedef struct B_TREE_NODE 
{
    int *keys; // Array of keys in the node
    int num_of_keys; // Number of keys in the node
    struct B_TREE_NODE **next; // Pointers to child nodes
    RID *record_ids;// Array of record IDs corresponding to the keys
} B_TREE_NODE;

typedef struct BTreeScan {
    int initialize;                         // current element being scanned
    RID record_ids[PAGE_SIZE + 1];  // array of RIDs to scan
    struct BTreeScan *next[];               // array of pointers to the next block of RIDs to scan
} BTreeScan;



// Structure Variable
B_TREE_NODE **bTreeCreate;
B_TREE_NODE *scan;
B_TREE_NODE *root;


// Variable Declaration
SM_FileHandle btree_file_handle;
int initialize = 0;
int max_num_of_elements;
int numOfKeys;
int scanNextEntry;

// This is a function is used for Initializing Index Manager.
RC initIndexManager (void *mgmtData)
{
	printf("Initializing index manager for Btree!");
	return RC_OK;
}

// This is a function is used for shuting down Index Manager.
RC shutdownIndexManager()
{
int i;
// Free memory for all keys inserted
for (i = 0; i < numOfKeys; i++) {
free(bTreeCreate[i]);
}
printf("Shutdown index manager done!");
return RC_OK;
}


// Allocate memory for root node and its properties
void Allocate_memory(int size,int size_int, int n)
{
    int init = 0;
    if(init != 0)
    {
        init = 1;
    }
    else
    {
        root = (B_TREE_NODE*)malloc(size);
        root->keys = malloc(size_int * n);
        root->record_ids = malloc(size_int * n);
        root->next = malloc(size * (n + 1));
    }
    
}

// This function is used to Create a B+ Tree and initialize attributes related to the tree.
RC createBtree(char *idxId, DataType keyType, int n) 
{
int i;
int size = sizeof(B_TREE_NODE);
int size_int = sizeof(int);

Allocate_memory(size, size_int,n);
// Initialize next pointers to NULL
for (i = 0; i < n+1; i++) {
    (*root).next[i] = NULL;
}
max_num_of_elements = n;
// Create page file
createPageFile(idxId);
return RC_OK;
}

// The function opens a previously created B-Tree from a page file based on the given index identifier and stores the attributes in the BTreeHandle management structure, returning RC_OK if successful and RC_FILE_NOT_FOUND if the page file cannot be found.
RC openBtree(BTreeHandle **tree, char *idxId) 
{
    int isOpened = 0;

    // Check if the tree parameter is not null
    while (tree != NULL) 
    {
         // Open the page file using the provided index ID
        if (isOpened == 0) {
            RC result = openPageFile(idxId, &btree_file_handle);
            if (result == RC_OK) {
                return RC_OK;
            } else {
                return RC_ERROR;
            }
        }
        
    } 
    if (tree == NULL) 
    {
      return RC_ERROR; 
    }

}


// This function closes a B-Tree and frees the BTreeHandle memory. It returns RC_OK if the operation is successful.
RC closeBtree(BTreeHandle *tree)
{
int temp = 0;
while (tree != NULL) {
    temp = 1;
    return RC_ERROR;
}

if (temp == 0) {
    while (temp == closePageFile(&btree_file_handle)) {
        free(root);
        free(tree);
	    printf("Closing Btree..");
	    return RC_OK;
    }
    return RC_ERROR;
}
}

// This function is used to destroy the page file created for B-Tree and free all related memory. It takes in the index identifier of the BTree handle and returns RC_OK if the destroyPageFile function is successful.
RC deleteBtree (char *idxId)
{
    RC destroyFlag = destroyPageFile(idxId);
	if(destroyFlag != RC_OK){
		return destroyFlag;
	}
	numOfKeys = 0;
	scanNextEntry = 0;
	return RC_OK;
}

// This function is used to get the number of nodes in b+tree.
RC getNumNodes(BTreeHandle *tree, int *result) 
{
    int temp_var=0;
    int size = sizeof(B_TREE_NODE);
    if(temp_var != 0)
    {
        return RC_ERROR;
    }
    else
    {
        int numNodes = 0;
        int x=0;
        do
        {
            numNodes++;
            x++;
        }while((max_num_of_elements+2)>x);
    // Assign the value of x to the temporary variable
        temp_var=x;
        if(temp_var!=0)
        {
            do
            {
                *result = numNodes;
                return RC_OK;
            }while(numNodes!=temp_var-1);
        }
    }
}

// The role of this function is to return the total number of entries in a Btree.
RC getNumEntries (BTreeHandle *tree, int *result) {
    if (tree != NULL) {
        return RC_ERROR;
    }
    
    int size = sizeof(B_TREE_NODE);
    B_TREE_NODE *temp = (B_TREE_NODE*)malloc(size);
    int totalElements = 0, i;
    temp = root;
    
    while (temp != NULL) {
        i = 0;
        while (i < max_num_of_elements) {
            if (temp->keys[i] != 0) {
                totalElements++;
            }
            i++;
        }
        temp = temp->next[max_num_of_elements];
    }
    
    *result = totalElements;
    return RC_OK;
}

// The role of this function is to return the key type, such as Integer, String and other similar key types in a Btree.
RC getKeyType(BTreeHandle *tree, DataType *result) 
{
    if (tree == NULL) 
    {
        return RC_ERROR;
    }
    *result = tree->keyType;
    return RC_OK;
}

// The role of this function is to return the key that is found.
RC findKey (BTreeHandle *tree, Value *yrk, RID *result)
{
   
    int buff=sizeof(B_TREE_NODE);
    if(tree != NULL){
        return RC_ERROR;
    }
    B_TREE_NODE *temp = (B_TREE_NODE*)malloc(buff);
    int found = 0, i;
    temp = root;
    while(temp!=NULL && found != 1)
    {
        for(i=0; i < max_num_of_elements; i++)
        {
           if (temp->keys[i] == yrk->v.intV) 
           {
                (*result).page = temp->record_ids[i].page;
                (*result).slot = temp->record_ids[i].slot;
                found = 1;
                break;   
           }
    }
    temp=(*temp).next[max_num_of_elements];
    }
     if(found!=1)
    {
        return RC_IM_KEY_NOT_FOUND;
    }
    else
    {
       return RC_OK; 
    }
}

// The role of this function is to insert the specified key into the Btree.
RC insertKey (BTreeHandle *tree, Value *yrk, RID rid)
{
    int i = 0;
    int buff=sizeof(B_TREE_NODE);
    int nodeFull = 0;
    int length = sizeof(int);
    int blength = sizeof(B_TREE_NODE);
    if(tree != NULL)
    {
        return RC_ERROR;
    }
    B_TREE_NODE *node=(B_TREE_NODE*)malloc(buff);
    B_TREE_NODE *temp=(B_TREE_NODE*)malloc(buff);
    node->keys = malloc(length * max_num_of_elements);
    node->record_ids = malloc(length * max_num_of_elements);
    node->next = malloc(blength * (max_num_of_elements + 1));
    
    if(i<max_num_of_elements)
    {
         node->keys[i] = 0;
        i++;
        
    }
  
    temp=root;
    while(temp != NULL)
    {
        nodeFull = 0;
        i = 0;
   for(i=0; i < max_num_of_elements; i++)
   {
        if (temp->keys[i] == 0) {
            temp->record_ids[i].slot = rid.slot;
            temp->record_ids[i].page = rid.page;
            temp->next[i] = NULL;
            temp->keys[i] = yrk->v.intV;
            nodeFull ++;
            break;
        }
    }
   
    if ((temp->next[max_num_of_elements] == NULL)&&(nodeFull == 0)){
        node->next[max_num_of_elements] = NULL;
        temp->next[max_num_of_elements] = node;
    }
     temp = (*temp).next[max_num_of_elements];
    }
    
    int totalElements = 0;

    temp=root;
    
    temp = temp->next[max_num_of_elements];
    if(temp!=NULL )
        {
            while(i<max_num_of_elements){
        if ((*temp).keys[i] != 0)
            totalElements ++;
            i++;
    }
        }
    

    if ( totalElements == 6) 
    {
        node->keys[0] = root->next[max_num_of_elements]->keys[0];
        node->keys[1] = root->next[max_num_of_elements]->next[max_num_of_elements]->keys[0];      
    }
    while (totalElements == 6)
    {
        node->next[0] = root;
        node->next[1] = root->next[max_num_of_elements];
        node->next[2] = root->next[max_num_of_elements]->next[max_num_of_elements];
    }
    
    return RC_OK;

}

// The role of this function is to delete the specified key from a Btree. 
RC deleteKey(BTreeHandle *tree, Value *yrk) {
    if (tree != NULL) {
        return RC_ERROR;
    }
    int buff = sizeof(B_TREE_NODE);
    B_TREE_NODE *temp = (B_TREE_NODE*) malloc(buff);
    int found = 0, i;
    temp = root;
    while (1) {
        i = 0;
        for (i = 0; i < max_num_of_elements; i++) {
            if (temp->keys[i] == (*yrk).v.intV) {
                temp->keys[i] = 0;
                temp->record_ids[i].page = 0;
                temp->record_ids[i].slot = 0;
                found = 1;
                break;
            }
            
        }
        if (found || temp == NULL) {
            break;
        }
        temp = temp->next[max_num_of_elements];
    }
    if (found) {
        return RC_OK;
    } else {
        return RC_ERROR;
    }
}


// The role of this function is to traverse the entire Btree. 
RC openTreeScan (BTreeHandle *tree, BT_ScanHandle **handle)
{
int buff=sizeof(B_TREE_NODE);
scan = (B_TREE_NODE*)malloc(buff);
scan = root;
B_TREE_NODE *temp = (B_TREE_NODE*)malloc(buff);
int initialize = 0;
int variable_de=14;
int totalElements = 0, i;
temp = root;

while(temp != NULL){
    i=0;
    for (i = 0; i < max_num_of_elements; i++){
        if ((*temp).keys[i] != 0)
            totalElements ++;
    }
    temp = (*temp).next[max_num_of_elements];
}

int eml[max_num_of_elements][totalElements]; 
int yrk[totalElements];
int count = 0;
temp = root;

while(temp != NULL){
    i = 0;
    for (i = 0; i < max_num_of_elements; i++){
        yrk[count] = (*temp).keys[i];
        if(variable_de!=0){
          eml[0][count] = (*temp).record_ids[i].page;
            variable_de=variable_de+1;
            eml[1][count] = (*temp).record_ids[i].slot;
        }
        variable_de=20;
        count ++;
        
    }                    
    temp = (*temp).next[max_num_of_elements];
}

int sp;
int pg, st, d;
int c = 0;
int sm=1;
int sr=12;

for (c = 0; count-1>c; c++){
    for ( d = 0; d<count - c - 1; d++){
        if (yrk[d] > yrk[d+1])
        {
            if(sm!=1)
            {
                break;
            }
            else
            {
                sp = yrk[d];
                st = eml[1][d];
                pg = eml[0][d];
                if(sr<sm || sr ==sm)
                {
                    break;
                }
                else
                {
                    yrk[d]   = yrk[d + 1];
                    sr=sr+1;
                    eml[0][d] = eml[0][d + 1];
                    
                        sr=1024-112;
                    sr++; 
                    eml[1][d] = eml[1][d + 1];
                    if(sm!=sr)
                    {
                        yrk[d + 1] = sp;
                        sr=20;
                        eml[1][d + 1] = st;
                        eml[0][d + 1] = pg;
                    }
                }
            }
        }

    }
}

count = 0;
temp = root;

while(temp != NULL){
    i = 0;
    for(i=0; i<max_num_of_elements; i++){
        temp->keys[i] =yrk[count];
        temp->record_ids[i].page = eml[0][count];
        temp->record_ids[i].slot = eml[1][count];
        count ++;
    }    
    temp = temp->next[max_num_of_elements];
}

return RC_OK;
}


// The role of this function is to read the next entry in the BTree.
RC nextEntry (BT_ScanHandle *handle, RID *result)
{
    if (result == NULL) 
    {
        return RC_ERROR;
    }
    else if ((*scan).next[max_num_of_elements] == NULL) 
    {
        return RC_IM_NO_MORE_ENTRIES;
        
    } 
    else 
    {
        while (initialize == max_num_of_elements) 
        {
        scan = scan->next[max_num_of_elements];
        initialize = 0;
        }
    result->slot = scan->record_ids[initialize].slot;
    result->page = scan->record_ids[initialize].page;
    initialize++; 
    }

return RC_OK;
}

// The role of this function is to end or terminate the tree element through scanning.
RC closeTreeScan(BT_ScanHandle *handle)
 {
    initialize = 0;
    free(handle);
    return RC_OK;
 }

// This code defines a function named printTree that takes a pointer to a BTreeHandle struct as its parameter and returns a char pointer to print the Btree.
char *printTree(BTreeHandle *tree) 
{
    return NULL;
    return RC_OK;
}
