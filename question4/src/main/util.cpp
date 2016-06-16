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
    Tree* element;
    bool visited;
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
bool push(Tree* element) {
    if (stackHead == NULL) return false;
    
    linkStack* node = (linkStack*)malloc(sizeof(linkStack));
    if (node == NULL) {
        printf("栈节点分配失败，可能内存已满\n");
        Finish();
    }
    (*node).element = element;
    (*node).visited = false;
    node -> next = stackHead -> next;
    stackHead -> next = node;
    
    return true;
}
Tree* top() {
    Tree* element = NULL;
    if (stackHead -> next != NULL) {
        element = (*(stackHead -> next)).element;
    }
    return element;
}
Tree* pop() {
    linkStack* node = stackHead -> next;
    Tree* element = NULL;
    if (node != NULL) {
        element = (*node).element;
        stackHead -> next = node -> next;
        free(node);
    }
    return element;
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

/**
 * 标记栈顶元素为已遍历.
 */
void markTopVisited() {
    if (stackHead -> next == NULL) {
        printf("严重错误，栈已空，不可标记栈顶元素为已遍历.");
        Finish();
    }
    (*(stackHead -> next)).visited = true;
}
/**
 * 获取栈顶元素是否为已遍历.
 */
bool isTopVisited() {
    if (stackHead -> next == NULL) {
        printf("严重错误，栈已空，不可获取栈顶元素是否已遍历.");
        Finish();
    }
    return (*(stackHead -> next)).visited;
}

Stack stack = {
    init, push, top, pop, release,
    isEmpty, markTopVisited, isTopVisited
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
