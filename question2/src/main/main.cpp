#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "expression.h"
// #include "util.h"

#define EXPR_LEN 1024


int main() {
    char flag = 'Y';
    do {
        //选择输入源
        printf("请输入要读取的文件的路径，若要从键盘输入请直接回车\n");
        printf("![文件路径最多允许255个字符]\n");
        char filePath[256];
        fgets(filePath, 256, stdin);
        fflush(stdin);
        FILE* source = stdin;
        if (filePath[0] != '\n') {
            filePath[strlen(filePath) - 1] = 0;
            source = fopen(filePath, "r");
            if (source == NULL) {
                printf("无效的文件路径\n");
                continue;
            }
            printf("[从文件%s 读取]\n", filePath);
        }
        else printf("[从键盘输入流读取]\n");
        printf("![表达式最多允许%d个字符]\n", EXPR_LEN);
        printf("![换行符'\\n'(回车)结束。'#'也可用作结束符，其后截断]\n");
        
        //读取表达式
        char expr[EXPR_LEN];
        fgets(expr, EXPR_LEN, source);
        fflush(source);
        expr[strlen(expr) - 1] = 0;
        
        //中缀转后缀
        ExprUnit postExpr[EXPR_LEN];
        ExprAnaly analy = MidPost(expr, postExpr);
        printf("%s\n", analy.message);
        if (!analy.avalid) continue;
        
        //计算表达式
        float result = PostCount(postExpr);
        
        printf("================================\n");
        printf("原表达式：\t%s\n", expr);
        printf("后缀表达式：\t");
        ExprUnit* cursor = postExpr;
        while ((*cursor).value.op != '#')  //(*cursor).isNum || 
            (*cursor).isNum ? printf("%f", (*cursor++).value.num) : printf("%c", (*cursor++).value.op);
        printf("\n计算结果：\t%f\n", result);
        printf("================================\n");
        // Finish();
        
        printf("输入N退出，否则继续\n");
        scanf("%c", &flag);
        fflush(stdin);
    } while(flag != 'N' && flag != 'n');
}