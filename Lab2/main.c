#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <inttypes.h>
enum GraphType {ADJ_MATRIX, ADJ_LIST};

typedef struct _listnode
{
    int vertex;
    int priority;
    struct _listnode *next;
}ListNode;

union GraphForm
{
    int **matrix;
    ListNode **list;
};

typedef struct _graph
{
    int V;
    int E;
    enum GraphType type;
    union GraphForm adj;
} Graph;

typedef struct _queueNode
{
    int vertex;
    int priority;
} QueueNode;

//Size of the priority queue
QueueNode q[1000000];

//Pointer to the last index
int size = 0;

void printGraphMatrix(Graph g)
{
    int i,j;
    printf("Print graph matrix\n");

    for(i=0; i<g.V; i++)
    {
        for(j=0; j<g.V; j++)
        {
            printf("%d\t",g.adj.matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void adjM2adjL(Graph *g)
{
	g->type = ADJ_LIST; //change representation form

    ListNode **list;
    list = (ListNode **) malloc(sizeof(ListNode)*(g->V));

    for(int i=0;i<g->V;i++)
    {
        list[i] = NULL;
    }

    for (int i=0 ; i<g->V ; i++)
    {
        for (int j=0 ; j<g->V ; j++)
        {
            if(g->adj.matrix[i][j])
            {
                ListNode *temp = malloc(sizeof(ListNode));

                if(list[i] == NULL)
                {

                    temp->vertex = j+1;
                    temp->priority = g->adj.matrix[i][j];
                    temp->next = NULL;
                    list[i] = temp;
                }
                else
                {
                    temp->vertex = j+1;
                    temp->priority = g->adj.matrix[i][j];
                    temp->next = list[i];
                    list[i] = temp;
                }
            }
        }
    }
    g->adj.list = list;
}

void printGraphList(Graph g)
{
    int i;
    ListNode* temp;
    printf("Print graph list \n");
    if(g.type == ADJ_MATRIX) {printf("Error"); return;}

    for(i=0;i<g.V;i++)
    {
        printf("%d:\t",i+1);
        temp = g.adj.list[i];
        while(temp!=NULL){
            printf("%d -> ",temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
    printf("\n");
}

void enqueue_1(int vertex, int priority)
{
    q[size].vertex = vertex;
    q[size].priority = priority;
    size++;
}

int peek()
{
    int i;
    int highestPriority = INT_MAX;
    int indexPos = -1;

    for(i = 0; i<size ; i++)
    {
        if(q[i].priority < highestPriority)
        {
            highestPriority = q[i].priority;
            indexPos = i;
        }
        else if(highestPriority == q[i].priority && q[i].vertex < q[indexPos].vertex)
        {
            highestPriority = q[i].priority;
            indexPos = i;
        }
    }
    return indexPos;
}

void dequeue_1(int index)
{
    int i;

    for (i = index ; i<size ; i++)
    {
        q[i] = q[i+1];
    }

    size--;
}

void printArrays(int *d, int *pi, int *S, int size)
{
    int i;

    for(i = 0 ; i<size ; i++)
        printf("Node %d , S: %d , d: %d , pi: %d\n",i+1,S[i],d[i],pi[i]);
}

void printPriorityQueue()
{
    printf("Priority Queue:\n");
    for(int i=0 ; i<size ; i++)
        printf("Vertex: %d | Weight: %d\n",q[i].vertex,q[i].priority);
}

void Dijkstra_1(Graph g, int source)
{
    int i;

    //array of estimates for the length of shortest path from source node to all node
    int *d_1 = (int *)malloc(g.V*sizeof(int));
    //array to store the predecessors for each vertex
    int *pi_1 = (int *)malloc(g.V*sizeof(int));
    //array to store whether the node has been visited
    int *S_1 = (int *)malloc(g.V*sizeof(int));

    for(i = 0 ; i<g.V ; i++)
    {
        //set d[i] to infinity
        d_1[i] = INT_MAX;
        //set pi[i] to have no predecessor
        pi_1[i] = NULL;
        //set S[i] to not visited
        S_1[i] = 0;
    }

    d_1[source-1] = 0;
    S_1[source-1] = 1;

    //printArrays(d_1,pi_1,S_1,g.V);

    for(i=0 ; i<g.V ; i++)
    {
        if(g.adj.matrix[source-1][i] > 0)
        {
            d_1[i] = g.adj.matrix[source-1][i];
            pi_1[i] = source;
            enqueue_1(i+1,d_1[i]);
        }
    }

    //printPriorityQueue();

    while(size>0)
    {
        int u = peek();
        int vertex = q[u].vertex;
        dequeue_1(u);
        //printPriorityQueue();
        S_1[vertex-1] = 1;
        //printf("Vertex: %d\n",vertex);

        for(i=0 ; i<g.V ; i++)
        {
            if(g.adj.matrix[vertex-1][i] > 0)
            {
                if(S_1[i] == 0 && d_1[i] > d_1[vertex-1] + g.adj.matrix[vertex-1][i])
                {
                    //dequeue();
                    d_1[i] = d_1[vertex-1] + g.adj.matrix[vertex-1][i];
                    pi_1[i] = vertex;
                    enqueue_1(i+1,d_1[i]);
                    //printPriorityQueue();
                }
            }
            //printArrays(d,pi,S,g.V);
        }
        //printArrays(d,pi,S,g.V);
    }
    //printArrays(d_1,pi_1,S_1,g.V);
    free(d_1);
    free(pi_1);
    free(S_1);
}

void swap(QueueNode q[], int a, int b)
{
    int temp_vertex = q[a].vertex;
    int temp_prioroty = q[a].priority;
    q[a].vertex = q[b].vertex;
    q[a].priority = q[b].priority;
    q[b].vertex = temp_vertex;
    q[b].priority = temp_prioroty;
}

void heapify(QueueNode q[], int N, int i)
{
    int smallest = i;

    // left = 2*i + 1
    int left = 2 * i + 1;

    // right = 2*i + 2
    int right = 2 * i + 2;

    // If left child is larger than root
    if (left < N && q[left].priority > q[smallest].priority)
        smallest = left;

    // If right child is larger than largest
    // so far
    if (right < N && q[right].priority > q[smallest].priority)
        smallest = right;

    // Swap and continue heapifying if root is not largest
    // If largest is not root
    if (smallest != i)
    {
        swap(q,i, smallest);

        // Recursively heapify the affected
        // sub-tree
        heapify(q, N, smallest);
    }
}

void heapSort(QueueNode q[], int N)
{
    // Build max heap
    for (int i = N / 2 - 1; i >= 0; i--)
        heapify(q, N, i);

    // Heap sort
    for (int i = N - 1; i >= 0; i--)
    {
        swap(q,0,i);
        // Heapify root element to get highest element at
        // root again
        heapify(q, i, 0);
    }
}

void enqueue_2(int vertex, int priority)
{
    q[size].vertex = vertex;
    q[size].priority = priority;
    size++;
}

void dequeue_2()
{
    for (int i = 0 ; i<size ; i++)
        q[i] = q[i+1];

    size--;
}

void Dijkstra_2(Graph g, int source)
{
    int i;
    size = 0;
    //printf("Dijkstra\n");
    //array of estimates for the length of shortest path from source node to all node
    int *d = (int *)malloc(g.V*sizeof(int));
    //array to store the predecessors for each vertex
    int *pi = (int *)malloc(g.V*sizeof(int));
    //array to store whether the node has been visited
    int *S = (int *)malloc(g.V*sizeof(int));

    for(i = 0 ; i<g.V ; i++)
    {
        //set d[i] to infinity
        d[i] = INT_MAX;
        //set pi[i] to have no predecessor
        pi[i] = NULL;
        //set S[i] to not visited
        S[i] = 0;
    }

    d[source-1] = 0;
    S[source-1] = 1;

    //printArrays(d,pi,S,g.V);

    ListNode *temp;
    temp = g.adj.list[source-1];

    while(temp!=NULL)
    {
        d[temp->vertex-1] = temp->priority;
        pi[temp->vertex-1] = source;
        enqueue_2(temp->vertex,d[temp->vertex-1]);
        temp = temp->next;
        //printArrays(d,pi,S,g.V);
    }
    heapSort(q,size);

    while(size>0)
    {
        //printf("Size: %d\n",size);
        int vertex = q[0].vertex;
        //printPriorityQueue();
        dequeue_2();
        S[vertex-1] = 1;
        //printf("Vertex: %d\n",vertex);
        //printArrays(d,pi,S,g.V);

        temp = g.adj.list[vertex-1];

        while(temp!= NULL)
        {
            i = temp->vertex;
            if(S[i-1] == 0 && d[i-1] > d[vertex-1] + temp->priority)
            {
                //dequeue();
                d[i-1] = d[vertex-1] + temp->priority;
                pi[i-1] = vertex;
                enqueue_2(i,d[i-1]);
                //printf("Before Sorting:\n");
                //printPriorityQueue();
                heapSort(q,size);
                //printf("After Sorting:\n");
                //printPriorityQueue();
            }
            temp = temp->next;
        }
    }
    //printArrays(d,pi,S,g.V);
    free(d);
    free(pi);
    free(S);
}

void pigeonRand(Graph g){
    time_t t1;
    srand( (unsigned) time (&t1));
    int max_e = (g.V-2)*(g.V-1)+1;
    int V1, V2;
    int weight;
    for(int i=0 ; i<max_e ; i++)
    {
        V1 = rand()%g.V;
        V2 = rand()%g.V;

        while(g.adj.matrix[V1][V2] != 0 || V1 == V2)
        {
            V1 = rand()%g.V;
            V2 = rand()%g.V;
        }

        g.adj.matrix[V1][V2] = rand()%max_e+1;
    }
}



void totalRand(Graph g, int numEdges){
    time_t t1;
    srand( (unsigned) time (&t1));
    uint64_t V1, V2;
    int weight;
    uint64_t num;

    for(int i=0 ; i<numEdges ; i++)
    {
        V1 = rand();
        V1 = (V1 << 32) | rand();
        V1 = (V1 % (g.V - 0)) + 0;

        V2 = rand();
        V2 = (V2 << 32) | rand();
        V2 = (V2 % (g.V - 0)) + 0;


        while(g.adj.matrix[V1][V2] != 0){
            int addWhich = rand()%2;

            if(addWhich){
                if(V1 < g.V-1){
                    V1++;
                }else{
                    V1 = 0;
                }
            }else{
                if(V2 < g.V-1){
                    V2++;
                }else{
                    V2 = 0;
                }

            }
        }

        g.adj.matrix[V1][V2] = rand()%g.V+1;
    }
}

void connectedRand(Graph g, int numEdges){
    time_t t1;
    srand( (unsigned) time (&t1));
    uint64_t num;

    int* connected = (int *)malloc(g.V*sizeof(int));
    int* connectedV = (int *)malloc(g.V*sizeof(int));

    connectedV[0] = 0;
    connected[0] = 1;
    int connectedSize = 1;

    while(connectedSize != g.V){

        srand( (unsigned) time (&t1));
        int unconnectedVertex = rand();
        unconnectedVertex = (unconnectedVertex << 32) | rand();
        unconnectedVertex = (unconnectedVertex % (g.V - 1)) + 1;

        while(connected[unconnectedVertex] >= 1){
            if(unconnectedVertex < g.V-1){
                unconnectedVertex++;
            }else{
                unconnectedVertex = 0;
            }
        }


        srand( (unsigned) time (&t1));
        int connectedIndex = rand();
        connectedIndex = (connectedIndex << 32) | rand();
        connectedIndex = (connectedIndex % (connectedSize - 0)) + 0;


        g.adj.matrix[connectedV[connectedIndex]][unconnectedVertex] = rand()%g.V+1;
        connected[unconnectedVertex] = 1;
        connectedV[connectedSize] = unconnectedVertex;
        connectedSize++;
    }

    int left = numEdges - connectedSize + 1;
    if(left > 0){
        totalRand(g, left);
    }
    free(connectedV);
    free(connected);
}

void weightRand(Graph g){
    int MAX = g.V;
    time_t t1;
    srand( (unsigned) time (&t1));
    uint64_t num;


    for(int i=0 ; i<g.V ; i++)
    {
        for(int j=0 ; j<g.V ; j++){
            num = rand();
            num = (num << 32) | rand();
            num = (num % (MAX - 1)) + 1;
            g.adj.matrix[i][j] = num;
        }
    }
}

int main()
{
    struct timespec tstart={0,0}, tend={0,0};

    Graph g;
    int i,j, source, numEdges;
    int *degreeV;
    double time_spent;
    clock_t start, end;

    printf("Enter the number of vertices: ");
    scanf("%d",&g.V);
    printf("Enter the max number of vertices: ");
    scanf("%d",&numEdges);






    /*int no_e = rand()%max_e + 1;
    printf("no_e: %d\n",no_e);*/
    
    for(int total_run = 8; total_run < 10; total_run++){
        int length = snprintf( NULL, 0, "%d", total_run );
        
        char* str11 = malloc( length + 1 );
        snprintf( str11, length + 1, "%d", total_run );
        char str12[] = ".csv";

        
        char name1[50] = "edges5000A\\Algo1_";
        strcat(name1,str11);
        strcat(name1,str12);

        char* str21 = malloc( length + 1 );
        snprintf( str21, length + 1, "%d", total_run );
        char str22[] = ".csv";
        
        char name2[50] = "edges5000A\\Algo2_";
        strcat(name2,str21);
        strcat(name2,str22);

        FILE *fpt1,*fpt2;
        fpt1 = fopen(name1, "w+");
        fpt2 = fopen(name2, "w+");
        for(int curVertex = 4200; curVertex <= 10000; curVertex += 10000/50){
            g.V = curVertex;
            for(int edge = 5000;edge<=5000;edge+=5000/5){

                printf("%d:\n",curVertex);
                g.E = 0;
                g.adj.matrix = (int **)malloc(g.V*sizeof(int *));
                for(i=0 ; i<g.V ; i++)
                    g.adj.matrix[i] = (int *)malloc(g.V*sizeof(int));
                for(i=0 ; i<g.V ; i++)
                    for(j=0 ; j<g.V ; j++)
                        g.adj.matrix[i][j] = 0;
                g.type = ADJ_MATRIX;
                //printGraphMatrix(g);

                degreeV = (int *)malloc(g.V*sizeof(int));
                for(i=0 ; i<g.V ; i++)
                    degreeV[i] = 0;



                //printf("Partial Connected\n");
                totalRand(g,edge);

                        //printGraphMatrix(g);
                //printf("Dijkstra 1:\n");

                time_t t1;
                srand( (unsigned) time (&t1));
                uint64_t vertex;
                vertex = rand();
                vertex = (vertex << 32) | rand();
                vertex = (vertex % (g.V - 0)) + 0;

                clock_gettime(CLOCK_MONOTONIC, &tstart);
                Dijkstra_1(g,vertex);
                clock_gettime(CLOCK_MONOTONIC, &tend);
                printf("Dijkstra_1 took about %.5f seconds\n",
                ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
                ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

                //printf("%f\n",time_spent);
                fprintf(fpt1,"%d %f\n",curVertex,((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
                ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));


                //printf("\n");
                adjM2adjL(&g);
                //printGraphList(g);
                //printf("Dijkstra 2:\n");

                clock_gettime(CLOCK_MONOTONIC, &tstart);
                Dijkstra_2(g,vertex);
                clock_gettime(CLOCK_MONOTONIC, &tend);
                printf("Dijkstra_2 took about %.5f seconds\n",
                ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
                ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

                //printf("%lf\n",time_spent);
                fprintf(fpt2,"%d %f\n",curVertex,((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
                ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

                for(i=0 ; i<g.V ; i++)
                    free(g.adj.matrix[i]);
                free(g.adj.matrix);
                free(degreeV);
            }

        }
        fclose(fpt1);
        fclose(fpt2);
        //printGraphMatrix(g);

        /*printf("Enter the source node: ");
        scanf("%d",&source);*/
    }
}

