#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

// Merge function used in mergeSort
void merge(int pData[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));
    
    if (!L || !R)
    {
        printf("Memory allocation failed in merge function\n");
        exit(1);
    }
    
    // Copy data to temp arrays L and R
    for (i = 0; i < n1; i++)
    {
        L[i] = pData[l + i];
        extraMemoryAllocated += sizeof(int);
    }
    for (j = 0; j < n2; j++)
    {
        R[j] = pData[m + 1 + j];
        extraMemoryAllocated += sizeof(int);
    }
    
    // Merge the temp arrays back into pData[l..r]
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            pData[k] = L[i];
            i++;
        }
        else
        {
            pData[k] = R[j];
            j++;
        }
        k++;
    }
    
    // Copy the remaining elements of L and R, if any
    while (i < n1)
    {
        pData[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        pData[k] = R[j];
        j++;
        k++;
    }
    
    free(L);
    free(R);
    extraMemoryAllocated -= n1 * sizeof(int);
    extraMemoryAllocated -= n2 * sizeof(int);
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(pData, l, m);
        mergeSort(pData, m + 1, r);
        merge(pData, l, m, r);
    }
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = pData[i];
        j = i - 1;
        
        while (j >= 0 && pData[j] > key)
        {
            pData[j + 1] = pData[j];
            j--;
            extraMemoryAllocated += sizeof(int);
        }
        
        pData[j + 1] = key;
    }
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
    int i, j, temp;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (pData[j] > pData[j + 1])
            {
                temp = pData[j];
                pData[j] = pData[j + 1];
                pData[j + 1] = temp;
                extraMemoryAllocated += sizeof(int);
            }
        }
    }
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
    int i, j, min_idx, temp;
    for (i = 0; i < n - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < n; j++)
        {
            if (pData[j] < pData[min_idx])
            {
                min_idx = j;
            }
        }
        temp = pData[i];
        pData[i] = pData[min_idx];
        pData[min_idx] = temp;
        extraMemoryAllocated += sizeof(int);
    }
}

// parses input file to an integer array
int parseData(char* inputFileName, int** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    *ppData = NULL;

    if (inFile)
    {
        // Read the data size from the first line
        if (fscanf(inFile, "%d", &dataSz) == 1)
        {
            *ppData = (int*)malloc(sizeof(int) * dataSz);
            if (!(*ppData))
            {
                printf("Memory allocation failed in parseData function\n");
                exit(1);
            }
            extraMemoryAllocated += dataSz * sizeof(int);
        }

        // Read the integer data line by line
        for (int index = 0; index < dataSz; ++index)
        {
            if (fscanf(inFile, "%d", &(*ppData)[index]) != 1)
            {
                printf("Error reading data from the input file: %s\n", inputFileName);
                exit(1);
            }
        }

        fclose(inFile);
    }
    else
    {
        printf("Error opening input file: %s\n", inputFileName);
        exit(1);
    }

    return dataSz;
}


// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
    int i, sz = dataSz - 100;
    printf("\tData:\n\t");
    for (i = 0; i < 100; ++i)
    {
        printf("%d ", pData[i]);
    }
    printf("\n\t");
    
    for (i = sz; i < dataSz; ++i)
    {
        printf("%d ", pData[i]);
    }
    printf("\n\n");
}

int main(void)
{
    clock_t start, end;
    int i;
    double cpu_time_used;
    char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
    
    for (i = 0; i < 3; ++i)
    {
        int* pDataSrc, * pDataCopy;
        int dataSz = parseData(fileNames[i], &pDataSrc);
        
        if (dataSz <= 0)
            continue;
        
        pDataCopy = (int*)malloc(sizeof(int) * dataSz);
        
        printf("---------------------------\n");
        printf("Dataset Size : %d\n", dataSz);
        printf("---------------------------\n");
        
        printf("Selection Sort:\n");
        memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
        extraMemoryAllocated = 0;
        start = clock();
        selectionSort(pDataCopy, dataSz);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\truntime\t\t\t: %.9lf\n", cpu_time_used);
        printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
        printArray(pDataCopy, dataSz);
        
        printf("Insertion Sort:\n");
        memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
        extraMemoryAllocated = 0;
        start = clock();
        insertionSort(pDataCopy, dataSz);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\truntime\t\t\t: %.9lf\n", cpu_time_used);
        printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
        printArray(pDataCopy, dataSz);
        
        printf("Bubble Sort:\n");
        memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
        extraMemoryAllocated = 0;
        start = clock();
        bubbleSort(pDataCopy, dataSz);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\truntime\t\t\t: %.9lf\n", cpu_time_used);
        printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
        printArray(pDataCopy, dataSz);
        
        printf("Merge Sort:\n");
        memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
        extraMemoryAllocated = 0;
        start = clock();
        mergeSort(pDataCopy, 0, dataSz - 1);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\truntime\t\t\t: %.9lf\n", cpu_time_used);
        printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
        printArray(pDataCopy, dataSz);
        
        free(pDataCopy);
        free(pDataSrc);
    }

    return 0;
}
