#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "queue.h"
#include "util.h"

#define EXPR_LEN 1024

void print_queue_unit(QueueData* ch) {
    printf("%c", *ch);
}

int main() {
    char flag = 'Y';
    Queue* queue = NULL;
    queue = (Queue*)malloc(sizeof(Queue));
    queue -> front = NULL;
    do {
        if (!InitQueue(queue)) {
            printf("初始化队列失败，队列非空或内存已满\n");
            Finish();
        }
        
        QueueData ch;
        do {
            printf("请输入字符，'0'表示出队，'@'表示依次出队并打印剩余元素\n");
            ch = getchar();
            fflush(stdin);
            
            switch (ch) {
                case '0':
                    printf("出队：%c\n", Dequeue(queue));
                    break;
                case '@':
                    printf("依次出队：\n");
                    EachInQueue(queue, print_queue_unit);
                    printf("\n出队完成.\n");
                    break;
                default:
                    if (Enqueue(queue, ch)) printf("入队：%c\n", ch);
                    else {
                        printf("入队失败，可能内存已满\n");
                        Finish();
                    }
            }
        } while(ch != '@');
        
        Clearqueue(queue);
        printf("输入N退出，否则继续\n");
        scanf("%c", &flag);
        fflush(stdin);
    } while(flag != 'N' && flag != 'n');
}