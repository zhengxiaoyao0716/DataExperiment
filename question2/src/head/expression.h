#ifndef __EXPRESSION_H
#define __EXPRESSION_H

/**
 * 表达式数据单元，{'#', false} 表示空.
 */
typedef struct {
    union {
        float num;
        char op;
    } value;
    bool isNum;
} ExprUnit;

/**
 * 表达式分析结果.
 */
typedef struct {
    bool avalid;
    const char *message;
} ExprAnaly;

/**
 * 中缀表达式转后缀.
 * @param expr 中缀表达式
 * @param postExpr 后缀表达式
 * @return 是否成功
 */
ExprAnaly MidPost(char* expr, ExprUnit* postExpr);

/**
 * 对后缀表达式求值.
 * @param postExpr 后缀表达式
 * @return 求得的结果
 */
float PostCount(ExprUnit* postExpr);

#endif