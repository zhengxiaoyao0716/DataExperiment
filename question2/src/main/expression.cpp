#include "expression.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"


float addInt(float now, int num) {return now * 10 + num;}
float decimal;
float addDec(float now, int num) {
    decimal *= 0.1;
    return now + decimal * num;
}
/*
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
*/

enum charFlag {EXPR_NUM, EXPR_OP, EXPR_PH};
bool flagAllow[3];
int phCount;    //括号计数
ExprUnit unit;
float (* addNum)(float, int);
ExprAnaly reportError(const char *message) {
    ExprAnaly analy = {false, message};
    return analy;
}
ExprUnit* postCursor;
ExprAnaly parseExpr(char ch) {
    switch (ch) {
        case ' ':
            // if (!flagAllow[EXPR_SPC])
            //     return reportError("[error]错误的表达式.");
            if (unit.isNum) {
                *postCursor++ = unit;
                unit.isNum = false;

                flagAllow[EXPR_NUM] = false;    //禁止空格两端同为数字
            }
            //keep flagAllow
            break;
        case '+':
        case '-':
            if (!flagAllow[EXPR_OP])
                return reportError("[error]运算符左侧应该为数字.");
            if (unit.isNum) {
                *postCursor++ = unit;
                unit.isNum = false;
            }
            unit = stack.top();
            if (unit.value.op != '(' && unit.value.op != '#')
                *postCursor++ = stack.pop();
            unit.value.op = ch;
            unit.isNum = false;
            stack.push(unit);
            
            flagAllow[EXPR_NUM] = true;
            flagAllow[EXPR_OP] = false;
            flagAllow[EXPR_PH] = false;
            break;
        case '*':
        case '/':
            if (!flagAllow[EXPR_OP])
                return reportError("[error]运算符左侧应该为数字.");
            if (unit.isNum) {
                *postCursor++ = unit;
                unit.isNum = false;
            }
            unit = stack.top();
            switch (unit.value.op) {
                case '*':
                case '/':
                    *postCursor++ = stack.pop();
                    break;
                default:
                    break;
            }
            unit.value.op = ch;
            unit.isNum = false;
            stack.push(unit);
            
            flagAllow[EXPR_NUM] = true;
            flagAllow[EXPR_OP] = false;
            flagAllow[EXPR_PH] = false;
            break;
        case '(':
            // if (!flagAllow[EXPR_LPH])
            //     return reportError("[error]错误的表达式.");
            if (unit.isNum) {
                *postCursor++ = unit;
                unit.value.op = ')';
                unit.isNum = false;
            }
            if (unit.value.op == ')') {
                unit = stack.top();
                switch (unit.value.op) {
                    case '(':
                    case '*':
                    case '/':
                        *postCursor++ = stack.pop();
                        break;
                    default:
                        break;
                }
                unit.value.op = '*';
                unit.isNum = false;
                stack.push(unit);
            }
            unit.value.op = ch;
            unit.isNum = false;
            stack.push(unit);
            
            phCount++;
            
            flagAllow[EXPR_NUM] = true;
            flagAllow[EXPR_OP] = false;
            flagAllow[EXPR_PH] = false;
            break;
        case ')':
            if (!flagAllow[EXPR_PH] || phCount <= 0)
                return reportError("[error]错误的表达式，右括号不匹配.");
            if (unit.isNum) {
                *postCursor++ = unit;
                unit.isNum = false;
            }
            unit = stack.top();
            while (unit.value.op != '(') {
                *postCursor++ = stack.pop();
                unit = stack.top();
            }
            unit.value.op = ch;
            unit.isNum = false;
            
            phCount--;
            
            flagAllow[EXPR_NUM] = false;
            flagAllow[EXPR_OP] = true;
            flagAllow[EXPR_PH] = true;
            break;
        case '.':
            if (!flagAllow[EXPR_NUM])
                return reportError("[error]错误的表达式，数字或右括号右侧不能连接数字.");
            if (!unit.isNum) {
                unit.value.num = 0.0;
                unit.isNum = true;
            }
            // assertTrue(unit.isNum);
            addNum = addDec;
            
            flagAllow[EXPR_NUM] = true;
            flagAllow[EXPR_OP] = true;
            flagAllow[EXPR_PH] = true;
            break;
        default:
            if (ch < '0' || ch > '9') {
                printf("未知的字符：%c\n", ch);
                return reportError("[error]错误的表达式，遇到未知字符");
            }
            if (!flagAllow[EXPR_NUM])
                return reportError("[error]错误的表达式，禁止空格两端同为数字.");
            if (!unit.isNum) {
                unit.value.num = 0.0;
                unit.isNum = true;
            }
            // assertTrue(unit.isNum);
            unit.value.num = addNum(unit.value.num, ch - '0');
            
            flagAllow[EXPR_NUM] = true;
            flagAllow[EXPR_OP] = true;
            flagAllow[EXPR_PH] = true;
            break;
    }
    ExprAnaly analy = {true, NULL};
    return analy;
}
/**
 * 中缀表达式转后缀.
 * @param expr 中缀表达式
 * @param postExpr 后缀表达式
 * @return 表达式是否有效
 */
ExprAnaly MidPost(char* expr, ExprUnit* postExpr) {
    ExprAnaly analy = {true, "表达式校验通过."};
    stack.init();
    decimal = 1.0;
    phCount = 0;
    addNum = addInt;
    flagAllow[0] = true;
    flagAllow[1] = true;
    flagAllow[2] = true;
    
    char* cursor = expr;
    postCursor = postExpr;
    while (*cursor != 0) {
        if (*cursor == '#') {
            //原表达式无效部分截断并释放
            *cursor = 0;
            while (*++cursor != 0) *cursor = NULL;
            *cursor = NULL;
            analy.message = "[warn]遇到截止符号，原表达式部分截断";
            break;
        }
        else {
            analy = parseExpr(*cursor++);
            if (!analy.avalid) break;
        }
    }
    parseExpr(' ');
    do {
        unit = stack.pop();
        if (unit.value.op != '(')
            *postCursor++ = unit;
    } while (unit.value.op != '#');
    
    stack.release();
    return analy;
}

/**
 * 对后缀表达式求值.
 * @param postExpr 后缀表达式
 * @return 求得的结果
 */
float PostCount(ExprUnit* postExpr) {
    stack.init();
    
    ExprUnit* cursor = postExpr;
    while ((*cursor).value.op != '#')  //(*cursor).isNum || 
    {
        if (!(*cursor).isNum) {
            float num2 = stack.pop().value.num;
            float num1 = stack.pop().value.num;
            ExprUnit unit = {{0.0}, true};
            switch ((*cursor).value.op) {
                case '+':
                    unit.value.num = num1 + num2;
                    break;
                case '-':
                    unit.value.num = num1 - num2;
                    break;
                case '*':
                    unit.value.num = num1 * num2;
                    break;
                case '/':
                    unit.value.num = num1 / num2;
                    break;
                default:
                    printf("发送未知错误\n");
                    Finish();
            }
            stack.push(unit);
        } 
        else stack.push(*(cursor));
        cursor++;
    }
    return stack.pop().value.num;
}