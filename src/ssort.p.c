#include <xmtc.h>

psBaseReg psb0;
//#define PRINT //If you want to see every intermediate value uncomment this line
//#define PRINT_FINAL //If you want to check the final result is correct uncomment this line

/////////////////////  Quick Sort /////////////////////

int Partition(int A[], int p, int r){
    int tmp;
    int x = A[r];
    int i = p-1;
    int j;
    for (j=p ; j<r ; j++) {
        if (A[j] <= x){
            i++;
            tmp = A[i];
            A[i] = A[j];
            A[j] = tmp;
        }
    }
    i++;
    tmp = A[i];
    A[i] = A[r];
    A[r] = tmp;
    return i;
}

/* Recursive Quick Sort */
void QuickSort(int A[] , int p, int r){
    if (p < r) {
        int q;
        q = Partition(A, p, r);
        QuickSort(A, p, q-1);
        QuickSort(A, q+1, r);
    }
}


//////////////////////  Heap Sort /////////////////////

// Source: https://rosettacode.org/wiki/Sorting_algorithms/Heapsort#C
int max (int *a, int n, int i, int j, int k) {
    int m = i;
    if (j < n && a[j] > a[m]) {
        m = j;
    }
    if (k < n && a[k] > a[m]) {
        m = k;
    }
    return m;
}

void downheap (int *a, int n, int i) {
    while (1) {
        int j = max(a, n, i, 2 * i + 1, 2 * i + 2);
        if (j == i) {
            break;
        }
        int t = a[i];
        a[i] = a[j];
        a[j] = t;
        i = j;
    }
}

void HeapSort (int *a, int n) {
    int i;
    for (i = (n - 2) / 2; i >= 0; i--) {
        downheap(a, n, i);
    }
    for (i = 0; i < n; i++) {
        int t = a[n - i - 1];
        a[n - i - 1] = a[0];
        a[0] = t;
        downheap(a, n - i - 1, 0);
    }
}


int C[NTCU][NTCU] = {0};
int PS[NTCU+1][NTCU] = {0};
int global_ps[NTCU+1];
int size_arr[NTCU];
int IDX[N] = {0};
int PART[N], SERIAL[N], Result[N], sum[NTCU];

int main(void)
{
    int p = NTCU;
    int S_SIZE = s * p;
    int S[S_SIZE];
    int S_P[p];
    int i,j;

    #ifdef PRINT
        printf("N: %d\n", N);
        printf("p: %d\n", p);
    #endif
    
    spawn(0,S_SIZE-1){
        S[$] = A[RANDOM[($*2)%NRAND]%N];//I am using different random numbers to get a little bit more speedup
    }
    QuickSort(S,0,S_SIZE-1);


    for (i=0; i<p-1; i++) {S_P[i] = S[(i+1)*s]; }

    #ifdef PRINT
        printf("S_Prime is : \n");
        for (i=0; i<p-1; i++) {printf("%d ", S_P[i]);}
        printf("\n\n\n\n\n");
    
        printf("A is : \n");
        for (i=0; i<N; i++) {
            printf("%d ", A[i]);
        }
        printf("\n\n\n\n\n");
    #endif
    
    int low = 0;
    int high = p-1;
    /////////////////////   Step 2   /////////////////////
    spawn(0,high){
        int idx_start = $ * (N/p);
        //int idx = 0;
        int a_idx = 0;
        //int res_idx = 0;
        
        for (int idx=0; idx< N/p; idx++) {
            int l; int r; int m;
            int  a_idx =  idx_start + idx;
            
            // Binary Search for the Range
            l = 0;
            r = p - 2;

            while (l <= r) {
                // invariants: value > A[i] for all i < low value <= A[i] for all i > high
                m = (l + r) / 2;
                if (S_P[m] <= A[a_idx]) l = m + 1;
                    else r = m - 1;
            }
            PART[a_idx] = l;
            SERIAL[a_idx] = C[$][l];
            C[$][l]++;
        }
        
    }//join
     
    #ifdef PRINT
        printf("After Step 2\n");
        for(i=0;i<N;i++){
            printf ("A[%d]:\t %d \t idx: %d \t Ser: %d \n",i,A[i], PART[i], SERIAL[i]);
        }
        printf("\n\n\n");
    
        printf("\nC is : \n");
        for (j=0; j<NTCU; j++) {
            for (i=0; i<NTCU; i++) {
                printf("%d ", C[j][i]);
            }
            printf("\n");
        }
    #endif
    
    /////////////////////   Step 3   /////////////////////
    //Actually it can be mixed with the previous spawn
    low = 0;
    high = p-1;
    int local_thread[p];
    for (i=0; i<p; i++){ local_thread[i] = 0; }
    int idx_row;
    for (idx_row=0; idx_row< p; idx_row++) {
        spawn(low,high){
            int sum_t = 0;
            sum_t = C[idx_row][$];
            psm(sum_t, local_thread[$]);
            PS[idx_row+1][$] = local_thread[$];
            sum[$] = local_thread[$];
        }//join
    }


    int idx_col;
    i = 0;
    int temp = 0;
    global_ps[0] = 0;
    for (idx_col = 1; idx_col<=p; idx_col++) {
        global_ps[idx_col] = global_ps[idx_col-1] + sum[idx_col-1];
    }

    #ifdef PRINT
        printf("\nPS is : \n");
        for (i=0; i<p+1; i++) {
            for (j=0; j<p; j++) {
                printf("%d ", PS[i][j]);
            }
            printf("\n");
        }
        printf("\n\nsum is: ");
        for(i=0;i<p;i++){
            printf ("%d ",sum[i]);
        }
        printf("\n\nglobal_sum is: ");
        for(i=0;i<p+1;i++){
            printf ("%d ",global_ps[i]);
        }
        printf("\n\nsize_arr is: ");
        for(i=0;i<p+1;i++){
            printf ("%d ",size_arr[i]);
        }
        printf("\n\n\n");
    #endif

    /////////////////////   Step 4   /////////////////////
    int pos_s, part_s, ps_s, serial_s, idx_s;
    low = 0;
    high = p-1;

    spawn(low,high){
        int idx_start = $ * (N/p);
        for (int idx=0; idx< N/p; idx++) {
            int part = PART[idx_start + idx];
            int pos = global_ps[part] + PS[$][part] + SERIAL[idx_start + idx];
            Result[pos] = A[idx_start + idx];
        }
    
    }//join

    #ifdef PRINT
        printf("Result Before sort is: ");
        for(i=0;i<N;i++){
            printf ("%d ",Result[i]);
        }
        printf("\n\n\n");
    #endif
    
    
    /////////////////////   Step 5   /////////////////////
    spawn(low,high){
        // The final partition will be sorted by quick sort
        QuickSort(Result,global_ps[$],global_ps[$+1]-1);
        
        // The final partition will be sorted by Heap sort
        //int *ptr = &Result[global_ps[$]];
        //HeapSort(ptr,global_ps[$+1] - global_ps[$]);
    }//join
    
    #ifdef PRINT_FINAL
        QuickSort(A,0,N-1);
    #endif
    
    #ifdef PRINT_FINAL
        int flag_check = 0;
        for (i=0; i<N; i++) {
            if(A[i] != Result[i]){
                flag_check = 1;
                printf("Fail at index %d \n", i);
            }
        }
        if(!flag_check) printf("Success\n");
    #endif

    #ifdef PRINT
    
        printf("Result of A is: ");
        for(i=0;i<N;i++){
            printf ("%d ",A[i]);
        }
        printf("\n\n\n");
        printf("Result is: ");
        for(i=0;i<N;i++){
            printf ("%d ",Result[i]);
        }
        printf("\n\n\n");
    #endif
    
    return 0;
}