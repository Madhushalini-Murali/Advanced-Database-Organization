#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "buffer_mgr.h"
#include "storage_mgr.h"
#include "record_mgr.h"
#include "expr.h"

// Details of the Table.
typedef struct
{
	int tuples_count;
	int sizeOfSchema;
} DetailsOfTable;

// Struct for Record Manager.
typedef struct
{
	BM_BufferPool *bm_pool_buffer;
	int *pages_free;
} ManageRecord;

// Struct for Scan Manager.
typedef struct
{
	Record *present_record;
	int present_slot;
	Expr *rm_expr;
	int present_page;
} ScanMgr_RM;

bool condition = true;
int pages_in_total;

// This method, which calls dirty info, unpin page, and force page functions, takes no inputs and returns void.
void ModifyPageDetails(RM_TableData *rm_tbdata, BM_PageHandle *bm_page)
{
	markDirty(((ManageRecord *)rm_tbdata->mgmtData)->bm_pool_buffer, bm_page);
	unpinPage(((ManageRecord *)rm_tbdata->mgmtData)->bm_pool_buffer, bm_page);
	forcePage(((ManageRecord *)rm_tbdata->mgmtData)->bm_pool_buffer, bm_page);
}

RC AttributeVal_SetOff(Schema *rm_schema, int attribute_num, int *rm_result)
{
	int current_offset = 0;
	bool should_update_offset = true;

	for (int i = 0; i < attribute_num; i++)
	{
		if (rm_schema->dataTypes[i] == DT_INT && should_update_offset)
		{
			current_offset += sizeof(int);
		}
		else if (rm_schema->dataTypes[i] == DT_BOOL && should_update_offset)
		{
			current_offset += sizeof(bool);
		}
		else if (rm_schema->dataTypes[i] == DT_STRING && should_update_offset)
		{
			current_offset += rm_schema->typeLength[i];
		}
		else if (rm_schema->dataTypes[i] == DT_FLOAT && should_update_offset)
		{
			current_offset += sizeof(float);
		}
	}

	*rm_result = current_offset;
	return RC_OK;
}

// This method initializes the record manager, takes no inputs, and returns an RC code.
RC initRecordManager(void *mgmtData)
{
	if (condition)
	{
		printf("Entering initRecordManager function\n");
		printf("Record Manager initialization successful!\n");
		return RC_OK;
	}
}

// This method shuts down the record manager, takes no inputs, and returns an RC code.
RC shutdownRecordManager()
{
	if (condition)
	{
		printf("Entering shutdownRecordManager function\n");
		printf("Record Manager shutdown successful!\n");
		return RC_OK;
	}
}

// This method create a table in the record manager, takes inputs as schema and name, and returns an RC code.
RC createTable(char *name, Schema *schema)
{
	int count = 0;
	SM_FileHandle sm_handlefile;
	char *data = serializeSchema(schema);

	DetailsOfTable *detailsofTable = (DetailsOfTable *)malloc(sizeof(DetailsOfTable));

	RC createPageReturn = createPageFile(name);
	RC openPageReturn = openPageFile(name, &sm_handlefile);
	// validateFileExist(createPageReturn, openPageReturn);
	if (createPageReturn == RC_OK && openPageReturn == RC_OK)
	{
		printf("\n The files are found -Success!!");
	}
	else if (createPageReturn != RC_OK || openPageReturn != RC_OK)
	{
		return RC_FILE_NOT_FOUND;
	}
	else
	{
		return RC_OK;
	}

	detailsofTable->sizeOfSchema = count;
	RC writeflag = writeBlock(count, &sm_handlefile, data);
	if (writeflag == RC_OK)
	{
		return RC_OK;
	}
	else
	{
		return RC_WRITE_FAILED;
	}
	return RC_OK;
}

// Total page value is set from the read Header
char init_totalPages(char *read) { return (true) ? atoi(read) : '\0'; }

// This method is used to open a table that is already created, takes inputs as table data and name, and returns an RC code.
RC openTable(RM_TableData *rm_tbdata, char *name)
{
	// Print log message
	printf("Opening table...\n");

	// Initialize record manager and read header from file
	ManageRecord *recordManager = (ManageRecord *)malloc(sizeof(ManageRecord));
	FILE *required_file = fopen(name, "r+");
	char *readHeader = (char *)calloc(PAGE_SIZE, sizeof(char));
	fgets(readHeader, PAGE_SIZE, required_file);

	// Extract total number of pages from header and initialize buffer pool
	pages_in_total = init_totalPages(readHeader);
	recordManager->bm_pool_buffer = MAKE_POOL();
	BM_PageHandle *bm_page = MAKE_PAGE_HANDLE();
	initBufferPool(recordManager->bm_pool_buffer, name, 6, RS_FIFO, NULL);
    if(condition)
	{
    // Pin first page of table and initialize record manager data
	pinPage(recordManager->bm_pool_buffer, bm_page, 0);
	recordManager->pages_free = (int *)malloc(sizeof(int));
	recordManager->pages_free[0] = pages_in_total;

	// Set table metadata and free memory
	rm_tbdata->name = name;
	rm_tbdata->schema = deserializeSchema(bm_page->data);
	rm_tbdata->mgmtData = recordManager;
	free(bm_page);
	free(readHeader);

	}
	
	// Print log message
	printf("Table opened successfully!\n");

	return RC_OK;
}

// This method will free all the attribute once the table has been closed.
void freeAttr(ManageRecord *recordManager, RM_TableData *rm_tbdata)
{
	free(recordManager);
	free(rm_tbdata->schema->attrNames);
	free(rm_tbdata->schema->dataTypes);
	free(rm_tbdata->schema->keyAttrs);
	free(rm_tbdata->schema->typeLength);
	free(rm_tbdata->schema);
}

// This method is used to close a table that is already created, takes inputs as table data, and returns an RC code.
RC closeTable(RM_TableData *rm_tbdata)
{
	printf("Closing table...\n");
	ManageRecord *recordManager = (ManageRecord *)malloc(sizeof(ManageRecord));
	recordManager = rm_tbdata->mgmtData;
	// Shutdown buffer pool and free all attributes
	shutdownBufferPool(recordManager->bm_pool_buffer);
	freeAttr(recordManager, rm_tbdata);
	printf("Table closed.\n");
	return RC_OK;
}

// This method is used to delete a table that is already exists, takes inputs as name, and returns an RC code.
RC deleteTable(char *name)
{
    printf("delete table is started\n");
    RC df = destroyPageFile(name);
    if (df != RC_OK) {
        printf("delete table failed: file not found\n");
        return RC_FILE_NOT_FOUND;
    }
    printf("delete table is ended\n");
    return RC_OK;
}

// This method is used to get the tuple value, takes inputs as name of the table, and returns an RC code.
int getNumTuples(RM_TableData *rm_tbdata)
{
	Record *record = (Record *)malloc(sizeof(Record));
	RID ridValue;
	RC flagGetRecord;
	int total = 0;
	ridValue.page = 1;
	ridValue.slot = 0;
	while (ridValue.page < pages_in_total && ridValue.page > 0)
    {
		flagGetRecord = getRecord(rm_tbdata, ridValue, record);
		if (flagGetRecord == RC_OK)
		{
			total += 1;
			ridValue.page += 1;
			ridValue.slot = 0;
		}
		else
		{
			break;
		}
	}
	record = NULL;
	free(record);
	return total;
}

// This method is used to insert the record values into the table, takes inputs as table data and record object, and returns an RC code.
RC insertRecord(RM_TableData *rm_tbdata, Record *record)
{
    printf("Insert Record is started\n");
	Record *record1 = (Record *)malloc(sizeof(Record));
    // Find the first free slot to insert the record
    RID rid;
    rid.page = 1;
    rid.slot = 0;

    while (rid.page < pages_in_total && rid.page > 0)
    {
        rid.page = rid.page + 1;
        rid.slot = 0;
    }
	record1 = NULL;
	free(record1);
    int freepage = ((ManageRecord *)rm_tbdata->mgmtData)->pages_free[0];
    // Update the record's ID to the first free slot
    record->id.page = freepage;
    record->id.slot = 0;

    // Serialize the record
    Schema *schema = rm_tbdata->schema;
    char *serializedRecord = serializeRecord(record, schema);

    // Pin the page where the record will be inserted
    BM_BufferPool *bm_pool_buffer = ((ManageRecord *)rm_tbdata->mgmtData)->bm_pool_buffer;
    BM_PageHandle *bm_page = MAKE_PAGE_HANDLE();
    pinPage(bm_pool_buffer, bm_page, freepage);

    // Write the serialized record to the page
    char *dt = bm_page->data;
    memset(dt, '\0', strlen(dt));
    sprintf(bm_page->data, "%s", serializedRecord);

    // Mark the page as dirty and unpin it
    markDirty(bm_pool_buffer, bm_page);
    unpinPage(bm_pool_buffer, bm_page);

    // Update the table metadata
    ((ManageRecord *)rm_tbdata->mgmtData)->pages_free[0]++;
    pages_in_total++;

    // Free resources and return
    free(serializedRecord);
    free(bm_page);
    printf("Insert Record is ended\n");
    return RC_OK;
}

// This method is used to delete records from a previously existing table, takes inputs as table data and record id, and returns an RC value.
RC deleteRecord(RM_TableData *rm_tbdata, RID id)
{
    const char *deleteFlag = "DEL";
	char *flag = (char *)malloc(sizeof(char *));
    int zero = 0;

    if (id.page < zero || id.page >= pages_in_total)
    {
        return RC_RM_NO_MORE_TUPLES;
    }
    else
    {
        BM_PageHandle *bm_page = MAKE_PAGE_HANDLE();
        BM_BufferPool *bm_pool_buffer = ((ManageRecord *)rm_tbdata->mgmtData)->bm_pool_buffer;
        pinPage(bm_pool_buffer, bm_page, id.page);
        char *dt = bm_page->data;
        // Mark the record as deleted
        memset(dt, '\0', strlen(dt));
		sprintf(bm_page->data, "%s", flag);
        ModifyPageDetails(rm_tbdata, bm_page);
        free(bm_page);
        return RC_OK;
    }
}

// This method is used to update records from a previously existing table, takes inputs as table data and record object, and returns an RC value.
RC updateRecord(RM_TableData *rm_tbdata, Record *record)
{
    printf("update record is started\n");
    printf("record updated: %s\n", record->data);
    
    if (record->id.page < pages_in_total || record->id.page >= 0)
    {
        BM_PageHandle *bm_page = MAKE_PAGE_HANDLE();
        BM_BufferPool *bm_pool_buffer = ((ManageRecord *)rm_tbdata->mgmtData)->bm_pool_buffer;
        char *record_str = serializeRecord(record, rm_tbdata->schema);
        int pg = record->id.page;
        pinPage(bm_pool_buffer, bm_page, pg);
        char *dt = bm_page->data;
        memset(dt, '\0', strlen(dt));
        sprintf(bm_page->data, "%s", record_str);
        ModifyPageDetails(rm_tbdata, bm_page);
        
        free(record_str);
        free(bm_page);
        
        return RC_OK;
		
    }
    else
    {
        return RC_RM_NO_MORE_TUPLES;
    }
	printf("update record is ended\n");
}

// This method is used to get the records from a previously existing table with rid value, takes inputs as table data, record and record id, and returns an RC value.
RC getRecord(RM_TableData *rm_tbdata, RID id, Record *record)
{
    printf("get record is started\n");
    if (id.page > pages_in_total && id.page <= 0)
		return RC_RM_NO_MORE_TUPLES;

    BM_PageHandle *bm_page = MAKE_PAGE_HANDLE();
    BM_BufferPool *bm_pool_buffer = ((ManageRecord *)rm_tbdata->mgmtData)->bm_pool_buffer;
    pinPage(bm_pool_buffer, bm_page, id.page);

    char *data = bm_page->data;
    char *flag = (char *)malloc(sizeof(char) * 3);
    strncpy(flag, data, 3);

    if (strcmp(flag, "DEL") == 0)
    {
        printf("Record has been deleted\n");
        return RC_RM_NO_MORE_TUPLES;
    }

    char *record_data = (char *)malloc(sizeof(char) * strlen(data));
    strncpy(record_data, data, strlen(data));

    Schema *schema = rm_tbdata->schema;
    Record *deSerializedRecord = deserializeRecord(record_data, schema);

    record->id = id;
    record->data = deSerializedRecord->data;

    unpinPage(bm_pool_buffer, bm_page);
    free(deSerializedRecord);
    free(record_data);
    free(flag);

    printf("get record is ended\n");

    return RC_OK;
}


// This method is used to scan the records from a previously existing table and returns an RC value.
RC startScan(RM_TableData *rm_tbdata, RM_ScanHandle *scan, Expr *cond)
{
    // Allocate memory for the scan manager and initialize its fields
    ScanMgr_RM *scanManager = (ScanMgr_RM *)malloc(sizeof(ScanMgr_RM));
    scanManager->present_record = (Record *)malloc(sizeof(Record));
    scanManager->present_slot = 0;
    scanManager->present_page = 1;
    scanManager->rm_expr = cond;
    
    // Assign the scan manager and the relation to the scan handle
    scan->rel = rm_tbdata;
    scan->mgmtData = scanManager;
    
    return RC_OK;
}

// This method is used to scan to the next record from a previously existing table and returns an RC value.
RC next(RM_ScanHandle *scan, Record *record)
{
	Value *result;
	RID rid;
	rid.slot = ((ScanMgr_RM *)scan->mgmtData)->present_slot;
	rid.page = ((ScanMgr_RM *)scan->mgmtData)->present_page;
	Expr *rm_expr = ((ScanMgr_RM *)scan->mgmtData)->rm_expr;

	while (rm_expr == NULL && rid.page < pages_in_total && rid.page > 0)
		{
			Record *rd1 = ((ScanMgr_RM *)scan->mgmtData)->present_record;
			RM_TableData *rmTD1 = scan->rel;
			getRecord(rmTD1, rid, rd1);
			record->id = rd1->id;
			record->data = rd1->data;
			rid.page = rid.slot + 1;
			return RC_OK;
		}

	while (rid.page < pages_in_total && rid.page > 0)
		{
			Record *rd = ((ScanMgr_RM *)scan->mgmtData)->present_record;
			RM_TableData *rmTD = scan->rel;
			getRecord(rmTD, rid, rd);
			evalExpr(rd, rmTD->schema, rm_expr, &result);
			if (!(result->v.boolV) || result->dt != DT_BOOL)
			{
				((ScanMgr_RM *)scan->mgmtData)->present_page = rid.page + 1;
				rid.slot = ((ScanMgr_RM *)scan->mgmtData)->present_slot;
				rid.page = ((ScanMgr_RM *)scan->mgmtData)->present_page;
				
			}
			else
			{
				record->id = rd->id;
				record->data = rd->data;
				((ScanMgr_RM *)scan->mgmtData)->present_page = rid.page + 1;
				return RC_OK;
				
			}
		}
	rid.page = 1;
	return RC_RM_NO_MORE_TUPLES;
}

// This method is used to close the scan that was started to access the records from a previously existing table and returns an RC value.
RC closeScan(RM_ScanHandle *scan) {
    printf("Closing the scan\n");
    free(((ScanMgr_RM*)scan->mgmtData)->present_record);
    free(scan->mgmtData);
    scan->mgmtData = NULL;
    scan = NULL;
    free(scan);
    return RC_OK;
}

// This method is used to get the size of the record, getting the input as schema object and return the value of record size.
int getRecordSize(Schema *schema)
{
	int x,size,attr;
	size = 0;
	attr = schema->numAttr;
	for (x = 0; x < attr; x++)
	{
		if (schema->dataTypes[x] == DT_INT && condition)
		{
			size += sizeof(int);
		}
		else if (schema->dataTypes[x] == DT_FLOAT && condition)
		{
			size += sizeof(float);
		}
		else if (schema->dataTypes[x] == DT_BOOL && condition)
		{
			size += sizeof(bool);
		}
		else
		{
				size += schema->typeLength[x];
		}
	}
	return size;
}



// This method is used to create database schema, getting the input as schema related properties and returns newly created schema.
Schema *createSchema(int numAttr, char **attrNames,DataType *dataTypes, int *typeLength, int keySize, int *keys)
{
	Schema *schema = (Schema *)malloc(sizeof(Schema));
	int numAttribute, sizeofkey;
	int *k = keys;
	char **atn = attrNames;
	int *len = typeLength;

	numAttribute = numAttr;
	sizeofkey = keySize;
	
	if(condition)
	{
		schema->attrNames = atn;
		schema->typeLength = len;
		schema->numAttr = numAttribute;
		schema->keyAttrs = k;
		schema->dataTypes = dataTypes;
		schema->keySize = sizeofkey;
	}
	return schema;
}

//This method is used to free a database schema. It takes a schema object as input and returns an RC code once the schema has been freed.
RC freeSchema(Schema *schema)
{
	if (schema != NULL)
	{
		free(schema);
	}
	else
	{
		return NULL;
	}
	return RC_OK;
}

//This method is used to create Record for a database schema. It takes a schema object and record object as input and returns an RC code once the record has been created.
RC createRecord(Record **rec, Schema *schema)
{
    printf("Creating record...\n");
    
    // Allocate memory for the record
    *rec = (Record *) malloc(sizeof(Record));
    if (*rec == NULL) {
        return RC_RM_NO_MORE_TUPLES;
    }
    
    // Get the size of the record from the schema
    int size = getRecordSize(schema);
    
    // Allocate memory for the data in the record
    (*rec)->data = (char *) malloc(size);
    if ((*rec)->data == NULL) {
        return RC_RM_NO_MORE_TUPLES;
    }
    
    // Initialize the data in the record to 0
    memset((*rec)->data, 0, size);
    
    printf("Record created successfully.\n");
    return RC_OK;
}

//This method is used to free the Record created. It takes a record object as input and returns an RC code once the record has been freed.
RC freeRecord(Record *record)
{
	printf("free record is started\n");
	if (record != NULL)
	{
        record = NULL;
		free(record);
		printf("free record is ended\n");
		return RC_OK;
	}
	else
	{
		return RC_NULL;
	}
}

RC getAttr(Record *record, Schema *schema, int attrNum, Value **val)
{
	
	int a, l,memalloc;
	char *attribute, *buffer;
	char *dt = record->data;
	AttributeVal_SetOff(schema, attrNum, &a);
	attribute = dt + a;
	*val = (Value *)malloc(sizeof(Value));
	(*val)->dt = schema->dataTypes[attrNum];
	switch (schema->dataTypes[attrNum])
	{
	case DT_INT:
			memcpy(&((*val)->v.intV), attribute, sizeof(int));
		break;
	case DT_STRING:
			l = schema->typeLength[attrNum];
			memalloc = l + 1;
			buffer = (char *)malloc(memalloc);
			strncpy(buffer, attribute, l);
			buffer[l] = '\0';
			(*val)->v.stringV = buffer;
		break;
	case DT_FLOAT:
			memcpy(&((*val)->v.floatV), attribute, sizeof(float));
		break;
	case DT_BOOL:
			memcpy(&((*val)->v.boolV), attribute, sizeof(bool));
		break;
	default:
			return RC_RM_NO_DESERIALIZER_FOR_THIS_DATATYPE;
		break;
	}

	return RC_OK;
}


RC setAttr(Record *record, Schema *schema, int attrNum, Value *value)
{
    int offset;
    char *attrData;
    int len;
    char *buf;
    AttributeVal_SetOff(schema, attrNum, &offset);

    attrData = record->data + offset;

    switch (schema->dataTypes[attrNum])
    {
        case DT_INT:
            memcpy(attrData, &(value->v.intV), sizeof(int));
            break;
        case DT_STRING:
            len = schema->typeLength[attrNum];
            buf = (char *)malloc(len + 1);
            memcpy(buf, value->v.stringV, len);
            buf[len] = '\0';
            memcpy(attrData, buf, len + 1);
            free(buf);
            break;
        case DT_FLOAT:
            memcpy(attrData, &(value->v.floatV), sizeof(float));
            break;
        case DT_BOOL:
            memcpy(attrData, &(value->v.boolV), sizeof(bool));
            break;
        default:
            return RC_RM_NO_DESERIALIZER_FOR_THIS_DATATYPE;
    }

    return RC_OK;
}
