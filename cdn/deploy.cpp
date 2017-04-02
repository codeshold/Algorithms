//
// Created by Wenfeng Shi on 17/4/2.
//

#include "deploy.h"
#include "minicost.h"
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <iostream>

using namespace std;

void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{

    int vNum, linkNum, consumerNum;
    int srvCost = 0;
    int start, end, cap, cost;
    int netnode, need;

    char *c;
    int spaceCount = 0;
    int i;

    string res;

    // 读取顶点数、链路数、消费节点数
    c = topo[0];
    spaceCount = 0;
    vNum = linkNum = consumerNum = 0;
    while (*c != '\0' && *c != '\n' && *c != '\r')
    {
        if (*c == ' ')
        {
            c++;
            spaceCount++;
            continue;
        }
        if (spaceCount == 0)
        {
            vNum = *c - '0' + vNum * 10;
        }
        if (spaceCount == 1)
        {
            linkNum = *c - '0' + linkNum * 10;
        }
        if (spaceCount == 2)
        {
            consumerNum = *c - '0' + consumerNum * 10;
        }
        c++;
    }

    // 读取服务器成本
    c = topo[2];
    spaceCount = 0;
    while (*c != '\0' && *c != '\n' && *c != '\r')
    {
        if (*c == ' ')
        {
            c++;
            spaceCount++;
            continue;
        }
        if (spaceCount == 0)
        {
            srvCost = *c - '0' + srvCost * 10;
        }
        c++;
    }
    set_num(vNum, linkNum, consumerNum, srvCost);

    // 读取链路信息
    for( i = 4; i < 4 + linkNum; i++)
    {
        c = topo[i];
        spaceCount = 0;
        start = end = cap = cost = 0;
        while (*c != '\0' && *c != '\n' && *c != '\r')
        {
            if (*c == ' ')
            {
                c++;
                spaceCount++;
                continue;
            }
            if (spaceCount == 0)
            {
                start = *c - '0' + start * 10;
            }
            if (spaceCount == 1)
            {
                end = *c - '0' + end * 10;
            }
            if (spaceCount == 2)
            {
                cap = *c - '0' + cap * 10;
            }
            if (spaceCount == 3)
            {
                cost = *c - '0' + cost * 10;
            }
            c++;
        }
        add_edge(start, end, cost, cap);
        add_edge(end, start, cost, cap);
    }


    // 读取消费节点信息
    for (int i = 1; i < consumerNum+1; i++)
    {
        c = topo[line_num-i];
        netnode = need = spaceCount = 0;
        while (*c != '\0' && *c != '\n' && *c != '\r')
        {
            if (*c == ' ')
            {
                c++;
                spaceCount++;
                continue;
            }
            if (spaceCount == 1)
            {
                netnode = *c - '0' + netnode * 10;
            }
            else if (spaceCount == 2)
            {
                need = *c - '0' + need * 10;
            }
            c++;
        }

        set_consume(consumerNum-i, netnode, need);

    }

    // 输出结果
    char * topo_file = cdn_minicost();
    write_result(topo_file, filename);
}
