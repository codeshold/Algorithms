#include <stdio.h>

// Joseph Circle
int main(int argc, int *argv[])
{
    int result = 0;
    int i;
    int n, m;

    scanf("%d%d", &n, &m);

    for(i = 2; i <= n; i++){
        result = (result + m) % i;
    }

    printf("\nThe winner is %d\n", result+1);

    return 0;
    
}
