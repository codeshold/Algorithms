//
// Created by Wenfeng Shi on 17/4/2.
//

#include "minicost.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <deque>
#include <string>
#include <map>
#include <set>

using namespace std;


#define MAXN (1000+2)       // 网络节点数 + S + D
#define MAXM (5000*2*2)      // 5000*2*2条就行(5000边*双向*反向）
#define INF 1000000           // 带宽消耗总需求是[0, 5000]
#define MAX_CONSUME 500     // 消费节点最大数

// 统计值
#ifdef _DEBUG
int g_mcmf_count = 0;
int g_dfs_path_count = 0;
int g_mcmf_time = 0;
int g_dfs_path_time = 0;
#endif


typedef struct edge
{
    struct edge *next;     // 下一个节点
    struct edge *pair;     // 配对边
    int to;
    int residual;   // 剩余容量
    int cost;       // 单位费用
    int flow;       // 边上的流
} edge;
edge g_edge[MAXM];      // 边集
edge *g_list[MAXN];     // 每个顶点的链表表头指针
int g_node_matrix[MAXN][MAXN] = {{0,},}; // 节点邻接矩阵
int E_COUNT = -1;       // 边计数器, 从0开始
bool g_init_flag = true; // true表示要保存边的信息
int g_matrix[MAXM][4];    // from, to , cost, residual
int M_COUNT = -1;

typedef struct node{
    int v;
    int degree;
    int capacity;
} node;
node g_node_sort_cap[MAXN];         // 按capacity升序排列
node g_node_sort_deg[MAXN];         // 按degree升序排列
node g_node_sort_cost[MAXN];        // 按cost = 0.2*capacity + 0.8*degree 升序排列
int g_sort_type = 0;                // 0 按照capacity升序取点，1 按照degree取点, 2 按照cost取点
//set<int> *g_node_set[MAXN];

typedef struct consume
{
    int netnode;
    int need;
}consume;
consume g_consume[MAX_CONSUME]; // 消费节点
map<int, int> g_consume_map;
int V_CONSUME;  // 消费节点数
int V_NUM;      // 顶点数
int SRV_COST;   // 服务器成本


// spfa && argument, 每次使用前初始化
int g_inq[MAXN];            // 每个顶点是否在队列中（1：在， 0: 不在）
int g_dist[MAXN];           // 当前最短路径
int g_path[MAXN];           // 最短路径中，当前节点的前驱节点
edge *g_path_edge[MAXN];    // 最短路径中，当前节点的前驱边
deque<int> Q;   // 队列中的节点为顶点序列号


int V_SRC = -1;      // 源节点
int V_DST = -1;      // 目的节点
int g_flow;     // 纪录总流量
string g_result;        // 保存路径结果
int g_flow_count = 0;   // 保存路径总数

int  g_min_cost;
int g_min_flow;
int g_min_flow_count;

clock_t g_start_time;
bool g_exit_flag = false;   // 递归中，全局退出标志！

deque<edge*> S;     // 栈 for dfs_path()



long g_count_old = 0;
long g_count_new = 0;

static int mcmf(int srv_cost);
static void init_list_edge();
static bool spfa();
static int argument();
static void dfs_path();
static void node_set_sort(int flag, int count); // 升序排序(1 表示按capacity排序； 2 表示按degree排序
static void calculate(int nodes[], int count, int need);
static void cal_all_combination(node arr[],int data[],int start, int end,int index , int r, int rneed, int need);
static bool check_path();

char *cdn_minicost()
{

    int need_sum = 0;   // 总费用
    int cdn_cost;
    int i, n;
    int v_count = V_NUM;    // 服务器节点个数
    char buf[10];

    g_start_time = clock(); // 定时器

    // 按容量对顶点进行排序
    node_set_sort(1, v_count);
//    for(i = 0; i < v_count; i++)
//    {
//        printf("[%d] %d: %d %d \n", __LINE__, i, g_node_sort_cap[i].v, g_node_sort_cap[i].capacity);
//    }

    V_NUM += 2;
    V_DST = V_NUM - 2;
    V_SRC = V_NUM - 1;

    // 1. 添加汇聚节点
    for(i = 0; i < V_CONSUME; i++)
    {
        add_edge(g_consume[i].netnode, V_DST, 0, g_consume[i].need);
        need_sum += g_consume[i].need;
    }
    g_init_flag = false;

    // 2. 判断是否存在可行解
    for(i = 0; i < V_CONSUME; i++)
    {
        add_edge(V_SRC, g_consume[i].netnode, 0, INF);
    }
    cdn_cost = V_CONSUME * SRV_COST;

    g_min_cost = mcmf(cdn_cost);
    g_min_flow = g_flow;
    //PRINT("[%d]%d, %d, %d \n", __LINE__, g_min_cost, cdn_cost, g_min_cost-cdn_cost);
    if(g_flow != need_sum)
    {
        sprintf(buf, "%s", "NA");
        g_result = buf;
        return (char*)g_result.c_str();
    }
    else
    {
        dfs_path();
        g_min_flow_count = g_flow_count;
        n = sprintf(buf, "%d\n\n", g_min_flow_count);
        g_result.insert(0, buf, n);
        PRINT("[%d] need_sum:%d, g_min_flow:%d, g_min_cost: %d, g_min_flow_count: %d \n\n",
              __LINE__, need_sum, g_min_flow, g_min_cost, g_min_flow_count);
//        check_path();
//        return (char*)g_result.c_str();
    }

    // test
//////////////////////////////////////////////////////////////////////////////////
    //case 1
//    int arr[] = {6, 7, 13, 17, 35, 41, 48};  //need_sum:381, g_min_flow:381, g_min_cost: 2136/2520, g_min_flow_count: 9
    // case 3
//    int arr[] = {10, 22, 26, 29, 35};      //need_sum:340, g_min_flow:340, g_min_cost: 2111/2700, g_min_flow_count: 14
//    int arr[] = {10, 22, 26, 35};          //need_sum:340, g_min_flow:340, g_min_cost: 2111/2700, g_min_flow_count: 18
    // case 4
    int arr[] = {12, 15, 20, 22, 26, 37, 48};   //need_sum:284, g_min_flow:284, g_min_cost: 1967/2160, g_min_flow_count: 13

    calculate(arr, sizeof(arr)/sizeof(arr[0]), need_sum);
    PRINT("[%d] need_sum:%d, g_min_flow:%d, g_min_cost: %d, g_min_flow_count: %d \n\n",
          __LINE__, need_sum, g_min_flow, g_min_cost, g_min_flow_count);
    check_path();
    return (char*)g_result.c_str();

//////////////////////////////////////////////////////////////////////////////////

    // 3. 选定CDN服务器位置
    int lower = 1 + need_sum / g_node_sort_cap[v_count-1].capacity;  // 最少节点数
    int *nodes = (int*)calloc(V_CONSUME-1, sizeof(int));        // 保存CDN服务器组合数
    g_sort_type = 0; // 按照capacity

    for(;lower < V_CONSUME; lower++)
    {
        // 结果保存在g_result中
        cal_all_combination(g_node_sort_cap, nodes, 0, v_count-1 ,0 ,lower, need_sum, need_sum);
        memset(nodes, 0, (V_CONSUME-1)*sizeof(int));
    }

//    PRINT("[%d]g_min_flow:%d, need_sum:%d, g_min_flow_count: %d, g_min_cost: %d\n\n",
//          __LINE__, g_min_flow, need_sum, g_min_flow_count, g_min_cost);

    if(g_exit_flag)
    {
        PRINT("[%d]time!!!!!!\n", __LINE__);
    }
    PRINT("[%d]statistical information--g_mcmf_count:%d(%d, %d); g_dfs_path_count:%d(%d, %d), all time:%ld\n",
           __LINE__, g_mcmf_count, g_mcmf_time, g_mcmf_time/g_mcmf_count,
           g_dfs_path_count, g_dfs_path_time, g_dfs_path_time/g_dfs_path_count,
           (clock()-g_start_time));
    return (char*)g_result.c_str();
}


static void init_list_edge()
{
    int i;

    memset(g_list, 0, MAXN * sizeof(g_list[0]));
    memset(g_edge, 0, MAXM * sizeof(g_edge[0]));
    E_COUNT = -1;

    for(i = 0; i <= M_COUNT; i++)
    {
        add_edge(g_matrix[i][0], g_matrix[i][1], g_matrix[i][2], g_matrix[i][3]);
    }
}


// true 表示找到了最短路径
static bool spfa()
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
    memset(g_path_edge, 0, sizeof(g_path_edge));

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
            //PRINT("[%d] cur:%d, tmp->to:%d, tmp->residual:%d, tmp->cost:%d\n", __LINE__, cur, tmp->to, tmp->residual, tmp->cost);
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
static int argument()
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

// 返回总费用
static int mcmf(int srv_cost)
{
    int flow_cost = 0;

#ifdef _DEBUG
    g_mcmf_count++;     // statistics
    clock_t begin = clock();
#endif
    // 初始化
    g_flow = 0;

    while(spfa())
    {
        flow_cost += argument();
    }
#ifdef _DEBUG
    g_mcmf_time += clock()-begin;   // statistics
#endif
    return flow_cost + srv_cost;
}

// 找流量路径, 结果存放在g_result中
static void dfs_path()
{
    edge *p;
    int min = INF;
    unsigned int i;

#ifdef _DEBUG
    g_dfs_path_count++;
    clock_t begin = clock();
#endif
    memset(g_inq, 0, sizeof(g_inq)); //初始化为0
    p = g_list[V_SRC];
    g_inq[V_SRC] = 1;
    g_flow_count = 0;

    g_result.clear();
    S.clear();
    g_result = "";

    while(1)
    {
        // 寻找满足条件的边
        while(p->flow <= 0 || g_inq[p->to] == 1)    // 无流量或在队列中
        {
            p = p->next;
            if(p == NULL) break;
        }
        if(p == NULL)
        {
            if(!S.empty())
            {
                p = S.back();
                S.pop_back();
                continue;   // 重新开始循环
            }
            break;          // 路径查找结束
        }

        S.push_back(p);     // 压入栈
        // g_inq[p->to] = 1;   // 在队列中
        if(min > p->flow)
        {
            min = p->flow;
        }
        if(p->to == V_DST)
        {
            // 1. 保存路径信息
            //add_print_flow(min, p->pair->to);
            for(i = 0; i < S.size()-1; i++)
            {
                g_result += to_string(S[i]->to) + " ";
                //PRINT("%d ", S[i]->to);
            }
            g_result += to_string(g_consume_map[p->pair->to]) + " " + to_string(min) + "\n";
            g_flow_count++;
            //PRINT("netnode: %d, consume: %d, flow: %d\n", netnode, g_consume_map[netnode], flow);

            // 2. 调整flow, 并更新min
            int min_new = INF;
            for(i = 0; (unsigned)i < S.size(); i++)
            {
                S[i]->flow -= min;
                if(S[i]->flow > 0 && min_new > S[i]->flow) {
                    min_new = S[i]->flow;
                }
            }

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
        }
        else
        {
            g_inq[p->to] = 1;   // 在队列中
            p = g_list[p->to];
        }
    }
#ifdef _DEBUG
    g_dfs_path_time += clock() - begin;
#endif
}


void set_num(int vNum, int linkNum, int consumerNum, int srvcost)
{
    V_NUM = vNum;
    V_CONSUME = consumerNum;
    SRV_COST = srvcost;
}

void set_consume(int index, int netnode, int need)
{
    g_consume[index] = {netnode, need};
    g_consume_map[netnode] = index;
    //PRINT("netnode:%d, index:%d\n", netnode, index);
}

//插入邻接表, cost 表示单位费用; res 表示容量
void add_edge(int from, int to, int cost, int res)
{
    //PRINT("%d %d %d %d\n", from , to, res, cost);
    if(g_init_flag)
    {
        M_COUNT++;
        g_matrix[M_COUNT][0] = from;
        g_matrix[M_COUNT][1] = to;
        g_matrix[M_COUNT][2] = cost;
        g_matrix[M_COUNT][3] = res;

        // 更新顶点的度数和容量信息
        if(from != V_SRC && from != V_DST && to != V_SRC && to != V_DST)
        {
            g_node_sort_cap[from].v = from;
            g_node_sort_cap[from].degree++;
            g_node_sort_cap[from].capacity += res;
            // 存储邻接矩阵
            g_node_matrix[from][to] = res;
//            PRINT("[%d]g_node_matrix[%d][%d] = %d\n", __LINE__, from, to, res);
        }
        //g_node_set[from]->;
    }
    edge e1 = {g_list[from], 0, to, res, cost, 0};
    edge e2 = {g_list[to], 0, from, 0, -cost, 0};

    g_edge[++E_COUNT] = e1;
    g_list[from] = &g_edge[E_COUNT];
    g_edge[++E_COUNT] = e2;
    g_list[to] = &g_edge[E_COUNT];

    g_list[from]->pair = g_list[to];
    g_list[to]->pair = g_list[from];

}

// 按流量进行升序排序
static int cmp_cap(const void* a, const void* b)
{
    node *p = (node *)a;
    node *q = (node *)b;

    return (p->capacity - q->capacity);
}

static int cmp_degree(const void* a, const void* b)
{
    node *p = (node*)a;
    node *q = (node*)b;

    return (p->degree - q->degree);
}

static void node_set_sort(int flag, int count)
{
    if(flag == 1)
    {
        qsort(g_node_sort_cap, count, sizeof(g_node_sort_cap[0]), cmp_cap);
    }
    else if(flag == 2)
    {
        qsort(g_node_sort_cap, count, sizeof(g_node_sort_cap[0]), cmp_degree);
    }
}

static void calculate(int nodes[], int count, int need)
{
    int cdn_cost = 0, cost = 0;
    int n = 0;
    char buf[10];

    init_list_edge();

    //PRINT("[%d]CDN node: ", __LINE__, count);
    n = count;
    while(count > 0)
    {
        add_edge(V_SRC, nodes[count-1], 0, INF);
        //PRINT("%d ", nodes[count-1]);
        count--;
    }
    //PRINT(" \n");
    cdn_cost = n * SRV_COST;
    cost = mcmf(cdn_cost); // 计算当前最大流、最小费用，包含服务器费用
    //PRINT("[%d]cost:%d", __LINE__, cost-cdn_cost);
    if(g_flow == need && cost < g_min_cost)
    {
        // 保存路径信息
        g_min_cost = cost;
        g_min_flow = g_flow;
        dfs_path();
        g_min_flow_count = g_flow_count;
        n = sprintf(buf, "%d\n\n", g_flow_count);
        g_result.insert(0, buf, n);
        PRINT("[%d] need_sum:%d, g_min_flow:%d, g_min_cost: %d, g_min_flow_count: %d \n\n",
              __LINE__, need, g_min_flow, g_min_cost, g_min_flow_count);
    }
}

// data[]保存当前的组合; start, end 剩余数组的起始位置; index 已经找到的组合数的个数; r 总共需要的组合数的个数;
// need 总的需求; rneed 剩余的需求
static void cal_all_combination(node arr[],int data[],int start, int end,int index , int r, int rneed, int need)
{
    if(g_exit_flag) return;

    int i, j, count, tmp, sum;

    if(index == r ){
        if(rneed <= 0)
        {
//            PRINT("[%d]", __LINE__);
//            for(int i=0; i<r; i++)
//            {
//                PRINT("%d ", data[i]);
//            }
//            PRINT("\n");
            // 计算节点内部流（重复计算了的)
            sum = 0;
            for(i = 0; i < r-1; i++)
            {
                for(j = i+1; j < r; j++)
                {
                    //PRINT("[%d]%d %d", __LINE__, data[i], data[j]);
                    sum += g_node_matrix[data[i]][data[j]];
                }
            }
            g_count_old++;
            //PRINT("[%d]%ld \n", __LINE__, g_count_old);
            if(2*sum + rneed <= 0)
            {
                g_count_new++;
                // 计算函数
                calculate(data, r, need);
                if(clock() - g_start_time > EXIT_SECOND * CLOCKS_PER_SEC)
                {
                    g_exit_flag = true;
                    PRINT("[%d]old_count: %ld; new_count: %ld, diff:%ld\n",
                          __LINE__, g_count_old, g_count_new, g_count_old-g_count_new);
                }
            }
        }
        return;
    }
    //如果剩下的数组个数不够个就直接返回。
    if(end - start + 1 < r - index) return;

    // 按照不同的排序方式，剔除不满足条件的节点
//    if(g_sort_type == 0) // capacity
//    {
        i = end;
        count = 0;
        tmp = 0;
        //PRINT("=== %d %d %d\n", i, count, tmp);
        // 剩下的数组个数容量之和达不到rneed
        for(count = r-index; count > 0; count--)
        {
            tmp += arr[i--].capacity;
        }
        if(tmp < rneed)
        {
            return;
        }
//    }

    data[index] = arr[end].v; //记录当前数据
    cal_all_combination(arr, data , start, end-1, index+1, r, rneed-arr[end].capacity, need); //包括end
    cal_all_combination(arr, data, start, end-1, index, r, rneed, need); //不包括end
}


#include <vector>
static bool check_path() {
    PRINT("================================[check]================================================\n");

    init_list_edge();

    bool error_flag = false;
    char *strc = new char[strlen(g_result.c_str()) + 1];
    char *line;
    char *node;
    vector<string> resultVec;
    deque<int> checkQ;
    int n = 0, flow = 0, from = 0, to = 0, len = 0;
    char *outer_ptr = NULL;
    char *inner_ptr = NULL;

    strcpy(strc, g_result.c_str());
    line = strtok_r(strc, "\n", &outer_ptr);
    n = atoi(line);
    if (n != g_min_flow_count) {
        PRINT("[%d] 总的流数不相等！ %d != %d\n", __LINE__, n, g_min_flow_count);
        error_flag = true;
    }
    PRINT("[%d] 总的流数为 %d \n", __LINE__, n);
    line = strtok_r(NULL, "\n", &outer_ptr);

    // 解析流量情况
    while (line != NULL) {
//        PRINT("[%d]%s\n", __LINE__, line);
        // 1. 解析每个flow的情况
        node = strtok_r(line, " ", &inner_ptr);
        PRINT("[%d]", __LINE__);
        while (node != NULL) {
            n = atoi(node);
            checkQ.push_back(n);
            PRINT("%d ", n);
            node = strtok_r(NULL, " ", &inner_ptr);
        }
        flow = n;   //获取了容量
        PRINT("\n");

        // 2. 核对信息
        from = checkQ.front();  // 获取每条flow的第一个节点
        checkQ.pop_front();
        len = checkQ.size();
        if (len == 2)    // 消费节点直接和服务器相连
        {
            to = checkQ.front();    // get consume
            checkQ.pop_front();
            checkQ.pop_front();

            PRINT("[%d]cdn服务器节点 %5d 和 消费节点 %5d 直接连接，使用了 %5d 带宽\n", __LINE__, from, to, flow);
            if (flow > g_consume[to].need) {
                PRINT("[ERROR]消费节点 %5d 的剩余需求仅仅是 %5d, 但是提供了 %5d 的流\n",
                      to, g_consume[to].need, flow);
                error_flag = true;
            } else {
                g_consume[to].need -= flow; // 消费节点需求减少了
            }
        } else {
            int i;
            for (i = 0; i < len - 2; i++) // 不处理最后两个节点(consume, flow)
            {
                to = checkQ.front();
                checkQ.pop_front();
                if (flow > g_node_matrix[from][to]) {
                    PRINT("[%d] 节点%d->节点%d 的流量超出了剩余的可用带宽(可用:%d, 需要%d)\n",
                          __LINE__, from, to, g_node_matrix[from][to], flow);
                    error_flag = true;
                }
                else
                {
                    g_node_matrix[from][to] -= flow;
                }
                from = to;
            }
            to = checkQ.front(); // get consume
            checkQ.pop_front(); // del consume
            checkQ.pop_front(); // 去掉最后的元素，即容量值
            if (flow > g_consume[to].need) {
                PRINT("[ERROR]消费节点 %5d 的剩余需求仅仅是 %5d, 但是提供了 %5d 的流\n",
                      to, g_consume[to].need, flow);
                error_flag = true;
            }
            else
            {
                g_consume[to].need -= flow; // 消费节点的需求减少
            }
            if (!checkQ.empty()) {
                PRINT("[%d]队列不为空！！！！！\n", __LINE__);
                error_flag = true;
            }
        }
        line = strtok_r(NULL, "\n", &outer_ptr);
    }
    int i;
    for (i = 0; i < V_CONSUME; i++)
    {
        if(g_consume[i].need != 0)
        {
            PRINT("[%d] 没有满足消费节点 %5d 的需求，其还需要 %5d 的流量\n\n", __LINE__, i, g_consume[i].need);
        }
    }

    delete[] strc;

    if(error_flag)
    {
        PRINT("\n\n[%d]出现错误啦！！！！傻逼！！！\n\n", __LINE__);
    }

    PRINT("================================[check over]================================================\n");

    return error_flag;
}
/*void init()
{
    int s, d, r, c;
    int i;
    int link_num, con_num;
    scanf(" %d %d %d", &V_NUM, &E_NUM, &V_CONSUME);
    scanf(" %d ", &SRV_COST);
    //scanf(" %d %d ", &V_SRC, &V_DST);  // 总顶点数

    // 起、末、容、单位费用
    for(i = 0; i < E_NUM; i++)
    {
        scanf(" %d %d %d %d", &s, &d, &r, &c);
        if(r == -1)
        {
            r = INF;
        }
        add_edge(s, d, c, r);
        //printf("%d %d %d %d\n", s, d, r, c);
    }
    // 消费节点、相连的网络节点、带宽消耗
    for(i = 0; i < V_CONSUME; i++)
    {
        //xscanf(" %d %d %d ", );
    }
}*/

/*
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
}*/

/*
inline void add_print_flow(int flow, int netnode)
{
    int i;

    for(i = 0; i < S.size(); i++)
    {
        g_result += to_string(S[i]->to) + " ";
        //PRINT("%d ", S[i]->to);
    }
    g_result += to_string(g_consume_map[netnode]) + " " + to_string(flow) + "\n";
    g_flow_count++;
    //PRINT("netnode: %d, consume: %d, flow: %d\n", netnode, g_consume_map[netnode], flow);
}
*/