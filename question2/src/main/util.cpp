#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include "conio.h"


/* 
 * ================================
 * 栈工具 
 * ================================
 */

//内部实现的栈（当前为链栈）
typedef struct stackNode {
    ExprUnit data;
    struct stackNode* next;
} linkStack;
linkStack* stackHead;

bool init() {
    if (stackHead != NULL) return false;
    
    stackHead = (linkStack*)malloc(sizeof(linkStack));
    if (stackHead == NULL) {
        printf("栈首节点分配失败，可能内存已满\n");
        Finish();
    }
    stackHead -> next = NULL;
    return true;
}
bool push(ExprUnit data) {
    if (stackHead == NULL) return false;
    
    linkStack* node = (linkStack*)malloc(sizeof(linkStack));
    if (node == NULL) {
        printf("栈节点分配失败，可能内存已满\n");
        Finish();
    }
    (*node).data = data;
    node -> next = stackHead -> next;
    stackHead -> next = node;
    
    return true;
}
ExprUnit top() {
    ExprUnit data;
    if (stackHead -> next == NULL) {
        data.value.op = '#';
        data.isNum = false;
    }
    else data = (*(stackHead -> next)).data;
    return data;
}
ExprUnit pop() {
    linkStack* node = stackHead -> next;
    ExprUnit data;
    if (node != NULL) {
        data = (*node).data;
        stackHead -> next = node -> next;
        free(node);
    }
    else {
        data.value.op = '#';
        data.isNum = false;
    }
    return data;
}
bool release() {
    if (stackHead == NULL) return false;
    
    while (stackHead -> next != NULL) {
        linkStack* node = stackHead -> next;
        stackHead -> next = node -> next;
        free(node);
    }
    free(stackHead);
    stackHead = NULL;
    return true;
}

Stack stack = {
    init, push, top, pop, release
};


/**
 * 结束应用.
 */
void Finish() {
    release();
    printf("任意键退出");
    fflush(stdin);
    getch();
    exit(-1);
}
