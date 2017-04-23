#include <stdio.h>
#include <stdlib.h>


#define MAX_DISTANCE 999

typedef struct Graph{
    int n;
    int *matrix;
} Graph;

typedef struct Result{
    int cost;
    int *path;
    int n;
} Result;

Result *Dijkstra(Graph *graph, int start, int end)
{
    int vertex;
    int *matrix;
    int *path;  // 保存当前最短路径的前驱节点
    int *dist;  // 保存当前节点到各节点的最短路径
    int *flag;  // 标记已在最短路径中的节点
    int nodes;  // 最短路径上的节点数
    int i, j, k, min;
    Result *presult; // 保存结果
    int *rpath; // 保存最短路劲(正序)
    int cost; // 最短路径cost
    int pre;


    /* 参数合法性检测 */

    vertex = graph->n;
    matrix = graph->matrix;
    path = (int *)malloc(vertex * sizeof(int));
    dist = (int *)malloc(vertex * sizeof(int));
    flag = (int *)malloc(vertex * sizeof(int));
    presult = (Result *)malloc(sizeof(Result));

    // 初始化path
    for(i = 0; i < vertex; i++) {
        path[i] = start;
        flag[i] = 0;
    }
    flag[start] = 1;
    nodes = 1;

    // 初始化dist
    for(i = 0; i < vertex; i++) {
        dist[i] = matrix[start*vertex + i];
    }

    printf("===%d\n", __LINE__);
    // 循环
    for(j = 1; j < vertex; j++) {
        // 找到dist当前最短路径的节点
        min = MAX_DISTANCE;
        for(i = 0; i < vertex; i++) {
            if((flag[i] == 0) && (min > dist[i])) {
                min = dist[i];
                k = i;
            }
        }

        // 更新flag
        flag[k] = 1;
        nodes++;
        if(k == end) {
            break;
        }

        // 更新dist
        for(i = 0; i < vertex; i++) {
            if((flag[i] == 0) && (dist[i] > dist[k] + matrix[k*vertex + i])) {
                dist[i] = dist[k] + matrix[k*vertex + i];
                path[i] = k; // 更新前驱节点
            }
        }
    }
    printf("===%d\n", __LINE__);

    printf("nodes: %d\n", nodes);
    printf("%d ", end);
    for(i = end; i != start; i = dist[i]) {
        printf("%d ", dist[i]);
        i = dist[i];
    }
    printf("===%d\n", __LINE__);


    // 保存结果
    rpath = (int *)malloc(nodes * sizeof(int));
    presult->path = rpath;
    presult->n = nodes;
    i = end;
    cost = 0;
    while(path[i] != start){
        pre = path[i];
        rpath[pre] = i;
        cost += matrix[pre*vertex + i];
    }
    rpath[start] = i;
    cost += matrix[start*vertex + i];
    presult->cost = cost;


    // free
    free(path);
    path = NULL;
    free(dist);
    dist = NULL;

    return presult;
}

int main(int argc, char *argv[])
{
    int i, j;
    Graph *p = (Graph *)malloc(sizeof(Graph));
    p->n = 5;
    p->matrix = (int *)malloc(5 * 5 * sizeof(int));
    for(i = 0; i < 5; i++) {
        for(j = 0; j < 5; j++)
            if(i == j) {
                (p->matrix)[i*5 + j] = 0;
            }
            else {
                (p->matrix)[i*5 + j] = MAX_DISTANCE;
            }
    }
    (p->matrix)[0*5 + 1] = 1;
    (p->matrix)[0*5 + 3] = 2;

    (p->matrix)[1*5 + 2] = 1;
    (p->matrix)[1*5 + 4] = 3;

    (p->matrix)[2*5 + 3] = 1;
    (p->matrix)[2*5 + 4] = 1;

    (p->matrix)[3*5 + 4] = 1;

    // 打印矩阵
    for(i = 0; i < 5; i++){
        for(j =0; j < 5; j++){
            printf("%d ", (p->matrix)[i*5 + j]);
        }
        printf("\n");
    }
    printf("\n");


    Result *r = Dijkstra(p, 0, 4);
    printf("path: ");
    for(i = 0; i < r->n; i++){
        printf("%d ", (r->path)[i]);
    }
    printf("\n");
    printf("cost: %d\n", r->cost);

    return 0;
}

