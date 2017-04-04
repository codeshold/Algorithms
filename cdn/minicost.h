//
// Created by Wenfeng Shi on 17/4/2.
//

#ifndef __MINICOST_H__
#define __MINICOST_H__

#define EXIT_SECOND 75
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

#endif //__MINICOST_H__
