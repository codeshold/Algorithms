#include <stdio.h>
#include "swfcommon.h"

int partition(int a[], int low, int high){
    int pivot = a[low];

    while(high>low){
        while(high > low && a[high] >= pivot){ high--; }
        a[low] = a[high];
        while(high > low && a[low] <= pivot){ low++; }
        a[high] = a[low];
    }
    a[low] = pivot;

    return low;
}

void quicksort1(int a[], int low, int high){
     int pivot = 0;

     if(low < high){
        pivot = partition(a, low, high);
        quicksort1(a, low, pivot-1);
        quicksort1(a, pivot+1, high);
     }
}


void quicksort2(int str[], int low, int high){
    int a = low;
    int b = high;

    if(low < high) {
        int pivot = str[low];
        while(low < high) {
            while(low < high && str[high] >= pivot){ high--; }
            str[low] = str[high];
            while(low < high && str[low] <= pivot){ low++; }
            str[high] = str[low];
        }
        str[low] = pivot;

        quicksort2(str, low, pivot-1);
        quicksort2(str, pivot+1, high);
    }
}

int main(int argc, char *argv[]){
    const int number = 10;
    int *str = getArray(number);
    int str1[] = {1};
    int str2[] = {1, 1};
    int str21[] = {2, 1};

    printArray(str1, 1);
    quicksort1(str1, 0, 0);
    printArray(str1, 1);
    quicksort2(str1, 0, 0);
    printArray(str1, 1);


    printArray(str2, 2);
    quicksort1(str2, 0, 1);
    printArray(str2, 2);
    quicksort2(str2, 0, 1);
    printArray(str2, 2);

    printArray(str21, 2);
    quicksort1(str21, 0, 1);
    printArray(str21, 2);
    quicksort2(str21, 0, 1);
    printArray(str21, 2);

    printArray(str, number);
    quicksort1(str, 0, number-1);
    printArray(str, number);
    quicksort1(str, 0, number-1);
    printArray(str, number);

    return 0;
}
