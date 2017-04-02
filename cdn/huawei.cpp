//
// Created by Wenfeng Shi on 17/4/1.
//

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
int V_DST;      // 目的节点
int V_NUM;      // 顶点总数

ArcNode *g_list[MAXN];    // 每个顶点的链表表头指针
int g_inq[MAXN];          // 每个顶点是否在队列中（1：在， 0: 不在）
int g_dist[MAXN];         // 纪录源节点到其它节点的最短距离
int g_path[MAXN];         // 纪录节点的前驱节点


//返回1表示找到了最短路径，0表示未找到
int spfa()
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
    g_path[V_SRC] = -1;
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

    return (g_dist[V_DST] != INF) ? 1 : 0;
}

int main()
{
    V_DST = 1;
}