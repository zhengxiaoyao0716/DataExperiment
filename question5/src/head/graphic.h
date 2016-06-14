#ifndef __GRAPHIC_H
#define __GRAPHIC_H

#include <stdio.h>

#define maxn 64


typedef char vtype;
typedef struct Anode {
    int tail, head;
    struct Anode* hlink, * tlink;
} arcnode;
typedef struct Vnode {
    vtype data;
    arcnode *fin, *fout;
} Vexnode;

/**
 * 建立有向图十字链表.
 * @param G 有向图十字链表
 * @param stream 输入流
 * @return 顶点数
 * @see 教材P182页
 */
int Createorlist(Vexnode* G, FILE* stream);

/*
 * 拓扑排序
 * @param G 十字链表
 * @param n 当前G中的顶点数
 * @return 是否有环
 * @see 教材P203页
 */
bool Topsort(Vexnode* G, int n);

#endif