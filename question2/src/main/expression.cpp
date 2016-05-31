#include "expression.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"


float addInt(float now, int num) { return now * 10 + num; }
float decimal = 1.0;
float addDec(float now, int num) {
    decimal *= 0.1;
    return now + decimal * num;
}
float str2float(char* str) {
    float (* addNum)(float, int) = addInt;
    float result = 0.0;
    char* cursor = str;
    while (*cursor != 0) {
        if (*cursor != '.') {
            int num = *cursor - '0';
            if (num >= 0 || num <= 9) result = addNum(result, num);
            else {
                printf("错误，%s 不是一个有效的数字！\n", str);
                Finish();
            }
        }
        else addNum = addDec;
        
        cursor++;
    }
    return result;
}

enum exprFlag { EXPR_SPC, EXPR_NUM, EXPR_OP, EXPR_LPH, EXPR_RPH } flag;
int allowFlagMap[] = {-1, -1, 0, -1, 0};    //-1表示允许无限个
bool verifyExpr(char ch) {
    return true;
}
/**
 * 中缀表达式转后缀.
 * @param expr 中缀表达式
 * @param postExpr 后缀表达式
 * @return 是否成功
 */
bool MidPost(char* expr, ExprUnit* postExpr) {
    StackUtil stackUtil = NewStackUtil();
    stackUtil.init();
    
    char* cursor = expr;
    while (*cursor != 0) {
        if (*cursor == '\n' || *cursor == '#') {
            //原表达式无效部分截断并释放
            *cursor = 0;
            while (*++cursor != 0) *cursor = NULL;
            *cursor = NULL;
            break;
        }
        else {
            // TODO 转换逻辑
        }
        cursor++;
    }
    
    return true;
}

/**
 * 对后缀表达式求值.
 * @param postExpr 后缀表达式
 * @return 求得的结果
 */
float PostCount(ExprUnit* postExpr) {
    return false;
}