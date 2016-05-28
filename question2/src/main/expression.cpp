#include "expression.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"


float str2float(char* str) {
    int result = 0, index = 0;
    while (index < (int)strlen(str)) {
        result *= 10;
        int num = str[index++] - '0';
        if (num < 0 || num > 9) {
            printf("无效的输入，%s 不是一个整数！\n", str);
            Finish();
        }
        else result += num;
    }
    return result;
}

/**
 * 中缀表达式转后缀.
 * @param expr 中缀表达式
 * @param postExpr 后缀表达式
 * @return 是否成功
 */
bool MidPost(char* expr, char* postExpr) {
    char* cursor = expr;
    while (*cursor != 0 && *cursor != '\n' && *cursor != '#') {
        if (*cursor != ' ') {
            // TODO 转换逻辑
        }
        cursor++;
    }
    
    //原表达式无效部分截断并释放
    *cursor = 0;
    while (*++cursor != NULL) *cursor = NULL;
    
    return true;
}

/**
 * 对后缀表达式求值.
 * @param postExpr 后缀表达式
 * @return 求得的结果
 */
float PostCount(char* postExpr) {
    return false;
}