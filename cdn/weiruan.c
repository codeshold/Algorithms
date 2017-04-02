#include <stdio.h>
#include <stdlib.h>

typedef struct _node
{
    double data;
    struct _node *next;
} Node, *pNode;

pNode g_head = NULL;
pNode g_pcur = NULL;

int N = 0;
int Q = 0;
int P = 0;

int cal(int curP, int curN, int curReq, int status, double sumP)
{
    printf("curP:%d, curN:%d, curReq:%d, status:%d, sumP:%d\n", curP, curN, curReq, status, sumP);
    int pro;

    if(curN == N)
    {
        if(g_pcur->next == NULL)
        {
            g_pcur->next = (pNode)calloc(1, sizeof(Node));
        }
        g_pcur->next->data = curReq * sumP;
        printf("%d: %f, curReq: %d , curN: %d \n", __LINE__, g_pcur->next->data, curReq, curN);
        g_pcur = g_pcur->next;
        return 0;
    }
    else
    {
        if(status == 1) // ok
        {
            pro = P/(1<<curN);
            //printf("P: %d, curN:%d, pro:%d, 2<<curN:%d\n", P, curN, pro, 2<<curN);
            cal(pro, curN+1, curReq+1, 1, sumP * (double)(pro)/100);  // ok
            cal(100-pro, curN, curReq+1, 0, sumP * (double)(100-pro)/100); // no ok
        }
        else //no ok
        {
            pro = (curP + Q >= 100) ? 100 : (curP + Q);
            cal(pro, curN+1, curReq+1, 1, sumP * ((double)pro)/100); //ok
            if(pro != 100)
            {
                cal(100-pro, curN, curReq+1, 0, sumP * (double)(100-pro)/100); //no ok
            }
        }

    }
    return 0;
}

int main(void)
{
    //scanf("%d %d %d", &P, &Q, &N);
    P = 50;
    Q = 75;
    N = 2;
    g_head = (pNode)malloc(sizeof(Node));
    g_head->next = NULL;
    g_pcur = g_head;

    //ok
    //int cal(int curP, int curN, int curReq, int status, double sumP)
    cal(P, 1, 1, 1, (double)P/100);
    //printf("%f\n", (double)P/100);

    //no ok
    cal(100-P, 0, 1, 0, (double)(100-P)/100);

    double result = 0.0;
    pNode tmp = g_head;

    while(tmp->next != NULL)
    {
        result += tmp->next->data;
        //printf("%d: %f\n", __LINE__, tmp->next->data);
        tmp = tmp->next;
    }
    printf("%d: %f\n", __LINE__, result);


    return 0;
}