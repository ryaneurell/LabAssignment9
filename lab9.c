#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

// Fill out this structure
struct HashType
{
    struct RecordType** records; // Array of pointers to records
    int count; // Number of records in this bucket
};

// Compute the hash function
int hash(int x)
{
    // Simple hash function
    return x % 10;
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
    int i, j;

    for (i=0; i<hashSz; ++i)
    {
        printf("Index %d -> ", i);
        if (pHashArray[i].count > 0) {
            for (j = 0; j < pHashArray[i].count; ++j) {
                struct RecordType* record = pHashArray[i].records[j];
                printf("%d, %c, %d", record->id, record->name, record->order);
                if (j < pHashArray[i].count - 1) {
                    printf(" -> ");
                }
            }
        } else {
            printf("%d", i);
        }
        printf("\n");
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);
    
    // Initialize Hash Table
    int hashSz = 10;
    struct HashType *pHashArray = (struct HashType*)malloc(hashSz * sizeof(struct HashType));
    for (int i = 0; i < hashSz; ++i) {
        pHashArray[i].records = NULL;
        pHashArray[i].count = 0;
    }

    // Hashing
    for (int i = 0; i < recordSz; ++i) {
        int h = hash(pRecords[i].id);
        if (pHashArray[h].records == NULL) {
            pHashArray[h].records = (struct RecordType**)malloc(sizeof(struct RecordType*));
        } else {
            pHashArray[h].records = (struct RecordType**)realloc(pHashArray[h].records, (pHashArray[h].count + 1) * sizeof(struct RecordType*));
        }
        pHashArray[h].records[pHashArray[h].count] = &pRecords[i];
        pHashArray[h].count++;
    }

    // Display records in hash
    displayRecordsInHash(pHashArray, hashSz);

    // Free allocated memory
    for (int i = 0; i < hashSz; ++i) {
        free(pHashArray[i].records);
    }
    free(pHashArray);
    free(pRecords);
    
    return 0;
}
