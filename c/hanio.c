#include <stdio.h>
//#include <stdlib.h> // for atoi

int i = 0; //记录步数

void move(int n, char src, char dst) {
    printf("[STEP %d]: 将%d号盘子%c-->%c\n", i++, n, src, dst);
}

void hanio(int n, char src, char tmp, char dst) {

    if(n == 1){
        move(1, src, dst); //一个盘子直接移动到目的地
    } else {
        hanio(n-1, src, dst, tmp); //将src上面的n-1个盘移动到tmp上
        move(n, src, dst); //将src剩下的一个盘移动到dst上
        hanio(n-1, tmp, src, dst); //将tmp上的n-1个盘移动到dst上
    }

}

int main(int argc, char *argv[])
{
    int n = 0;
    char src = 'A';
    char tmp = 'B';
    char dst = 'C';

    printf("请输出盘子的个数:\n");
    scanf("%d", &n);
    printf("方法如下:\n");
    hanio(n, src, tmp, dst);
}
