//
// Created by Wenfeng Shi on 17/4/2.
//

#ifndef HUAWEI_MINICOST_H
#define HUAWEI_MINICOST_H

#define EXIT_SECOND 85
void add_edge(int from, int to, int cost, int res);

char *cdn_minicost(void);

void set_num(int vNum, int linkNum, int consumerNum, int srvcost);
void set_consume(int index, int netnode, int need);

#define INLINE  static __inline
#ifdef _DEBUG
#define PRINT   printf
#else
#define PRINT(...)
#endif

#endif //HUAWEI_MINICOST_H
