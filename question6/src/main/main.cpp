#include <stdio.h>
#include <stdlib.h>

#include "counter.h"
#include "util.h"


//'clc' for windows, 'clear' for linux
#define CLEAR_SCR       if (system("cls") != 0) system("clear");
#define DRAW_LINE(text) DrawCenterText(text, '#', 36);
#define DRAW_TEXT(text) DrawCenterText(text, ' ', 36);
#define WAIT_NEXT       printf("\n\t__请按下回车键继续\n"); getchar(); fflush(stdin);
#define MAKE_SURE       printf("\n\t__重要操作，请输入'Y'确认执行：\n"); char makeSure = getchar(); fflush(stdin); if (makeSure != 'y' && makeSure != 'Y') {printf("操作已取消\n"); break;}

int main() {
    CounterInit();
    while (true) {
        CLEAR_SCR
        
        DRAW_LINE("【功能菜单】")
        DRAW_TEXT("")
        DRAW_TEXT("1、[追加] 统计新的文件")
        DRAW_TEXT("2、[查看] 当前统计报告")
        DRAW_TEXT("3、[导出] 当前统计报告")
        DRAW_TEXT("4、[搜索] 单词出现次数")
        DRAW_TEXT("5、[载入] 或初始化统计")
        DRAW_TEXT("0、[保存] 统计以及结束")
        DRAW_TEXT("")
        DRAW_LINE("")
            
        char choose; scanf("%c", &choose); fflush(stdin);
        switch (choose) {
            case '1':
                CountWordInFile();
                break;
            case '2':
                PrintReport();
                break;
            case '3':
                ExportReport();
                break;
            case '4':
                SearchWordCount();
                break;
            case '5': 
            {
                printf("请输入备份所在路径(e.g: backup.csv )");
                printf("若要初始化请直接回车：\n");
                FILE* stream;
                char *filePath = ChooseFile(&stream, false);
                if (filePath == NULL) {
                    printf("确定要初始化？您将丢失所有未保存的数据！\n");
                    MAKE_SURE
                    CounterInit();
                    printf("初始化完成.\n");
                }
                else if (stream == NULL) printf("无效的路径（或文件不可读）\n");
                else {
                    printf("正在从%s 载入\n", filePath);
                    if (LoadResult(stream)) printf("从%s 载入完成.\n", filePath);
                    else printf("从%s 载入失败.\n", filePath);
                    free(filePath);
                    fclose(stream);
                }
                break;
            }
            case '0':
            {
                printf("请输入保存备份路径(e.g: backup.csv )");
                printf("若放弃保存请直接回车：\n");
                FILE* stream;
                char *filePath = ChooseFile(&stream, true);
                if (filePath == NULL) {
                    printf("已放弃保存\n");
                    printf("现在结束您将丢失所有数据，仍然要结束应用吗？\n");
                }
                else if (stream == NULL) {
                    printf("无效的路径（或文件不可读）\n");
                    break;
                }
                else {
                    printf("正在保存到%s\n", filePath);
                    if (!SaveResult(stream)) {
                        printf("保存到%s 失败\n", filePath);
                        fclose(stream);
                        free(filePath);
                        break;
                    }
                    else {
                        printf("保存到%s 成功\n", filePath);
                        fclose(stream);
                        printf("是否结束应用？\n");
                        free(filePath);
                    }
                }
                MAKE_SURE
                exit(0);
                break;
            }
            default:
                printf("未知的选项\n");
                break;
        }
        WAIT_NEXT
    }
}