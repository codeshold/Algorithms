#include <stdio.h>
#include <stdlib.h>
#include <queue>

using namespace std;

#define INF 1000000
#define MAXN 1000

typedef struct ArcNode
{
    int to;
    int weight;
    ArcNode *next;
} ArcNode;

queue<int> Q;   // 队列中的节点为顶点序列号
int V_SRC;      // 源节点
int V_NUM;      // 顶点总数

ArcNode *g_list[MAXN];    // 每个顶点的链表表头指针
int g_inq[MAXN];          // 每个顶点是否在队列中（1：在， 0: 不在）
int g_dist[MAXN];         // 纪录源节点到其它节点的最短距离
int g_path[MAXN];         // 纪录节点的前驱节点

void spfa()
{
    int i;      // 循环变量
    int cur;    // cur纪录队列头顶点的序号
    int ver;
    ArcNode *temp;

    for(i = 0; i < V_NUM; i++) // 初始化
    {
        g_dist[i] = INF;
        g_path[i] = V_SRC;
        g_inq[i] = 0;  // 顶点不在队列中
    }
    g_dist[V_SRC] = 0;
    g_path[V_SRC] = V_SRC;
    g_inq[V_SRC] = 1;
    Q.push(V_SRC);

    while(!Q.empty())
    {
        cur = Q.front();
        Q.pop();
        g_inq[cur] = 0;
        temp = g_list[cur];
        while(temp != NULL)
        {
            ver = temp->to;
            if(g_dist[ver] > g_dist[cur] + temp->weight)
            {
                g_dist[ver] = g_dist[cur] + temp->weight;   // 更新g_dist
                g_path[ver] = cur;  // 纪录g_path
                if(g_inq[ver] == 0)
                {
                    Q.push(ver);
                    g_inq[ver] = 1;
                }
            }
            temp = temp->next;
        }
    }
}

int main()
{
    int i, j;           //循环变量
    int u, v, w;        //边的起点和终点及权值
    int src = 0;        //定义源节点
    ArcNode *tmp;


    scanf("%d", &V_NUM);    //读入顶点个数
    memset(g_list, 0, sizeof(g_list));

    //构造邻接表
    while(1)
    {
        scanf(" %d %d %d ", &u, &v, &w);
        if(u == -1 && v == -1 && w == -1)
        {
            break;
        }
        tmp = (ArcNode *)malloc(sizeof(ArcNode));
        tmp->to = v;
        tmp->weight = w;
        tmp->next = NULL;
        if(g_list[u] == NULL) {
            g_list[u] = tmp;
        }
        else
        {
            tmp->next = g_list[u];
            g_list[u] = tmp;
        }
    }

    //求顶点0到其他顶点的最短路径
    V_SRC = 0;
    for(i = 0; i < V_NUM; i++)
    {
        ArcNode *p = g_list[i];
        printf("[%d]: ", i);
        while(p != NULL)
        {
            printf("->%d", p->to);
            p = p->next;
        }
        printf("\n");
    }
    spfa();

    //释放边链表
    for(j = 0; j < V_NUM; j++)
    {
        tmp = g_list[j];
        while(tmp != NULL)
        {
            g_list[j] = tmp->next;
            free(tmp);
            tmp = g_list[j];
        }
    }

    //输出最短路径
    int shortest[MAXN];
    for(i = 1; i < V_NUM; i++)
    {
        printf("%d\t", g_dist[i]);    //输出最短路径长度
        memset(shortest, 0, sizeof(shortest));
        int k = 0;
        shortest[k] = i;    //目的顶点为i
        while(g_path[shortest[k]] != src)
        {
            k++;
            shortest[k] = g_path[shortest[k-1]];
        }
        k++;
        shortest[k] = 0;
        for(j = k; j > 0; j--)
        {
            printf("%d->", shortest[j]);
        }
        printf("%d\n", shortest[0]);
    }
    return 0;
}