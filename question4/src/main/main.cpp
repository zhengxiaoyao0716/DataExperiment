#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"
// #include "util.h"

#define INPUT_LEN 1024


void printTreeAction(TreeData data) {
    printf("%s ", data.value);
}

int main() {
    char flag = 'Y';
    do {
        char input[INPUT_LEN];
        printf("请输入英文句子：\n");
        fgets(input, INPUT_LEN, stdin);
        fflush(stdin);
        // input[strlen(input) - 1] = 0;


        Tree* tree = (Tree*)malloc(sizeof(Tree));
        (*tree).data.value = NULL;
        tree -> left = NULL;
        tree -> right = NULL;
        CreateTree(tree, input);

        ListTree(tree, printTreeAction);
        // Finish();
        
        printf("\n输入N退出，否则继续\n");
        scanf("%c", &flag);
        fflush(stdin);
    } while(flag != 'N' && flag != 'n');
}