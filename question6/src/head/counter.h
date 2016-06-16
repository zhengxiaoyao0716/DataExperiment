#ifndef __COUNTER_H
#define __COUNTER_H

#include <stdio.h>


typedef struct coWoMapNode {
    int count;
    char* word;
    struct coWoMapNode* next;
} CountWordsMap;

typedef struct {
    char* files;
    CountWordsMap* maps;
    int mapsDeep;
} Report;
extern Report report;

/** 初始计数 */
void CounterInit();
/** 统计文件 */
bool CountWordInFile();
/** 打印报告 */
void PrintReport();
/** 导出报告 */
void ExportReport();
/** 搜索单词出现次数 */
void SearchWordCount();
/**
 * 选择文件
 * <p>只要用户做出选择（不直接回车），该函数就会尝试载入并返回文件路径，文件载入结果存入streamAddr。</p>
 * @return 用户选择的文件路径（由alloc系列分配，请注意管理内存。NULL表示直接回车）
 */
char* ChooseFile(FILE** streamAddr, bool writeMode);
/** 载入统计 */
bool LoadResult(FILE* stream);
/** 保存结果 */
bool SaveResult(FILE* stream);

#endif