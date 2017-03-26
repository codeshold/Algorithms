#include <stdio.h>

#define SWAP(a, b) do {int tmp = a; a = b; b = tmp;} while(0)
#define MAX_NUM 100

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}


/*************************************************
 *              Simple Insertion Sort
 *************************************************/
void SimpleInsertionSort(int a[], int len)
{
    int i, j, tmp;

    for(i=1; i<len; i++){
        tmp = a[i];
        for(j=i-1; j>=0 && a[j]>tmp; j--){
           a[j+1] = a[j]; //move backward
        }
        a[j+1] = tmp;
    }
}


/*************************************************
 *              Binary Insertion Sort
 *************************************************/
void BinaryInsertionSort(int a[], int len)
{
    int i, j, tmp;
    int low, high, mid;

    for(i=1; i<len; i++){
        tmp = a[i];
        low = 0;
        high = i-1;
        while(low <= high){ // find the inserted location by binary serach
            mid = (low+high)/2;
            if(a[mid] > tmp) {
                high = mid - 1;
            }
            else {
                low = mid + 1;
            }
        }
        for(j=i-1; j>=high+1; j--) { // move backward
            a[j+1] = a[j];
        }
        a[high+1] = tmp;
    }
}



/*************************************************
 *              Shell Sort
 *************************************************/
void SSort(int a[], int len, int delt)
{
    int i, j, tmp;
    for(i = delt; i < len; i += delt){
        tmp = a[i];
        for(j = i-delt; j >= 0 && tmp < a[j]; j -= delt){
            a[j+delt] = a[j];//move backward
        }
        a[j+delt] = tmp;
    }

}

void ShellSort(int a[], int len)
{
    int delt;

    for(delt = len/2; delt > 0; delt/=2){ // test
        SSort(a, len, delt);
    }
}




/*************************************************
 *              Bubble Sort
 *************************************************/
void BubbleSort(int a[], int len) { int flag = 0; int i, j;
    for(i=0; i<len-1; i++){
        for(j=0; j<len-1-i; j++){
            if(a[j] > a[j+1]) {
                SWAP(a[j], a[j+1]);
                flag = 1;
            }
        }
        if(flag == 0) break; //for sorted sequence
    }
}


/*************************************************
 *              Simple Selection Sort
 *************************************************/
void SimpleSelectionSort(int a[], int len)
{
    int min, k, i, j;

    for(i=0; i<len-1; i++){
        min = a[i];
        k = i;
        for(j=i+1; j<len; j++){
            if(min > a[j]){
                min = a[j];
                k = j; // save the minimum index
            }
        }
        SWAP(a[i], a[k]);
    }
}


/*************************************************
 *              Merge Sort
 *************************************************/
int g_tmp[MAX_NUM] = {0,};

void Merge(int *a, int low, int mid, int high)
{
    int i, j, k;

    for(i=low; i<=high; i++) g_tmp[i] = a[i];
    for(i=low, j=mid+1, k=low; i<=mid && j<=high; k++) {
        if(g_tmp[i] <= g_tmp[j]) {
            a[k] = g_tmp[i++];
        }
        else {
            a[k] = g_tmp[j++];
        }
    }
    while(i <= mid) a[k++] = g_tmp[i++];
    while(j <= high) a[k++] = g_tmp[j++];
}

void MSort(int a[], int low, int high)
{
    int mid;
    if(low < high) {
        mid = (low+high)/2;
        MSort(a, low, mid);
        MSort(a, mid+1, high);
        Merge(a, low, mid, high);
    }
}
void MergeSort(int a[], int len)
{
    MSort(a, 0, len-1);
}


/*************************************************
 *              Heap Sort
 *************************************************/
void AdjustDown(int a[], int i, int len)
{
    int tmp = a[i];
    int k; //child node

    for(k=2*i+1; k<len; k=2*k+1) {
        if(k+1 < len && a[k+1] > a[k]) k++; //find the largest child
        if(tmp >= a[k]) { // break
            break;
        }
        else {
            a[i] = a[k]; //move the child node to the parent index
            i = k;
        }
    }
    a[i] = tmp;
}

void BuildMaxHeap(int a[], int len)
{
    int i;
    for(i=len/2 - 1; i>=0; i--){
        AdjustDown(a, i, len);
    }
}
void HeapSort(int a[], int len)
{

    BuildMaxHeap(a, len);

    while(len>1){
        SWAP(a[0], a[len-1]);
        AdjustDown(a, 0, --len);
    }
}


/*************************************************
 *              Quick Sort
 *************************************************/
int Partition(int a[], int left, int right)
{
    //swap(&a[left], &a[(left+right)/2]);
    //SWAP(a[left], a[(left+right)/2]);

    int pivot = a[left];

    while(left < right) {
        while(left < right && a[right] >= pivot) { right--; }
        a[left] = a[right];
        while(left < right && a[left] <= pivot) { left++; }
        a[right] = a[left];
    }
    a[left] = pivot;

    return left;
}


void QSort(int a[], int left, int right)
{
    int pivotpos;

    if(left < right){
        pivotpos = Partition(a, left, right);
        QSort(a, left, pivotpos - 1);
        QSort(a, pivotpos + 1, right);
    }
}


void QuickSort(int a[], int n)
{
    QSort(a, 0, n-1);
}



int main(int argc, char *argv[])
{
    int array[] = {11, 2, 3, 4, 3, 5, 6, 10, 97, 0,
                    12, 23, 45, 34, 54, 45, 2, 3, 56,
                    2, 4, 5, 6, 7, 44, 3, 6,3, 5, 6,};
    int i;
    //int array[] = {87, 45, 78, 32, 17, 65, 53, 9};
    int n = sizeof(array)/sizeof(array[0]);

    //SimpleInsertionSort(array, n);
    //BinaryInsertionSort(array, n);
    //ShellSort(array, n);
    //BubbleSort(array, n);
    //SimpleSelectionSort(array, n);
    //MergeSort(array, n);
    //QuickSort(array, n);
    HeapSort(array, n);

    for(i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}

