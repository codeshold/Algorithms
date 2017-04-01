#include <stdio.h>
#include <stdlib.h>
#include <deque>
#include <string>
//#include <iostream>
//#include <stack>

using namespace std;

#define MAXN 1003       // 最大顶点数
#define MAXM 10002*4    // 最大边数
#define INF 1000000

typedef struct edge
{
    struct edge *next;     // 下一个节点
    struct edge *pair;     // 配对的
    int to;         //
    int residual;   // 剩余容量
    int cost;       // 单位费用
    int flow;   // 边上的流
} edge;
edge g_edge[MAXM];      // 边集
edge *g_list[MAXN];     // 每个顶点的链表表头指针

int g_inq[MAXN];            // 每个顶点是否在队列中（1：在， 0: 不在）
int g_dist[MAXN];           // 当前最短路径
int g_path[MAXN];           // 最短路径中，当前节点的前驱节点
edge *g_path_edge[MAXN];    // 最短路径中，当前节点的前驱边

deque<int> Q;   // 队列中的节点为顶点序列号

int V_NUM;      // 顶点数
int V_SRC;      // 源节点
int V_DST;      // 目的节点
int E_COUNT = -1;    // 边计数器

int g_flow;     // 纪录总流量


deque<edge*> S;     // 栈
string g_result;    // 保存路径结果
int g_flow_count = 0;   // 保存路径总数


//插入邻接表, cost 表示单位费用; res 表示容量
void add_edge(int from, int to, int cost, int res)
{
    edge e1 = {g_list[from], 0, to, res, cost, 0};
    edge e2 = {g_list[to], 0, from, 0, -cost, 0};

    g_edge[++E_COUNT] = e1;
    g_list[from] = &g_edge[E_COUNT];
    g_edge[++E_COUNT] = e2;
    g_list[to] = &g_edge[E_COUNT];

    g_list[from]->pair = g_list[to];
    g_list[to]->pair = g_list[from];
}

void init()
{
    int s, d, r, c;
    scanf(" %d", &V_NUM);  // 总顶点数
    scanf(" %d %d ", &V_SRC, &V_DST);  // 总顶点数

    //printf(" %d\n", V_NUM);
    // 起、末、容、单位费用
    while(scanf(" %d %d %d %d", &s, &d, &r, &c) != EOF)
    {
        if(r == -1)
        {
            r = INF;
        }
        add_edge(s, d, c, r);
        //printf("%d %d %d %d\n", s, d, r, c);
    }
}

// true 表示找到了最短路径
bool spfa()
{
    int i;
    int cur, ver;
    edge *tmp;

    for(i = 0; i < V_NUM; i++)
    {
        g_dist[i] = INF;
        g_path[i] = -1;
        g_inq[i] = 0;  // 顶点不在队列中
    }
    Q.clear();
    Q.push_back(V_SRC);
    g_dist[V_SRC] = 0;
    g_path[V_SRC] = -1; //V_SRC; // -1
    g_inq[V_SRC] = 1;

    while(!Q.empty())
    {
        cur = Q.front();
        Q.pop_front();
        g_inq[cur] = 0;
        for(tmp = g_list[cur]; tmp != NULL; tmp = tmp->next)
        {
            ver = tmp->to;
            // 松弛
            if(tmp->residual > 0 && g_dist[ver] > g_dist[cur] + tmp->cost)
            {
                g_dist[ver] = g_dist[cur] + tmp->cost;
                g_path[ver] = cur;
                g_path_edge[ver] = tmp; // 注意！！！
                if(g_inq[ver] == 0) // 加入队列
                {
                    Q.push_back(ver);
                    g_inq[ver] = 1;
                }
            }
        }
    }

    return g_dist[V_DST] != INF;
}

//增广路算法，寻找增广路并调整流量, 返回调整的费用
int argument()
{
    int i;
    int delta = INF;     // 可改进量
    int flow_cost = 0;   // 费用
    edge *e;

    // 获取改进量min
    for (i = V_DST; g_path[i] != -1; i = g_path[i])
    {
        e = g_path_edge[i];
        if( e->residual < delta)
            delta = e->residual;
    }
    // 调整流量
    for(i = V_DST; g_path[i] != -1; i = g_path[i])
    {
        e = g_path_edge[i];
        e->residual -= delta;
        e->flow += delta;                 // path
        e->pair->residual += delta;       // 注意！！！
        e->pair->flow -= delta;           // path
        flow_cost += e->cost * delta;     // 计算费用！！！
    }
    // 纪录最大流
    if(delta != INF)
    {
        g_flow += delta;
        //printf("%d\n", g_flow);
    }

    return flow_cost;
}


inline void add_print_flow(int flow)
{
    int i;
    printf("[%d] ", V_SRC);
    for(i = 0; i < S.size(); i++)
    {
        g_result += to_string(S[i]->to) + " ";
        printf("%d ", S[i]->to);
    }
    g_result += to_string(flow) + "\n";
    g_flow_count++;
    printf("flow: %d\n", flow);
}

// 找到流量路径
void dfs_path()
{
    edge *p;
    int min = INF;
    int i;

    memset(g_inq, 0, sizeof(g_inq)); //初始化为0
    p = g_list[V_SRC];
    g_inq[V_SRC] = 1;

    while(1)
    {
        // 寻找满足条件的边
        while(p->flow <= 0 || g_inq[p->to] == 1)    // 无流量或在队列中
        {
            //printf("[%d]\n", __LINE__);
            p = p->next;
            if(p == NULL) break;
        }
        //printf("[%d]\n", __LINE__);
        if(p == NULL)
        {
            if(!S.empty())
            {
                p = S.back();
                S.pop_back();
                continue;   // 重新开始循环
            }
            //printf("[%d]\n", __LINE__);
            break;          // 路径查找结束
        }
        //printf("[%d]\n", __LINE__);

        S.push_back(p);     // 压入栈
        // g_inq[p->to] = 1;   // 在队列中
        if(min > p->flow)
        {
            min = p->flow;
        }
        if(p->to == V_DST)
        {
            add_print_flow(min);
            //printf("[%d]\n", __LINE__);

            // 调整flow, 并更新min
            int min_new = INF;
            for(i = 0; i < S.size(); i++)
            {
                S[i]->flow -= min;
                if(S[i]->flow > 0 && min_new > S[i]->flow) {
                    min_new = S[i]->flow;
                }
            }
            //printf("[%d]\n", __LINE__);

            min = min_new;
            while(!S.empty())
            {
                p = S.back();
                S.pop_back();
                g_inq[p->to] = 0;   // 不在队列中
                if(p->flow != 0)
                {
                    break;
                }
            }
            //printf("[%d]\n", __LINE__);
        }
        else
        {
            g_inq[p->to] = 1;   // 在队列中
            p = g_list[p->to];
        }
    }
}



// 返回总费用
int mcmf()
{
    int flow_cost = 0;

    while(spfa())
    {
        flow_cost += argument();
    }

    return flow_cost;
}

void debug_print()
{
    int i;
    for(i = 0; i < V_NUM; i++)
    {
        edge *p = g_list[i];
        printf("[%d]: ", i);
        while(p != NULL)
        {
            printf("->%d(%d)", p->to, p->flow);
            p = p->next;
        }
        printf("\n");
    }
}
int main()
{
    init();
    int cost;
    //g_flow = 0;

    //return 0;
    cost = mcmf();
    dfs_path();
    printf("cost: %d\n", cost);
    printf("flow: %d\n", g_flow);
    //debug_print();

    printf("======\n%d\n", g_flow_count);
    printf("%s", g_result.c_str());
    return 0;
}

