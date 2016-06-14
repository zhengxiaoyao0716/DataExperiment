#include "graphic.h"

#include <stdlib.h>
#include <string.h>

#include "util.h"


int locatevex(Vexnode *G, vtype u)  
{
    for(int i = 0; i < maxn; i++)  
        if(G[i].data == u) return i;  
    return -1;
}

/**
 * 建立有向图十字链表.
 * @param G 有向图十字链表
 * @return 顶点数
 * @see 教材P182页
 */
int Createorlist(Vexnode* G, FILE* stream) {
    int i, j, n;
    arcnode* p;
    vtype ch, u, v;
    i = n = 0;
    char op;
    do {
        fscanf(stream, "%c%c", &ch, &op);
        n++;
        G[i].data = ch;
        G[i].fin = G[i].fout = NULL;
        i++;
    } while (op != '\n');
    do {
        fscanf(stream, "%c%c%c", &u, &v, &op);
        i = locatevex(G, u);
        j = locatevex(G, v);
        p = (arcnode*)malloc(sizeof(arcnode));
        p -> tail = i;
        p -> head = j;
        p -> hlink = G[j].fin;
        G[j].fin = p;
        p -> tlink = G[j].fout;
        G[i].fout = p;
    } while (op != '\n');
    fflush(stream);
    return n;
}

void Creatid(Vexnode G[], int n, int* id) {
    int count, i;
    arcnode *p;
    for (i = 0; i < n; i++) {
        count = 0;
        p = G[i].fin;
        while (p) {
            count++;
            p = p -> hlink;
        }
        id[i] = count;
    }
}

void output(int j, vtype data) {
    // printf("?j=%d&data=%c\n", j, data);
}
/*
 * 拓扑排序
 * @param G 十字链表
 * @param n 当前G中的顶点数
 * @return 是否无环
 * @see 教材P203页
 */
bool Topsort(Vexnode* G, int n) {
    StackUtil stack = NewStackUtil();
    stack.init();
    int i, j, k, count, id[maxn];
    arcnode *p;
    Creatid(G, n, id);
    // Clearstack(s);
    for (i = 0; i < n; i++)
        if (id[i] == 0) stack.push(i);
    count = 0;
    while (!stack.isEmpty()) {
        j = stack.pop();
        output(j, G[j].data);
        count++;
        p = G[j].fout;
        while (p) {
            k = p -> head;
            id[k]--;
            if (id[k] == 0)
                stack.push(k);
            p = p -> tlink;
        }
    }
    stack.release();
    return count == n;
}