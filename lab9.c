#include <stdio.h>

# define TABLESIZE 200

// RecordType
typedef struct RecordType
{
	int		id;
	char	name;
	int		order; 
} record;

// Fill out this structure
typedef struct HashType
{
	struct node** lists;
	int size;
} hashtable;

typedef struct node {
	record* rPtr;
	struct node* next;
} node;

// Compute the hash function
int hash(int x)
{
	return ((x-1)/2) % TABLESIZE;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	printf("Records in hash table:\n");

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		printLinkedList(pHashArray->lists[i]);
	}
	free(pHashArray->lists);
	free(pHashArray);
}

void printLinkedList(node* head) {
	if (head == NULL)
		return;
	
	if (head->next != NULL)
		printLinkedList(head->next);
	
	printf("\t%d %c %d\n", head->rPtr->id, head->rPtr->name, head->rPtr->order);
	free(head->rPtr);
	free(head);
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	hashtable* h = (hashtable*) malloc(sizeof(hashtable));
	h->lists = (node*) malloc(sizeof(node*)*TABLESIZE);
	h->size = TABLESIZE;
	for (int x = 0; x < h->size; x++)
		h->lists[x] = NULL;

	for (int y = 0; y < recordSz; y++) {
		//printf("%d\n", hash(pRecords[y].id));
		int row = hash(pRecords[y].id);
		node* head = h->lists[row];
		node* entry = (node*) malloc(sizeof(node));
		entry->rPtr = (record*) malloc(sizeof(record));
		record* rPtr = entry->rPtr;
		rPtr->id = pRecords[y].id;
		rPtr->name = pRecords[y].name;
		rPtr->order = pRecords[y].order;
		entry->next = head;
		h->lists[row] = entry;
	}

	displayRecordsInHash(h, h->size);

}