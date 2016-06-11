#ifndef __QUEUE_H
#define __QUEUE_H

typedef char QueueData;

/**
 * 队列（当前实现为循环队列）
 */
typedef struct {
    QueueData *front, *rear;
    int size, len, end;
} Queue;

/**
 * 队列创建.
 * @param 队列
 * @return 成败
 */
bool InitQueue(Queue* queue);

/**
 * 队列迭代.
 * @param queue 队列
 * @param action 迭代每项执行的动作
 */
void EachInQueue(Queue* queue, void (* action)(QueueData* ch));

/**
 * 队列清空.
 * @param queue 队列
 * @return 成败
 */
bool Clearqueue(Queue* queue);
/**
 * 队列判空.
 * @param 队列
 * @return 成败
 */
bool Emptyqueue(Queue* queue);

/**
 * 入队.
 * @param 队列
 * @return 成败
 */
bool Enqueue(Queue* queue, QueueData ch);
/**
 * 出队.
 * @param 队列
 * @return 出队元素的值
 */
QueueData Dequeue(Queue* queue);

#endif