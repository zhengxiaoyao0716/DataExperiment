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
    StackData data;
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
bool push(StackData data) {
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
StackData top() {
    StackData data = -1;
    if (stackHead -> next != NULL)
        data = (*(stackHead -> next)).data;
    return data;
}
StackData pop() {
    linkStack* node = stackHead -> next;
    StackData data = -1;
    if (node != NULL) {
        data = (*node).data;
        stackHead -> next = node -> next;
        free(node);
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

/** 判空 */
bool isEmpty() {return stackHead -> next == NULL;}

StackUtil NewStackUtil() {
    StackUtil stackUtil;
    
    stackUtil.init = init;
    stackUtil.push = push;
    stackUtil.top = top;
    stackUtil.pop = pop;
    stackUtil.release = release;
    stackUtil.isEmpty = isEmpty;
    
    return stackUtil;
}

/**
 * 结束应用.
 */
void Finish() {
    printf("任意键退出");
    fflush(stdin);
    getch();
    exit(-1);
}
