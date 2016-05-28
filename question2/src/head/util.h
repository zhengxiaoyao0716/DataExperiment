#ifndef util
#define util

void Finish();

typedef struct node {
    char data[8];
    struct node* next;
} LinkStack;

#endif