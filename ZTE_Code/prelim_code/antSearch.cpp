#include<iostream>
#include<fstream>
#include<vector>
#include<cstring>
#include<random>
#include<ctime>
#include<cmath>
#include<algorithm>

using namespace std;

const int maxN = 1000;
const int maxE = 1000000;
const int INF = 0x3f3f3f3f;

double g_start_t, g_period_t;    // 计时 ms

int d[maxN][maxN];  // 距离矩阵
int numV, numE;     // 点数和边数
int originalNumV;   // 原图点数

int edge[maxE][2];  // 链式前向星存边
int head[maxN];
int cntE;

int S, E;           // 起点和终点
int num_mustV;      // 必经点个数
int mustV[maxN];
int limt_step;      // 最大限制步数


inline void init_graph();          // 图的初始化
inline void addE(int x, int y);    // 添加双向边
void read_and_rebuild(string filename);
inline void init_tarjan();         // tarjan算法初始化
void tarjian(int u);        // tarjan算法
void init_search();         // 蚁群算法总初始化
void updataInfo();          // 信息素更新
void updatabest();          // 更新最优解
void init_ant();            // 每只蚂蚁搜之前初始
inline void init_bath_search();   // 一批蚂蚁搜索之前初始化
int ant_search();           // 单只蚁搜索
void updataTempInfo();      // 更新局部信息素
void run_ants_search();     // 蚁群算法总控程序
void outputSolution();      // 输出解



inline void init_graph()
{
    memset(mustV, 0, sizeof(mustV));
    memset(d, 0x3f, sizeof(d));
    memset(head, -1, sizeof(head));
    cntE = 0;
}

inline void addE(int x, int y)
{
    edge[cntE][0] = y; edge[cntE][1] = head[x];
    head[x] = cntE++;
    edge[cntE][0] = x; edge[cntE][1] = head[y];
    head[y] = cntE++;
}

/*
 * 文件读取顺序：
 * 先读入点数和边数，点编号从1--N；
 * 读入步数限制
 * 读入起点终点；
 * 依次读入边（a,b,w）；a到b的无向边， 距离为w；
 * 读入必经点个数
 * 读入必经点
 * 读入必经边个数
 * 读入必经边
 * 读入禁止边个数
 * 读入禁止边
 * */
void read_and_rebuild(string filename)
{
    ifstream in(filename);
    in >> numV >> numE;
    in >> limt_step;
    originalNumV = numV;
    in >> S >> E;
    for (int i = 0; i < numE; i++)
    {
        int from, to;
        int distance;
        in >> from >> to >> distance;

        //编号如果从0到N-1
        //from++;
        //to++;

        addE(from, to);
        d[from][to] = d[to][from] = distance;
    }

    in >> num_mustV;

    for (int i = 0; i < num_mustV; i++)
    {
        int temp;
        in >> temp;
        //temp++;    // 编号从0开始 ，否则去掉该行代码
        mustV[temp] = 1;
    }

    int num_mustE;
    in >> num_mustE;
    for (int i = 0; i < num_mustE; i++) // 改必经点为必经边,原边去掉，添加点，新边的权值在后半边
    {
        int tx, ty;
        in >> tx >> ty;
        //tx++; ty++;            //编号从0开始 ，否则去掉该行代码

        d[tx][++numV] =0;
        d[numV][tx] = 0;
        d[numV][ty] = d[ty][numV] = d[tx][ty];  // 将权重记在后者上
        d[ty][tx] = d[tx][ty] = INF;            // 原边去除
        mustV[numV] = 1;
        num_mustV++;

        addE(tx, numV); // 添加新边
        addE(numV, ty);
    }

    int num_ForbidE;
    in >> num_ForbidE;
    for (int i = 0; i < num_ForbidE; i++) // 禁止边改为无穷大
    {
        int tx, ty;
        in >> tx >> ty;
        //tx++; ty++;    //编号从0开始 ，否则去掉该行代码
        d[ty][tx] = d[tx][ty] = INF;
    }
}


/*
 * 蚁群算法参数
 * */
const int Itera_time = 100000;           // 迭代次数
int NoUpdataMaxItera = 5000;              // 超过一定迭代次数无更新则退出
const int Ant_num = 50;                 // 每次发出去的蚂蚁
const double alph = 1.0, beta = 2.0;    // 权重
const double All_info = 100;
const double upperLimit = 80.0;         // 信息素浓度的限制优化
const double lowLimit = 1.0;
const double down = 0.8;                // 信息素衰减因子
vector<vector<double> > info;           // 信息素
vector<int>mark;                        // 剩余可走次数
vector<int> bestpath;                   // 全局最优解
vector<int> curpath;                    // 当前解
int bestlength;                         // 全局最优解对应的最短路
int curlength;                          // 当前解对应路长
int best_num_mustV;                     // 全局最优解对应必经点个数
int cur_num_mustV;                      // 当前必经点个数
int best_steps;                         // 最优解对应步数
int cur_steps;                          // 当前解对应步数


default_random_engine myRandom(time(NULL)); // 随机数产生器
uniform_real_distribution<double>  getrand(0, 1);
vector<vector<double> > tempInfo;

/*
 * tarjan 算法求所有解割点及其对应连通块个数
 * */
int dfn[maxN];
int low[maxN];          // 时间戳以及最早祖先的时间戳
int g_index;            // 时间戳变量
vector<int> subSets;    // 去掉节点i后形成的联通快个数（如果不是割点则为0）
int rootSons;           // dfs根的儿子数
vector<int>tarjanMark;  // 标记访问


inline void init_tarjan()
{
    g_index = 0;
    subSets.clear();
    subSets.resize(numV + 1, 0);
    rootSons = 0;
    tarjanMark.clear();
    tarjanMark.resize(numV + 1, 0);
    tarjanMark[S] = 1; // 标记起点
}


void tarjian(int u)
{
    dfn[u] = low[u] = g_index++;
    for (int j = head[u]; j != -1; j = edge[j][1])
    {
        int v = edge[j][0];
        if (!tarjanMark[v]&&d[u][v]<INF)
        {
            tarjanMark[v] = 1;
            tarjian(v);
            low[u] = min(low[u], low[v]);
            if (u == S) // 以S为根节点
                rootSons++;
            else if (dfn[u] <= low[v]) // 割点
                subSets[u]++;
        }
        else
        {
            low[u] = min(low[u], dfn[v]);
        }
    }
}


// 蚁群算法
// 第一次运行程序的初始化
void init_search()
{
    bestlength = INF;
    best_num_mustV = 2; // 起点终点
    best_steps = INF;
    info.clear();
    info.resize(numV + 1);
    for (int i = 1; i <= numV; i++)
    {
        info[i].resize(numV + 1, 10.0);
        for (int j = head[i]; j != -1; j = edge[j][1]) // 必经点必经边的入边增加信息素
        {
            int to = edge[j][0];
            if (mustV[to])
                info[i][to] += 30.0;
        }
    }
}


void updataInfo()
{
    for (int i = 1; i <= numV; i++)
    {
        for (int j = head[i]; j !=-1 ; j=edge[j][1])
        {
            int to = edge[j][0];
            double temp = down*info[i][to] + tempInfo[i][to];
            if (temp > upperLimit)
                temp = upperLimit;
            else if (temp < lowLimit)
                temp = lowLimit;
            if (mustV[to]) // 增加必经点信息素
                temp += 40;
            info[i][to] = temp;
        }
    }
}


void updatabest()
{
    if (cur_num_mustV >= best_num_mustV)
    {
        if (cur_num_mustV > best_num_mustV)  // 获得更多的必经点
        {
            best_num_mustV = cur_num_mustV;
            bestpath = curpath;
            bestlength = curlength;
            best_steps = cur_steps;

			NoUpdataMaxItera=5000;
            //cout << "更新最优解： " << endl;
            //outputSolution();
            //cout << "\n继续 搜索中 ...\n";
        }
        else if(curlength<bestlength) // 相等时候看是否更加短
        {
            bestpath = curpath;
            bestlength = curlength;
            best_steps = cur_steps;
            //cout << "更新最优解：" << endl;
            //outputSolution();
            //cout << "\n继续 搜索中 ...\n";
			NoUpdataMaxItera = 5000;
        }
    }
}

// 每只蚂蚁搜之前初始
void init_ant()
{
    cur_num_mustV = 2; // 起点终点必经
    curpath.clear();
    curlength = 0;
    cur_steps = 0;
    mark.resize(numV+1);

    subSets[S]= rootSons-1;
    for (int i = 1; i <= numV; i++)
        mark[i] = subSets[i]+1; // 剩余可走次数
    /*for (int x : mark)
        cout << x << " ";
    cout << endl;*/

}

// 一批蚂蚁搜索之前初始化
inline void init_bath_search()
{
    tempInfo.clear();
    tempInfo.resize(numV + 1);
    for (int i = 1; i <= numV; i++)
    {
        tempInfo[i].resize(numV + 1, 0);
    }
}

int ant_search()
{
    int cur = S;
    while (true)
    {
        //cout << cur << endl;
        if(cur<=originalNumV)
            cur_steps++;

        if (cur_steps > limt_step) // 超出限制
            return 0;
        if (cur_steps*1.0 / limt_step > cur_num_mustV*1.5 / (num_mustV + 2))
                return 0;

        curpath.push_back(cur);   // 记录当前路径
        if (cur == E)             // 到达终点
        {
            updatabest();
            return 1;
        }
        if (mustV[cur]&&mark[cur]==subSets[cur]+1) // 若是必经点，增加必经点个数,经过就算一次
            cur_num_mustV++;
        mark[cur]--; // 可走次数减一


        double sums = 0;
        vector<double>prop;
        for (int j = head[cur]; j != -1; j = edge[j][1]) // 求出边信息素之和
        {
            int next = edge[j][0];
            if (mark[next]>0 && d[cur][next]<INF)  // 还有次数可走
            {
                double temp=pow(info[cur][next], alph)*pow(1.0 / (d[cur][next]+1.0), beta);  //由信息素和距离决定
                prop.push_back(temp);
                sums += temp;
            }
        }
        if (prop.size() == 0) // 无路可走
            return 0;


        /*for (auto x : prop)
        {
            cout << x << " ";
        }
        cout << endl;*/

        double go = getrand(myRandom);

        double tempprop = 0;
        int tempCnt = 0;
        for (int j = head[cur]; j != -1; j = edge[j][1])
        {
            int next = edge[j][0];
            if (mark[next]>0 && d[cur][next]<INF)
            {
                tempprop += prop[tempCnt++] / sums;
                if (tempprop >= go)
                {
                    curlength += d[cur][next]; //累计长度
                    cur = next;
                    break;
                }
            }
        }
    }
}


//更新局部信息素
void updataTempInfo()
{
    for (unsigned int i=1;i<curpath.size();i++)
    {
        int cur = curpath[i - 1];
        int next = curpath[i];
        tempInfo[cur][next] += 1.0 / curlength + cur_num_mustV*1.0/(num_mustV+2); // 由路径短和走的必经点个数双方因素决定，+2是拉普拉斯平滑操作
    }

    if(num_mustV + 2==best_num_mustV) // 精英蚁优化
        for (unsigned int i = 1; i < bestpath.size(); i++)
        {
            int cur = bestpath[i - 1];
            int next = bestpath[i];
            tempInfo[cur][next] += All_info*1.0 / bestlength + cur_num_mustV*1.0 / (num_mustV + 2);
        }
}


void run_ants_search()
{
    init_search();
    for (int i = 0; i < Itera_time; i++)
    {
		NoUpdataMaxItera--;
        init_bath_search();
        for (int j = 0; j < Ant_num; j++)
        {
            init_ant();
            if (ant_search())   //到达目的地
            {
                updataTempInfo();
            }
            if ((clock() - g_start_t) / CLOCKS_PER_SEC * 1000 > g_period_t || NoUpdataMaxItera<0)
            {
                return ;
            }
        }
        updataInfo();
    }
}
void outputSolution()
{
    cout << endl;
    cout << "当前经过必经点和必经边总数： " << best_num_mustV-2 << endl;
    cout << "当前最优解总长度： " << bestlength << endl;
    cout << "总共经过点数次数（包括重复点）： " << best_steps << endl;
    cout << "路径如下：" << endl;

    int  haveGoV = 0;
    int  haveGoE = 0;
    for (unsigned int i = 0; i < bestpath.size(); i++)
    {
        int v  =bestpath[i];

        if (v > originalNumV)
        {
            cout << "(必经边)";
            haveGoE++;
            continue; // 如果是添加的点（必经边），输出下一个点
        }
        if (i!=0)
            cout << " --> " << v;
        else
            cout<<v;

        if (mustV[v])
        {
            cout << "(必经点)";
            haveGoV++;
        }
    }

    cout << endl;
    cout << "经过指定必经点： " << haveGoV << " 个（不包括起点终点）(总共指定" << num_mustV - numV + originalNumV << "个)" << endl;
    cout << "经过指定必经边： " << haveGoE << "条 (总共指定" << numV - originalNumV << "条)" << endl;
}

void outputSolution2file(string fname)
{
    ofstream myout(fname);
    myout << "当前经过必经点和必经边总数： " << best_num_mustV - 2 << endl;
    myout << "当前最优路径长度(权重)： " << bestlength << endl;
    myout << "总共经过点数次数（包括重复点）： " << best_steps << endl;
    myout << "路径如下：" << endl;

    int  haveGoV = 0;
    int  haveGoE = 0;
    for (unsigned int i = 0; i < bestpath.size(); i++)
    {
        int v = bestpath[i];

        if (v > originalNumV)
        {
            myout << "(必经边)";
            haveGoE++;
            continue; // 如果是添加的点（必经边），输出下一个点
        }
        if (i != 0)
            myout << " --> " << v;
        else
            myout << v;

        if (mustV[v])
        {
            myout << "(必经点)";
            haveGoV++;
        }
    }

    myout << endl;
    myout << "经过指定必经点： " << haveGoV << " 个（不包括起点终点）(总共指定"<<num_mustV-numV+originalNumV <<"个)"<< endl;
    myout << "经过指定必经边： " << haveGoE << "条 (总共指定"<<numV-originalNumV<<"条)"<< endl;
}


int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout << "请输入正确的参数，示例:\n \t ./antSearch <TOPO_FILE> <SECONDS>" << endl;
        return -1;
    }

    string resultOutput = "output.txt";
    string dataInput(argv[1]);      // 读入输入文件
    int seconds = atof(argv[2]);    // 读入最长运行时间

    g_start_t = clock();
    g_period_t = seconds * 1000;    // ms

    // 1. 初始化并构造模型
    init_graph();
    read_and_rebuild(dataInput);

    // 2. tarjan算法求解割点以及连通块
    init_tarjan();
    tarjian(S);

    // 3. 智能蚁群搜索
    run_ants_search();

    outputSolution2file(resultOutput);

    //system("pause");
    return 0;
}
