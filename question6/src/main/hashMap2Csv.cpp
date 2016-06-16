#include "hashMap2Csv.h"

#include <stdlib.h>

#include "hashMap.h"
#include "util.h"


void WriteTitleToFile(FILE* stream, char* column1, char* column2) {
    fprintf(stream, "\"%s\",\"%s\"\n", column1, column2);
}
void WriteLineToFile(FILE* stream, char* key, int value) {
    fprintf(stream, "\"%s\",\"%d\"\n", key, value);
}
bool LoadTitleFromFile(FILE* stream, char** column1, char** column2) {
    // fscanf(stream, "\"%s\",\"%s\"", *column1, *column2);
    int index = 0, length = 8;
    char* strBuilder = (char*)malloc(length * sizeof(char));
    char ch;
    while((ch = fgetc(stream)) != EOF) {
        if (ch == ',') {
            *(strBuilder + index) = 0;
            *column1 = strBuilder;
            index = 0;
            length = 8;
            strBuilder = (char*)malloc(length * sizeof(char));
            continue;
        }
        else if (ch == '\n') {
            *(strBuilder + index) = 0;
            *column2 = strBuilder;
            return true;
        }
        else if (ch == '\"') continue;

        *(strBuilder + index) = ch;
        if (++index >= length) {
            length <<= 1;
            char* newStr = (char*)realloc(strBuilder, length * sizeof(char));
            if (newStr == NULL) Finish("加载备份时分配字符串内存失败，可能内存不足\n");
            strBuilder = newStr;
        }
    }
    free(strBuilder);
    return false;
}
bool LoadLineFromFile(FILE* stream,  char** key, int** value) {
    // fscanf(stream, "\"%s\",\"%d\"", *key, value);
    char* valueStr;
    if (!LoadTitleFromFile(stream, key, &valueStr)) return false;
    char* cursor = valueStr;
    int valueInt = 0;
    while (*cursor != 0) valueInt = valueInt * 10 + *cursor++ - '0';
    *value = (int*)malloc(sizeof(int));
    *(*value) = valueInt;
    return true;
}