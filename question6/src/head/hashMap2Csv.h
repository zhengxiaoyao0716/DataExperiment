#ifndef __HASH_MAP_TO_CSV_H
#define __HASH_MAP_TO_CSV_H

#include <stdio.h>


void WriteTitleToFile(FILE* stream, char* column1, char* column2);
void WriteLineToFile(FILE* stream, char* key, int value);
bool LoadTitleFromFile(FILE* stream, char** column1, char** column2);
bool LoadLineFromFile(FILE* stream,  char** key, int** value);

#endif