#include "wordUtil.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashMap.h"

bool wordGenerator(char ch) {
    int code;
    /* 原ascii： 0: 48, 9: 57, A: 65, Z: 90, _: 95, a: 97, z: 122 */
    /* 归并到原点连续集合：[1~10],[11~36],[37],[38~63] */
    if (ch >= '0' and ch <= '9') code = ch - 47;        //ch - 48 + 1
    else if (ch >= 'A' and ch <= 'Z') code = ch - 54;   //ch - 65 + 1 + 10
    else if (ch == '_') code = 37;                      //95 - 95 + 1 + 36
    else if (ch >= 'a' and ch <= 'z') code = ch - 59;   //ch - 97 + 1 + 37
    else {
        if (wordUtil.index > 0) {
            *(wordUtil.word + wordUtil.index) = 0;
            wordUtil.index = 0;
            if (wordUtil.wordListener != NULL)
                wordUtil.wordListener(wordUtil.word, wordUtil.hash);
            return true;
        }
        return false;
    }
    
    if (wordUtil.index <= 0) {
        wordUtil.hash = 0;
        wordUtil.wordLen = 8;
        wordUtil.word = (char*)malloc(wordUtil.wordLen * sizeof(char));
    }
    *(wordUtil.word + wordUtil.index) = ch;
    wordUtil.hash = (wordUtil.hash << 5) - wordUtil.hash + code;
    if (++wordUtil.index >= wordUtil.wordLen)
    {
        wordUtil.wordLen <<= 1;
        wordUtil.word = (char*)realloc(wordUtil.word, wordUtil.wordLen * sizeof(char));
    }
    return false;
}

void defaultWordListener(char* word, int hash) {
    free(word);
}
WordUtil wordUtil = {
    0, 8, (char*)malloc(8 * sizeof(char)), 0, wordGenerator, defaultWordListener
};
