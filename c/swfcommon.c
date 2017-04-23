#include "swfcommon.h"

int* getArray(unsigned int n) {

    int *array = (int*)malloc(sizeof(int) * n);
    int *cur = array;
    int i = n;

    srand((unsigned int)time(NULL));

    while(i--){
        *cur++ = rand() % 10;
    }

    return array;
}

void printArray(int array[], unsigned int n) {
    for(int i = 0; i < n; i++) {
        printf("%2d ", array[i]);
    }
    printf("\n");
}

/*
int main(int argc, char *argv[])
{
    int *array = getArray(10);
    printArray(array, 10);
    return 0;
}
*/
