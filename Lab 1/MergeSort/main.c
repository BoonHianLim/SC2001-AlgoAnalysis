#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int count = 0;

void InsertionSort(int *list, int start, int end)
{
    for(int i=start+1 ; i<=end ; i++)
    {
        for(int j=i ; j>start ; j--)
        {
            count++;
            if(list[j] < list[j-1])
                swap(list,j,j-1);
            else break;
        }
    }
}

void swap (int *list, int var1, int var2)
{
    int temp = list[var1];
    list[var1] = list[var2];
    list[var2] = temp;
}


void MergeSort(int *list, int start, int end)
{
    int mid;
    if(start < end)
    {
        mid = (start+end)/2;
        MergeSort(list,start,mid);
        MergeSort(list,mid+1,end);
        merge(list, start, mid, end);
    }
    return;
}

void MergeHybrid(int *list, int start, int end, int s)
{
    int mid;
    if(end-start<=s)
    {
        InsertionSort(list,start,end);
    }
    else if(start < end)
    {
        mid = (start+end)/2;
        MergeHybrid(list,start,mid,s);
        MergeHybrid(list,mid+1,end,s);
        merge(list, start, mid, end);
    }
    return;
}

void merge(int *list, int start, int mid, int end)
{
    int n1 = mid-start+1;
    int n2 = end - mid;

    int *L, *M;

    L = (int*)malloc(n1*sizeof(int));
    M = (int*)malloc(n2*sizeof(int));

    for (int i=0 ; i<n1 ; i++)
    {
        L[i] = list[start+i];
    }

    for (int j=0 ; j<n2 ; j++)
    {
        M[j] = list[mid+1+j];
    }

    int i, j, k;
    i = j = 0;
    k = start;

    while(i<n1 && j<n2)
    {
        count++;
        if(L[i] <= M[j])
        {
            list[k] = L[i];
            i++;
        }
        else
        {
            list[k] = M[j];
            j++;
        }
        k++;
    }

    while (i<n1)
    {
        list[k] = L[i];
        i++;
        k++;
    }

    while (j<n2)
    {
        list[k] = M[j];
        j++;
        k++;
    }

    free(L);
    free(M);
}

void Randomizer(int *list1, int *list2, int size)
{
    time_t t1;
    srand( (unsigned) time (&t1));
    for (int i=0 ; i<size ; i++)
    {
        list1[i] = list2[i] = rand()%size + 1;
    }
}

void printArrary(int *list, int size)
{
    for(int i=0 ; i < size ; i++)
    {
        printf("%d ",list[i]);
    }
}

void reset(int *list1, int *list2, int size)
{
    for(int i=0 ; i<size ; i++)
    {
        list1[i] = list2[i];
    }
}

int main()
{
    int size;
    int *list1, *list2;
    double time_spent = 0;



    printf("Please enter array size: ");
    scanf("%d",&size);
    list1 = (int*)malloc(size*sizeof(int));
    list2 = (int*)malloc(size*sizeof(int));
    Randomizer(list1,list2,size);
    printf("Mergesort\n");
    //printf("Before Sorting:\n");
    //printArrary(list1,size);
    clock_t start = clock();
    MergeSort(list1,0,size-1);
    //InsertionSort(list1,0,size-1);
    clock_t end = clock();
    time_spent += (double)(end - start) / CLOCKS_PER_SEC;
    //printf("\nAfter Sorting:\n");
    //printArrary(list1,size);
    printf("\nNumber of comparison for Mergesort: %d\n",count);
    printf("MergeSort took %f seconds to sort\n\n",time_spent);
    printf("MergeHybrid\n");

    for(int total_run = 0; total_run < 100; total_run++){
        Randomizer(list1,list2,size);
        FILE *fptr1,*fptr2;
        int length = snprintf( NULL, 0, "%d", total_run );
        char* str11 = malloc( length + 1 );
        snprintf( str11, length + 1, "%d", total_run );
        char str12[] = ".txt";

        //printf(str);
        char file1[] = "C:\\Users\\USER\\Documents\\Github\\Algo_Analysis_Performance\\Data\\count";
        strcat(file1,str11);
        strcat(file1,str12);
        //printf(file1);

        char* str21 = malloc( length + 1 );
        snprintf( str21, length + 1, "%d", total_run );
        char str22[] = ".txt";

        char file2[] = "C:\\Users\\USER\\Documents\\Github\\Algo_Analysis_Performance\\Data\\delay";
        strcat(file2,str21);
        strcat(file2,str22);
        //printf(file2);

        //printf(file1);
        fptr1 = fopen(file1,"w");
        fptr2 = fopen(file2,"w");
        for(int s=0 ; s <= 100 ; s++)
        {
            reset(list1, list2, size);
            count = 0;
            time_spent = 0;
            printf("S value: %d\n",s);
            //printf("Before Sorting:\n");
            //printArrary(list1,size);
            start = clock();
            MergeHybrid(list1,0,size-1,s);
            end = clock();
            time_spent += (double)(end - start) / CLOCKS_PER_SEC;
            //printf("\nAfter Sorting:\n");
            //printArrary(list1,size);
            //printf("\nNumber of comparison for MergeHybird: %d\n",count);
            fprintf(fptr1,"%d\n",count);
            //printf("MergeSort took %f seconds to sort\n\n",time_spent);
            fprintf(fptr2,"%f\n",time_spent);
        }
    fclose(fptr1);
    fclose(fptr2);
    }
}
