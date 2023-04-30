# README file for record manager #

### Execute the below commands to run the record manager program in linux server ###

Open the terminal under the assign 3 folder and start executing the commands in order:

Make command:
$ make                                                                                     
Run command: 
$ ./recordmanager

# TEAM CONTRIBUTION
* DIKSHITHA VARMAN  (A20521565) -  dealing with scans (like startscan, closescan, etc)
* JAYASURYA (A20513480) - dealing with attribute values and records (like getattr, setattr,etc)
* MADHUSHALINI  (A20513784) - handling records in a table,dealing with schemas  (like createtable, opentable,etc)


## Purpose of the repository ##

* This repository stores the necessary C programming code files to perform operations such as adding, deleting, updating, and searching through records in a table.
* Version 1
* It is currently in Version 1 and includes C test cases, header files, output files, a make file, and a ReadMe file.

### Functions Implemented ###

##### extern RC initRecordManager (void *mgmtData) #####
1. Memory is assigned, and the record manager is set up.
2. Once the initialization is complete, the function returns RC_OK.

##### extern RC shutdownRecordManager () #####
1. Record manager is shut down.
2. Once the shut down process is complete, the function returns RC_OK.

##### extern RC createTable (char *name, Schema *schema) #####
1. This method is used for creating a table.
2. It creates a page file and saves the schema information within it.
3. The input parameters for this method are the name and schema of the table.
4. The function returns an RC value based on the operation performed.

##### extern RC openTable (RM_TableData *rel, char *name) #####
1. This method is used to access an existing table.
2. It takes the table data and name as input parameters.
3. The function returns an RC code based on the operation performed.
4. If the table is successfully opened, the method returns RC_OK.

##### extern RC deleteTable (char *name) #####
1. This method is used to delete a table after all operations have been completed.
2. The input parameter for this method is the table name.
3. The function returns an RC code based on the operation performed.
4. If the table is successfully deleted, the method returns RC_OK.

##### extern RC closeTable (RM_TableData *rel) #####
1. This method is used to close a table after all operations have been completed.
2. The input parameter for this method is the table data.
3. The function returns an RC code based on the operation performed.
4. If the table is successfully closed, the method returns RC_OK.

##### extern int getNumTuples (RM_TableData *rel) #####
1. Method gets the table tuple value
2. Takes table data as input
3. Returns the tuple value as integer

##### extern RC insertRecord (RM_TableData *rel, Record *record) #####
1. This method is used to insert records into a created table.
2. The input parameters for this method are the table data and record information.
3. The function returns an RC code based on the operation performed.
4. If the record is successfully inserted, the method returns RC_OK.

##### extern RC deleteRecord (RM_TableData *rel, RID id) #####
1. This method is used to delete records from a created table.
2. The input parameters for this method are the table data and record ID.
3. The function returns an RC code based on the operation performed.
4. If the record is successfully deleted, the method returns RC_OK.

##### extern RC updateRecord (RM_TableData *rel, Record *record) #####
1. Method updates records into the table once created
2. Takes table data and record info as input
3. Returns the RC code based on the operation performed
4. If the record is updated sucessfully, the method retuns RC_OK

##### extern RC getRecord (RM_TableData *rel, RID id, Record *record) #####
1. This method is used to retrieve records from a table once they have been inserted.
2. The input parameters for this method are the table data, record information, and record ID.
3. The function returns an RC code based on the operation performed.
4. If the record is successfully retrieved, the method returns RC_OK.

##### startScan (RM_TableData *rel, RM_ScanHandle *scan, Expr *cond) #####
1. this method is used for initializing the RM_ScanMgmt and RM_ScanHandle.

##### next (RM_ScanHandle *scan, Record *record) #####
1. The 'next' method searches for tuples based on a specified scan condition. 
2. If no scan condition is specified, it returns all tuples in the table. 
3. The tuples are evaluated based on the scan condition using EvalExpr() from expr.c. 
4. This continues until there are no more tuples in the table.

##### closeScan (RM_ScanHandle *scan) #####
1. This method notifies the record manager that it can clean up all the resources.
2. Any memory allocated during scans will be freed.

##### extern int getRecordSize (Schema *schema) #####
1. This function retrieves the size of a record from the provided schema.
2. The input for the function is the schema data.
3. The size of the record is returned as an integer value.

##### extern Schema *createSchema (int numAttr, char **attrNames, DataType *dataTypes, int *typeLength, int keySize, int *keys) #####
1. The method takes input parameters needed to create a schema object.
2. The input parameters include the number of attributes, attribute names, data types, type length, key size, and keys.
3. The method returns the created schema object.

##### extern RC freeSchema (Schema *schema) #####
1. The method is used to free the schema after all the operations have been performed
2. The method takes the schema data as input
3. The method returns the RC code based on the operation performed
4. If the record is successfully freed, the method returns RC_OK.

##### createRecord (Record **record, Schema *schema) #####
1. This method is used for  creating a record.
2. The data inside a record is allocated with a memory.

##### freeRecord (Record *record) #####
1. This method is used for freeing the memory allocated for a record.

##### getAttr (Record *record, Schema *schema, int attrNum, Value **value) #####
1. This method is used for retrieving the  attribute values of a record.

##### setAttr (Record *record, Schema *schema, int attrNum, Value *value) #####
1. This method is used for setting attribute values of a record.

#### Addtional Extensions  (Optional) ####
1. The implementation of tombstones will involve using deleteFlag and adding a prefix to the record after deletion.