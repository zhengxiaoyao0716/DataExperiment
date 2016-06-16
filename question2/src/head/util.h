#ifndef __UTILL_H
#define __UTILL_H

#include "expression.h"

void Finish();


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
    bool (* push)(ExprUnit data);
    
    /**
     * 读取栈顶.
     * @return 栈顶的数据，栈空时返回{'#', false}
     */
    ExprUnit (* top)();
    
    /**
     * 出栈.
     * @return 栈顶的数据，栈空时返回{'#', false}
     */
    ExprUnit (* pop)();
    
    /**
     * 释放.
     * @return 是否成功释放
     */
    bool (* release)();
} Stack;

/** 栈单例 */
extern Stack stack;

#endif