#ifndef __UTIL_H
#define __UTIL_H

#include "tree.h"

void Finish();


/**
 * 栈工具类.
 */
typedef struct {
    /**
     * 初始化.
     * @return 是否成功初始化
     */
    bool (* init)();
    
    /**
     * 入栈.
     * @param data 入栈的元素
     * @return 是否成功入栈
     */
    bool (* push)(Tree* element);
    
    /**
     * 读取栈顶.
     * @return 栈顶的数据，栈空时返回{'#', false}
     */
    Tree* (* top)();
    
    /**
     * 出栈.
     * @return 栈顶的数据，栈空时返回{'#', false}
     */
    Tree* (* pop)();
    
    /**
     * 释放.
     * @return 是否成功释放
     */
    bool (* release)();

    /**
     * 获取栈是否为空.
     */
    bool (*isEmpty)();
    
    /**
     * 标记栈顶元素为已遍历.
     */
    void (* markTopVisited)();

    /**
     * 获取栈顶元素是否为已遍历.
     */
    bool (*isTopVisited)();
} StackUtil;

/**
 * 栈工具构造方法.
 */
StackUtil NewStackUtil();

#endif