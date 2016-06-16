#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/**
 * 结束应用.
 * @param msg 提示信息.
 */
void Finish(const char* msg) {
    printf(msg);
    fflush(stdin);
    getchar();
    exit(-1);
}

/**
 * 模拟绘制文本
 * @param text 绘制的文本
 * @param occupy 占位字符
 * @param width 行宽度（不计算边框）
 */
void DrawCenterText(const char* text, char occupy, int width) {
    int index = 0;
    int textLen = strlen(text);
    int textLeft = (width - textLen) / 2;

    printf("#");
    while(index++ < width) {
        if (index == textLeft) {
            index += textLen;
            printf("%s", text);
        }
        else printf("%c", occupy);
    }
    printf("#\n");
}

void LOG_IN_FILE(char* msg) {
    FILE* stream = fopen("debug", "w");
    fprintf(stream, "Debug: %s", msg);
    fflush(stream);
    fclose(stream);
}
