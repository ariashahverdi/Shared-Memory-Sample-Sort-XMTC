#include <xmtc.h>
//#define PRINT
#define QS
//#define HS

psBaseReg psb0;

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

int main(void)
{
    int i;
    
    #ifdef PRINT
        for(i=0;i<N;i++){printf("%d ",A[i]);}
        printf("\n\n");
    #endif
    
    #ifdef QS
        QuickSort(A,0,N-1);
    #endif
    
    #ifdef HS
        HeapSort(A,N);
    #endif
    
    #ifdef PRINT
        for(i=0;i<N;i++){printf("%d ",A[i]);}
        printf("\n\n");
    #endif
    
    return 0;
}
