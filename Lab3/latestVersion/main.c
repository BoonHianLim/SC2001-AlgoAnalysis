#include <stdio.h>
#include <stdlib.h>


int knapsack(int *w, int *p, int n, int C,int *M, int *item){

    int i;
    int j;
    int profit;
    printf("currentProfit: ");
    for(i=1;i<=C;i++){
        //
        profit = 0;
        //start look through every object
        for(j=0;j<n;j++){

            int remainingWeight = i - w[j];

            if(remainingWeight >= 0){
                if((p[j] + M[remainingWeight]) > profit){
                    profit = p[j] + M[remainingWeight];
                    item[i] = j;
                }
            }
        }

        //end of loop, save max profit to array M
        M[i] = profit;
    }


    /*for(i=0;i<=C;i++){
        for(j=0;j<=n;j++){
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }*/
    printf("\n");
    return M[C];

}
int main()
{
    int *M;
    int *item;
    int i;
    int n; //number of items
    int C; //Capacity
    int *w; //weight array
    int *p; //profit array

    printf("Enter the number of items n: \n");
    scanf("%d",&n);

    printf("Enter the total capacity C: \n");
    scanf("%d",&C);

    //Initialise the 2D array for the memory space


    M = (int *)malloc(sizeof(int)*(C+1));
    item = (int *)malloc(sizeof(int)*(C+1));
    //1D array for the weight
    w = (int *)malloc(sizeof(int)*(n+1));
    //1D array for the profit
    p = (int *)malloc(sizeof(int)*(n+1));

    printf("Enter the weight of items (as a list, separate by spacing): \n");
    for(i=0;i<n;i++){
        scanf("%d",&w[i]);
    }


    printf("Enter the profit of items (as a list, separate by spacing): \n");
    for(i=0;i<n;i++){
        scanf("%d",&p[i]);
    }

    //Initialisation of the array
    for(i=0;i<=C;i++){
        M[i] = 0;
        item[i] = -1;
        printf("%d ", M[i]);
    }

    printf("\n");

    int profit = knapsack(w,p,n,C,M,item);
    printf("Total profit: %d\n",profit);

    printf("\n");
    printf("The content of the array M:");
    for(i=0;i<=C;i++){
        printf("%d ",M[i]);

    }

    printf("\n");
    printf("The content of the item M:");
    int j = C;
    while(j >= 0){
        printf("%d ",item[j]);
        j -= w[item[j]];
    }
    return 0;
}
