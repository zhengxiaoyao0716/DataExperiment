#ifndef __UTILL_H
#define __UTILL_H

typedef int StackData;

/**
 * 栈.
 */
typedef struct {
    /**
     * 初始化.
     * @return 是否成功初始化
     */
    bool (* init)();
    
    /**
     * 入栈.
     * @param data 入栈的数据
     * @return 是否成功入栈
     */
    bool (* push)(StackData data);
    
    /**
     * 读取栈顶.
     * @return 栈顶的数据，栈空时返回{'#', false}
     */
    StackData (* top)();
    
    /**
     * 出栈.
     * @return 栈顶的数据，栈空时返回{'#', false}
     */
    StackData (* pop)();
    
    /**
     * 释放.
     * @return 是否成功释放
     */
    bool (* release)();

    /**
     * 获取栈是否为空.
     */
    bool (*isEmpty)();
} Stack;

/** 栈单例 */
extern Stack stack;


void Finish();

#endif