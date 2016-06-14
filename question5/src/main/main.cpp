#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "graphic.h"
// #include "util.h"


int main() {
    char flag = 'Y';
    do {
        //选择输入源
        printf("================================================================\n");
        printf("![定义数据格式：回车('\\n')为换行符，第一行输入顶点，第二行输入弧度]\n");
        printf("![\t分隔符可以是除了换行符('\\n')以外任意符号]\n");
        printf("![第一行顶点：两个顶点字符之间一个分隔符，末尾换行符结束]\n");
        printf("![第二行弧度：两个顶点字符连接作为一个弧度，弧度间需要一个分隔符，末尾换行符]\n");
        printf("================================================================\n");
        printf("请输入要读取的文件的路径，若要从键盘输入请直接回车\n");
        printf("![文件路径最多允许255个字符]\n");
        char filePath[256];
        fgets(filePath, 256, stdin);
        fflush(stdin);
        FILE* stream = stdin;
        if (filePath[0] != '\n') {
            filePath[strlen(filePath) - 1] = 0;
            stream = fopen(filePath, "r");
            if (stream == NULL) {
                printf("无效的文件路径\n");
                continue;
            }
            printf("[从文件%s 读取]\n", filePath);
        }
        else printf("[从键盘输入流读取]\n");
        
        Vexnode G[maxn];
        printf("该图%s有向无环图\n", (Topsort(G, Createorlist(G, stream))) ? "是" : "不是");

        if (stream != stdin) fclose(stream);
        // Finish();
        
        printf("输入N退出，否则继续\n");
        scanf("%c", &flag);
        fflush(stdin);
    } while(flag != 'N' && flag != 'n');
}