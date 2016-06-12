#ifndef __LIST_H
#define __LIST_H

typedef int datatype;

typedef struct node {
    datatype value;
    struct node* next;
} List;

bool CreateList(List* h);
void Adjmax(List* h, int k, int* result);
bool FreeList(List* h);

#endif