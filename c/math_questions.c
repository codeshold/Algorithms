#include <stdio.h>

//x,y����С������(lcm, gcd)�����Լ���ĳ˻�����x*y

//��ٷ�
int GenGCD1(int a, int b)
{
    int i;
    for(i=0; ; i++){
        if(a%i == 0 && b%i == 0){
            break;
        }
    }

    return i;
}
//շת���
int GenGCD2(int a, int b)
{
    int c;
    while(b != 0) {
        c = a % b;
        a = b;
        b = c;
    }
    return a;
}
//�����
int GenGCD3(int a, int b)
{
    while(a != b) {
        if(a > b) {
            a = a - b;
        }
        else {
            b = b - a;
        }
    }    

    return a;
}
//��ٷ�
int GenLCM1(int a, int b)
{
    int i;
    for(i = 0; ; i++) {
        if(i%a == 0 && i%b == 0) {
            break;
        }
    } 
    return i;

}
//����
int GenLCM2(int a, int b)
{
     int m = a*b;

    return m/GenGCD1(a, b);
}




// �ж�n�Ƿ�Ϊ����
// ���Ӷ�ΪO(sqrt(n)/2)
int PrimeCheck(int n)
{
    int i;

    if(n < 2) {
        return -1;
    }
    if(n == 2) {
        return 0;
    }
    if(n % 2 == 0) {
        return -1;
    } 

    for(i = 3; i*i <= n; i += 2){
        if(n % i == 0){
            return -1;
        }
    }

    return 0;
}


//������������
int GenPrime(int primes[], int num)
{
    int j, count, flag, i;

    if(num < 1) return -1;
    if(num == 1) {
        primes[0] = 2;
        return 0;
    }
    primes[0] = 2;
    primes[1] = 3;

    for(i = 5,  count = 2; count < num; i += 2) {
        flag = 0;
        for(j = 1; primes[j]*primes[j] <= i; j++) {
            if(i % primes[j] == 0) { //��������
                flag = -1;
                break;
            }
        }
        if(flag == 0){
            primes[count++] = i;
        }

    }

    return 0;
}

// test
int main(int argc, char *argv[])
{
    int i = 7;
    
    if(PrimeCheck(i) == 0) {
        printf("%d is a prime!\n", i);
    }
    else {
    
        printf("%d is not a prime!\n", i);
    }

    int a[10] = {0,};
    GenPrime(a, 10);
    for(i=0; i<10; i++) {
        printf("%d ", a[i]); 
    }
    printf("\n");
    
    // lcm, gcd
    printf("%d\n", GenGCD1(12, 42));
    printf("%d\n", GenGCD2(12, 42));
    printf("%d\n", GenGCD3(12, 42));
    printf("%d\n", GenLCM1(12, 42));
    printf("%d\n", GenLCM2(12, 42));


    return 0;
} 

