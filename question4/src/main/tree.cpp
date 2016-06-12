#include "tree.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

#define WORD_DEFAULT_LEN 12

// int compareTo(char* self, char* compareWith) {
//     if (*self == 0) return *compareWith == 0 ? 0 : -1;
//     else if (*compareWith == 0) return 1;
//     else return *self == *compareWith ? compareTo(++self, ++compareWith) : *self - *compareWith;
// }
int compareByDict(TreeData newWord, TreeData word) {
    char* cursorL = newWord.value;
    char* cursorR = word.value;
    // return compareTo(cursorL, cursorR);
    while (true)  {
        if (*cursorL == 0) return *cursorR == 0 ? 0 : -1;
        else if (*cursorR == 0) return 1;
        else if (*cursorL != *cursorR) return *cursorL - *cursorR;
        cursorL++;
        cursorR++;
    }
}
bool insertToNode(Tree* node, TreeData newWord, int (* comparator)(TreeData newWord, TreeData word)) {
    Tree* nextNode;
    int compareResult = comparator(newWord, (*node).data);
    if (compareResult < 0) {
        nextNode = node -> left;
        if (nextNode == NULL) {
            Tree* newNode = (Tree*)malloc(sizeof(Tree));
            newNode -> left = NULL;
            newNode -> right = NULL;
            (*newNode).data = newWord;
            node -> left = newNode;
            return true;
        }
    }
    else if (compareResult > 0) {
        nextNode = node -> right;
        if (nextNode == NULL) {
            Tree* newNode = (Tree*)malloc(sizeof(Tree));
            newNode -> left = NULL;
            newNode -> right = NULL;
            (*newNode).data = newWord;
            node -> right = newNode;
            return true;
        }
    }
    else return true;
    return insertToNode(nextNode, newWord, comparator);
}
bool insertTree(Tree* tree, TreeData newWord, int (* comparator)(TreeData newWord, TreeData word)) {
    if ((*tree).data.value == NULL) {
        (*tree).data = newWord;
        return true;
    }
    else return insertToNode(tree, newWord, comparator);
}

/**
 * 创建二叉树.
 * @param tree 二叉树
 * @param input 输入的语句
 * @return 成败
 */
bool CreateTree(Tree* tree, char* input) {
    int len = WORD_DEFAULT_LEN, index = 0;
    char* word = (char*)malloc(len * sizeof(char));
    char* cursor = input;
    while (true) {
        if ((*cursor != ' ') and (*cursor != '.') and (*cursor != '\n')) {
            if (index + 1 >= len) {
                len <<= 1;
                char* newWord = (char*)realloc(word, len * sizeof(char));
                if (newWord == NULL) return false;
                word = newWord;
            }
            *(word + index++) = *cursor;
        }
        else {
            if (index > 0) {
                *(word + index) = 0;

                TreeData data = {word};
                insertTree(tree, data, compareByDict);

                len = WORD_DEFAULT_LEN;
                index = 0;
                word = (char*)malloc(len * sizeof(char));
            }
            if (*cursor != ' ') break;
        }
        cursor++;
    }
    fflush(stdin);

    return true;
}

/**
 * 遍历二叉树（中序）.
 * @param tree 二叉树
 * @param action 对每一节点的动作
 */
void ListTree(Tree* tree, void (* action)(TreeData data)) {
    StackUtil stack = NewStackUtil();
    stack.init();
    stack.push(tree);
    while (!stack.isEmpty()) {
        bool visited = stack.isTopVisited();
        Tree* cursor = stack.pop();
        if (cursor == NULL) continue;
        else if (visited) action((*cursor).data);
        else {
            stack.push(cursor -> right);
            stack.push(cursor);
            stack.markTopVisited();
            stack.push(cursor -> left);
        }
    }
    stack.release();
}
