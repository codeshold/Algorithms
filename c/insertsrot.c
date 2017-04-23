#include <stdio.h>
#include "swfcommon.h"

// str[0]作为哨作为哨兵兵
void insertsort1(int str[], int n){
    int i, j;

    for(i=2; i<=n-1; i++){
        if(str[i] < str[i-1]){
            str[0] = str[i]; //哨兵
            for(j=i-1; str[j]>str[0]; j--){
                str[j+1] = str[j]; //向后移动元素
            }
            str[j+1] = str[0]; //插入元素
        }
    }
}

// str[0]不做为哨兵，作为待排序元素
void insertsort2(int str[], int n){
    int i, j, tmp;
    for(i=1; i<=n-1; i++){
        if(str[i] < str[i-1]){
            tmp = str[i];
            for(j=i-1; str[j]>tmp && j>=0; j--){
                str[j+1] = str[j];
            }
        }

        str[j+1] = tmp;
    }
}

void halfinsertsort(int str[], int n){
    int i, j, low, high, mid, tmp;

    for(i=1; i<=n-1; i++){
        if(str[i] < str[i-1]){ // 带插入str[i]
            tmp = str[i];
            low = 0;
            high = i-1;
            while(low < high){ //寻找插入位置
                mid = (low + high)/2;
                if(str[mid] > tmp){
                    low = mid + 1;
                } else{
                    high = mid - 1;
                }
            }
            for(j=i-1; j>=high; j--){ //整体移动
                str[j+1] = str[j];
            }
            str[j+1] = tmp;
        }
    }
}
int main(int argc, char *argv[]){
    const int number = 10;
    int *str = getArray(number);
    int str1[] = {1};
    int str2[] = {1, 1};
    int str21[] = {2, 1};

    printArray(str1, 1);
    insertsort1(str1, 1);
    printArray(str1, 1);
    insertsort2(str1, 1);
    printArray(str1, 1);
    halfinsertsort(str, number);
    printArray(str, number);


    printArray(str2, 2);
    insertsort1(str2, 2);
    printArray(str2, 2);
    insertsort2(str2, 2);
    printArray(str2, 2);
    halfinsertsort(str, number);
    printArray(str, number);

    printArray(str21, 2);
    insertsort1(str21, 2);
    printArray(str21, 2);
    insertsort2(str21, 2);
    printArray(str21, 2);
    halfinsertsort(str, number);
    printArray(str, number);

    printArray(str, number);
    insertsort1(str, number);
    printArray(str, number);
    insertsort2(str, number);
    printArray(str, number);
    halfinsertsort(str, number);
    printArray(str, number);

    return 0;
}
