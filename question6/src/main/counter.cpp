#include "counter.h"

#include <stdlib.h>
#include <string.h>

#include "wordUtil.h"
#include "hashMap.h"
#include "util.h"
#include "hashMap2Csv.h"


#define HIGH_FREQ_SIZE 10

bool insertIntoHighFreqMaps(int count, char* word) {
    CountWordsMap* cursor = report.maps;
    CountWordsMap* parent = NULL;
    while (cursor != NULL and (*cursor).count <= count) {
        if (strcmp((*cursor).word, word) == 0) {
            CountWordsMap* next = cursor -> next;
            if (parent == NULL) report.maps = next;
            else parent -> next = next;
            free(cursor);
            cursor = next;
            report.mapsDeep--;
        }
        else {
            parent = cursor;
            cursor = cursor -> next;
        }
    }
    if (parent == NULL) {
        if (report.mapsDeep < HIGH_FREQ_SIZE) {
            CountWordsMap* node = (CountWordsMap*)malloc(sizeof(CountWordsMap));
            if (node == NULL) Finish("单词频率计数节点分配失败，可能内存不足\n");
            (*node).count = count;
            (*node).word = word;
            node -> next = cursor;
            report.maps = node;
            report.mapsDeep++;
            return true;
        }
        else return false;
    }

    CountWordsMap* node = (CountWordsMap*)malloc(sizeof(CountWordsMap));
    if (node == NULL) Finish("单词频率计数节点分配失败，可能内存不足\n");
    (*node).count = count;
    (*node).word = word;
    node -> next = cursor;
    parent -> next = node;
    if (report.mapsDeep >= HIGH_FREQ_SIZE) {
        CountWordsMap* head = report.maps;
        report.maps = head -> next;
        free(head);
    }
    else report.mapsDeep++;
    return true;
}
/* 遍历统计到的高频单词 */
void listHighFreqMaps(void (* action)(int count, char* word)) {
    CountWordsMap* cursor = report.maps;
    while (cursor != NULL) {
        if ((*cursor).count > 0) action((*cursor).count, (*cursor).word);
        cursor = cursor -> next;
    }
}
FILE* printHighFreqMapsStream = stdout;
void printHighFreqMapsAction(int count, char* word) {
    fprintf(printHighFreqMapsStream, "单词：%s,  出现次数%d.\n", word, count);
}

/** 取巧优化，只有在通过wordUtil的生成器生成单词的一瞬起作用 */
int countWordHashCode(void* key) {
    return wordUtil.hash;
}
/** 取巧优化，只有在通过wordUtil的生成器生成单词的一瞬起作用 */
int normalWordHashCode(void* key) {
    int hash = 0;
    char* cursor = (char*)key;
    char ch;
    while ((ch = *cursor) != 0) {
        int code = 0;
        /* 原ascii： 0: 48, 9: 57, A: 65, Z: 90, _: 95, a: 97, z: 122 */
        /* 归并到原点连续集合：[1~10],[11~36],[37],[38~63] */
        if (ch >= '0' and ch <= '9') code = ch - 47;        //ch - 48 + 1
        else if (ch >= 'A' and ch <= 'Z') code = ch - 54;   //ch - 65 + 1 + 10
        else if (ch == '_') code = 37;                      //95 - 95 + 1 + 36
        else if (ch >= 'a' and ch <= 'z') code = ch - 59;   //ch - 97 + 1 + 37
        else Finish("出现预期外的字符\n");
        hash = (hash << 5) - hash + code;
        cursor++;
    }
    return hash;
}

void countWordListener(char* word, int hash) {
    int* value = (int*)malloc(sizeof(int));
    *value = 1;
    int count = *((int*)hashMap.put(word, value));
    insertIntoHighFreqMaps(count, word);
    if (hashMap.throwKey != NULL) free((char*)hashMap.throwKey);
}
void* valueAddUpdate(void* oldVal, void* newVal) {
    *((int*)newVal) += *((int*)oldVal);
    free(oldVal);
    // oldVal = NULL;
    return newVal;
}


/* 此处很容易优化，内存换时间，不过懒得弄了。调用频率很低的，我的时间最重要(吐舌)*/
char* appendFilesStr(char** filesAddr, char* newFilePath) {
    int oldLen = strlen(*filesAddr);
    int expandLen = strlen(newFilePath);
    char* newStr = (char*)realloc(*filesAddr, (oldLen + expandLen + 4) * sizeof(char));
    if (newStr == NULL) Finish("扩展失败，内存不足\n");
    *filesAddr = newStr;
    *(*filesAddr + oldLen) = '<';
    *(*filesAddr + oldLen + 1) = 0;
    strcat(*filesAddr, newFilePath);
    *(*filesAddr + oldLen + expandLen + 1) = '>';
    *(*filesAddr + oldLen + expandLen + 2) = '\n';
    *(*filesAddr + oldLen + expandLen + 3) = 0;
    return *filesAddr;
}


Report report = {
    (char*)calloc(1, sizeof(char)),
    (CountWordsMap*)calloc(1, sizeof(CountWordsMap)),
    0
};
/** 初始计数 */
void CounterInit() {
    wordUtil.wordListener = countWordListener;
    hashMap.table.keyInterface.hashCode = countWordHashCode;
    hashMap.table.innerUtil.hashCode1Resize = normalWordHashCode;
    hashMap.table.valInterface.update = valueAddUpdate;

    if (report.mapsDeep > 0) {
        free(report.files);
        CountWordsMap* cursor = report.maps;
        while (cursor != NULL) {
            CountWordsMap* tmpNode = cursor;
            cursor = cursor -> next;
            free(tmpNode);
        }
        report.files = (char*)malloc(8 * sizeof(char));
        *(report.files) = 0;
        report.maps = (CountWordsMap*)calloc(1, sizeof(CountWordsMap));
        report.mapsDeep = 0;
    }
    char empty[] = "";
    report.maps -> word = empty;
}

/** 统计文件 */
bool CountWordInFile() {
    printf("请输入要统计的文件的路径：\n");
    FILE* stream;
    char *filePath = ChooseFile(&stream, false);
    if (filePath == NULL) {
        printf("无效的输入\n");
        return false;
    }
    if (stream == NULL) {
        printf("无效的文件路径（或文件不可读）\n");
        free(filePath);
        return false;
    }
    printf("[正在统计文件%s]\n", filePath);
    appendFilesStr(&report.files, filePath);
    free(filePath);

    //开始统计
    if (report.mapsDeep == 0) report.mapsDeep = 1;
    char ch;
    while((ch = fgetc(stream)) != EOF) {
        wordUtil.wordGenerator(ch);
    }
    wordUtil.wordGenerator(0);

    //打印报告
    PrintReport();
    fflush(stream);        
    fclose(stream);
    return true;
}

/** 打印报告 */
void PrintReport() {
    if (report.mapsDeep <= 0) {
        printf("当前没有已统计的文件\n");
        return;
    }
    printf("已统计的文件列表：\n%s\n", report.files);
    listHighFreqMaps(printHighFreqMapsAction);
}

/** 导出报告 */
void ExportReport() {
    if (report.mapsDeep <= 0) {
        printf("当前没有已统计的文件\n");
        return;
    }
    printf("请输入文件导出路径(e.g: export.txt )：\n");
    printf("（统计结果将以txt格式存入，建议文件以.txt 为后缀）\n");
    FILE* stream;
    char *filePath = ChooseFile(&stream, true);
    if (filePath == NULL) return;
    if (stream == NULL) {
        printf("导出失败：无效的文件路径（或文件不可写）\n");
        free(filePath);
        return;
    }
    printHighFreqMapsStream = stream;
    fprintf(stream, "已统计的文件列表：\n%s\n", report.files);
    listHighFreqMaps(printHighFreqMapsAction);
    fflush(stream);
    fclose(stream);
    printHighFreqMapsStream = stdout;
    printf("当前统计报告已成功导出到%s\n", filePath);
    free(filePath);
}

// void searchWordListener(char* word, int hash) {
//     int* result = (int*)hashMap.get(word, NULL);
//     printf("单词：%s,  出现次数%d.\n", word, result == NULL ? 0 : *result);
//     free(word);
// }
/** 搜索单词出现次数 */
void SearchWordCount() {
    // wordUtil.wordListener = searchWordListener;
    wordUtil.wordListener = NULL;
    printf("请输入要搜索的单词（可多选），回车结束：\n");
    printf("（单词定义为0-9、A-Z、_、a-z等字符的任意组合）\n");
    printf("（除回车表示换行以外，其它均视为分隔符）\n");

    int* zero = (int*)calloc(1, sizeof(int));
    char ch;
    do {
        ch = getc(stdin);
        if (wordUtil.wordGenerator(ch)) {
            char* word = wordUtil.word;
            int count = *((int*)hashMap.get(word, zero));
            printf("单词：%s,  出现次数%d.\n", word, count);
            free(word);
        };
    } while(ch != '\n');
    free(zero);

    wordUtil.wordListener = countWordListener;
}

/**
 * 选择文件
 * <p>只要用户做出选择（不直接回车），该函数就会尝试载入并返回文件路径，文件载入结果存入streamAddr。</p>
 * @return 用户选择的文件路径（由alloc系列分配，请注意管理内存。NULL表示直接回车）
 */
char* ChooseFile(FILE** streamAddr, bool writeMode) {
    int index = 0, initSize = 8;
    char* filePath = (char*)malloc(initSize * sizeof(char));
    if (filePath == NULL) Finish("文件路径字符串存放空间分配失败，可能内存不足\n");

    while ((*(filePath + index) = getchar()) != '\n') {
        if (++index >= initSize) {
            initSize <<= 1;
            char* newFilePath = (char*)realloc(filePath, initSize * sizeof(char));
            if (newFilePath == NULL) Finish("文件路径字符串存放空间分配失败，可能内存不足\n");
            filePath = newFilePath;
        }
    }

    *(filePath + index) = 0;
    fflush(stdin);

    if (*filePath == 0) {
        free(filePath);
        return NULL;
    }

    if (writeMode) {
        //是否覆盖
        FILE* tempFileStream = fopen(filePath, "r");
        if (tempFileStream != NULL) {
            fclose(tempFileStream);
            printf("【！警告】文件已存在，如果要强制覆盖请输入Y（此操作不可逆！）：\n");
            char sure = getchar();
            fflush(stdin);
            if (sure != 'y' && sure != 'Y') {
                printf("已根据用户选择放弃覆盖原文件\n");
                *streamAddr = NULL;
                return filePath;
            }
        }
        *streamAddr = fopen(filePath, "w");
    }
    else *streamAddr = fopen(filePath, "r");

    return filePath;
}

/** 载入统计 */
bool LoadResult(FILE* stream) {
    char* column1, * column2;
    LoadTitleFromFile(stream, &column1, &column2);
    free(column1);
    free(column2);
    char* key;
    int* value;

    hashMap.table.keyInterface.hashCode = hashMap.table.innerUtil.hashCode1Resize;
    while (LoadLineFromFile(stream, &key, &value)) {
        hashMap.put(key, value);
        insertIntoHighFreqMaps(*value, key);
        // if (hashMap.throwKey != NULL) free(hashMap.throwKey);
    }
    hashMap.table.keyInterface.hashCode = countWordHashCode;
    return true;
}

FILE* writeLineToCsvActionStream;
void writeLineToCsvAction(void* key, void* value) {
    WriteLineToFile(writeLineToCsvActionStream, (char*)key, *((int*)value));
}
/** 保存结果 */
bool SaveResult(FILE* stream) {
    char column1[] = "单词";
    char column2[] = "次数";
    WriteTitleToFile(stream, column1, column2);

    writeLineToCsvActionStream = stream;
    hashMap.list(writeLineToCsvAction);

    return true;
}