#include "queue.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

/*
 * ============循环队列内部结构============
 * (   0    1    2    3    4    5    6 ) *
 * |-> a -> b -> # -> # -> e -> f -> g | *
 *               .rear     .front        *
 * |<----------- size: 7 ------------->| *
 * ---------->|          |<- len: 5 ---  *
 *     .top                    bottom.   *
 *   end: 2 =>|          |<= begin: 4    *
 * =======================================
 */

/** 寻找原点指针 **/
QueueData* findTop(Queue* queue) {
    return queue -> rear - (*queue).end;
}
/** 计算队首位置 **/
int calculateBegin(Queue* queue) {
    return ((*queue).end + (*queue).size - (*queue).len) % (*queue).size;
}
// /** 寻找终点指针 **/
// Queue* findBottom(Queue* queue) {
//     return queue -> front + (*queue).size - 1 - (*queue).begin;
// }
// /** 计算队尾位置 **/
// int calculateEnd(Queue* queue) {
//     return ((*queue).begin + (*queue).len) % (*queue).size;
// }


/**
 * 队列创建.
 * @param 队列
 */
bool InitQueue(Queue* queue) {
    if (queue -> front != NULL) return false;
    
    queue -> front = (QueueData*)malloc(4 * sizeof(QueueData));
    queue -> rear = queue -> front;
    (*queue).size = 4;
    (*queue).len = 0;
    (*queue).end = 0;
    return true;
}

/**
 * 队列迭代.
 * @param queue 队列
 * @param action 操作队列中的元素的方法
 */
void EachInQueue(Queue* queue, void (* action)(QueueData* ch)) {
    QueueData* top = findTop(queue);
    QueueData* bottom = top + (*queue).size - 1;

    QueueData* cursor = queue -> front;
    while (cursor != queue -> rear) {
        if (action != NULL) action(cursor);
        if (cursor++ == bottom) cursor = top;
    }
}

/**
 * 队列清空.
 * @param queue 队列
 */
bool Clearqueue(Queue* queue) {
    if (queue -> front == NULL) return false;
    
    free(findTop(queue));
    queue -> front = NULL;
    return true;
}
/**
 * 队列判空.
 * @param 队列
 * @return 成败
 */
bool Emptyqueue(Queue* queue) {return (*queue).len == 0;}

/**
 * 入队.
 * @param 队列
 * @return 成败
 */
bool Enqueue(Queue* queue, QueueData ch) {
    QueueData* top = findTop(queue);
    QueueData* bottom = top + (*queue).size - 1;

    *(queue -> rear) = ch;
    if ((queue -> rear)++ >= bottom) {
        queue -> rear = top;
        (*queue).end = 0;
    }
    else (*queue).end++;
    if (++(*queue).len >= (*queue).size) {
        (*queue).size <<= 1;
        QueueData* newTop = (QueueData*)realloc(top, (*queue).size * sizeof(QueueData));
        if (newTop == NULL) return false;   //可以选择在这里释放内存
        else top = newTop;
        QueueData* cursor = queue -> front;
        if ((*queue).end < ((*queue).len >> 1)) {
            //移cursor左边
            while (cursor-- != top) {
                *(cursor + (*queue).len) = *cursor;
                *cursor = 0;
            }
            queue -> rear += (*queue).len;
            (*queue).end += (*queue).len;
        }
        else {
            //移cursor右边
            cursor--;
            while (cursor++ != bottom) {
                *(cursor + (*queue).len) = *cursor;
                *cursor = 0;
            }
            queue -> front += (*queue).len;
        }
    }
    return true;
}
/**
 * 出队.
 * @param 队列
 * @return 出队元素的值，0表示队列以空
 */
QueueData Dequeue(Queue* queue) {
    if ((*queue).len <= 0) return '0';
    else (*queue).len--;
    
    QueueData* top = findTop(queue);
    QueueData* bottom = top + (*queue).size - 1;

    QueueData data = *(queue -> front);
    *(queue -> front) = NULL;
    if ((queue -> front)++ == bottom) queue -> front = top;
    // // 需要还原分配以优化内存？
    // if ((queue -> front)++ == bottom) {
    //     DestroyQueue(queue);
    //     CreateQueue(queue);
    // }
    return data;
}
