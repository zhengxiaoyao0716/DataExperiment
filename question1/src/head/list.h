#ifndef list
#define list

typedef int datatype;

typedef struct node {
    datatype value;
    struct node* next;
} List;

bool CreateList(List* h);
void Adjmax(List* h, int k, int* result);
bool FreeList(List* h);

#endif