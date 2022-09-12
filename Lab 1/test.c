#include <stdio.h>
#include <time.h>
int count = 0;

int min(int x, int y) { return (x<y)? x :y; }

void Randomizer(int *list1, int *list2, int size)
{
    time_t t1;
    srand( (unsigned) time (&t1));
    for (int i=0 ; i<size ; i++)
    {
        list1[i] = list2[i] = rand()%100 + 1;
    }
}

void MergeSort(int *list, int start, int end)
{
    int mid = (start+end)/2;

    if(end-start <= 0) //Replace 2 with S
    {
        return;
    }
    else if(end-start > 1)
    {
        MergeSort(list,start,mid);
        MergeSort(list,mid+1,end);
    }
    merge(list,start,end);
}

void mergeSortIterative(int arr[], int n){

   int curr_size;  // For current size of subarrays to be merged
                   // curr_size varies from 1 to n/2
   int left_start; // For picking starting index of left subarray
                   // to be merged
  
   // Merge subarrays in bottom up manner.  First merge subarrays of
   // size 1 to create sorted subarrays of size 2, then merge subarrays
   // of size 2 to create sorted subarrays of size 4, and so on.
   for (curr_size=1; curr_size<=n-1; curr_size = 2*curr_size)
   {
       // Pick starting point of different subarrays of current size
       for (left_start=0; left_start<n-1; left_start += 2*curr_size)
       {
           // Find ending point of left subarray. mid+1 is starting 
           // point of right
           int mid = min(left_start + curr_size - 1, n-1);
  
           int right_end = min(left_start + 2*curr_size - 1, n-1);
  
           // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end]
           mergeIterative(arr, left_start,mid, right_end);
       }
   }

}
void mergeIterative(int *list, int start, int mid,int end)
{
    int a = start, b = mid+1, i, tmp;
    if(end-start <=0)
        return;
    while(a <= mid && b <= end)
    {
        int cmp = list[a] - list[b];
        count++;

        if(cmp>0)
        {
            tmp = list[b++];
            for(int i= ++mid ; i>a ; i--)
            {
                list[i] = list[i-1];
            }
            list[a++] = tmp;
        }
        else if(cmp<0)
        {
            a++;
        }
        else
        {
            if(a==mid && b==end)
                break;
            tmp = list[b++];
            a++;
            for(i=++mid ; i>a ; i--)
            {
                list[i] = list[i-1];
            }
            list[a++] = tmp;
        }
    }
}
void merge(int *list, int start, int end)
{
    int mid = (end+start)/2;
    int a = start, b = mid+1, i, tmp;
    if(end-start <=0)
        return;
    while(a <= mid && b <= end)
    {
        int cmp = list[a] - list[b];
        count++;

        if(cmp>0)
        {
            tmp = list[b++];
            for(int i= ++mid ; i>a ; i--)
            {
                list[i] = list[i-1];
            }
            list[a++] = tmp;
        }
        else if(cmp<0)
        {
            a++;
        }
        else
        {
            if(a==mid && b==end)
                break;
            tmp = list[b++];
            a++;
            for(i=++mid ; i>a ; i--)
            {
                list[i] = list[i-1];
            }
            list[a++] = tmp;
        }
    }
}

int main()
{
    int size;
    int *list1, *list2;
    printf("Enter your array size: ");
    
    scanf("%d",&size);
    
    list1 = (int*)malloc(size * sizeof(int));
    list2 = (int*)malloc(size * sizeof(int));
    Randomizer(list1,list2,size);

    //printf("\n");
    clock_t start = clock();
    mergeSortIterative(list1,size);
    clock_t end = clock();
    //double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    //printf("\nNumber of comparison for Mergesort: %d\n",count);
    //printf("MergeSort took %f miliseconds to sort\n\n",time_spent);
}
