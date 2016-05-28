#include "list.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"


datatype str2int(char* str) {
    datatype result = 0;
    char* cursor = str;
    while (*cursor != 0) {
        result *= 10;
        datatype num = *cursor - '0';
        if (num < 0 || num > 9) {
            printf("无效的输入，%s 不是一个整数！\n", str);
            Finish();
        }
        else result += num;
        
        cursor++;
    }
    return result;
}

/**
 * 创建链表.
 * @param h 链表头指针
 * @return 是否成功
 */
bool CreateList(List* h) {
    printf("请输入整型数据，#结束：\n");
    int index = 0;
    List *cursor = h;
    while (true) {
        char inputStr[8];
        scanf("%s", inputStr);
        if (inputStr[0] == '#') {
            printf("输入结束.\n");
            fflush(stdin);
            break;
        }
        datatype num = str2int(inputStr);
        printf("第%d个数字：%d\n", ++index, num);
        List* node = (List*)malloc(sizeof(List));
        if (node == NULL) {
            printf("节点分配失败，可能内存已满\n");
            Finish();
            // return false;
        }
        node -> value = num;
        cursor -> next = node;
        cursor = node;
    }
    cursor -> next = NULL;
    return true;
}

/**
 * 求目标节点.
 * @param *h 链表头指针
 * @param k 纳入计算的相邻节点数
 * @param result 绑定计算结果
 */
void Adjmax(List* h, int k, int* result) {
    List* iterator = h -> next;
    List* kLeft  = h -> next;
    datatype num = 0, sum = 0, maxSum = 0;
    int no = 0;
    datatype data = 0;
    while(iterator != NULL) {
        if (num++ >= k) {
            if (sum > maxSum) {
                maxSum = sum;
                data = kLeft -> value;
            }
            sum += iterator -> value;
            sum -= kLeft -> value;
            no++;
            kLeft = kLeft -> next;
        }
        else sum += iterator -> value;
        
        iterator = iterator -> next;
    }
    if (sum > maxSum) {
        maxSum = sum;
        data = kLeft -> value;
    }
    
    result[0] = no;
    result[1] = data;
}

/**
 * 释放链表.
 * @param *h 链表头指针
 * @return 是否成功
 */
bool FreeList(List* h) {
    while(h -> next != NULL) {
        List* node = h -> next;
        h -> next = node -> next;
        free(node);
    }
    free(h);
    
    return true;
}