#ifndef __UTILL_H
#define __UTILL_H


#define DEBUG(msg) printf("debug: %s", msg);getchar();fflush(stdin);
#define LOG(msg) LOG_IN_FILE(msg);

/**
 * 结束应用.
 * @param msg 提示信息.
 */
void Finish(const char* msg);

/**
 * 模拟绘制文本
 * @param text 绘制的文本
 * @param occupy 占位字符
 * @param width 行宽度（不计算边框）
 */
void DrawCenterText(const char* text, char occupy, int width);

void LOG_IN_FILE(char* msg);

#endif