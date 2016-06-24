#include <stdlib.h>
#include <stdio.h>

#include "list.h"
// #include "util.h"


int main() {
    char flag;
    do {
        List* h;
        h = (List*)malloc(sizeof(List));
        (*h).value = 0;
        (*h).next = NULL;
        
        CreateList(h);
        
        int k;
        printf("请输入k的值：");
        scanf("%d", &k);
        fflush(stdin);
        
        int result[2];
        Adjmax(h, k, result);
        printf("序号%d, data值%d\n", result[0], result[1]);
        
        FreeList(h);
        // Finish();
        
        printf("输入N退出，否则继续\n");
        scanf("%c", &flag);
        fflush(stdin);
    } while(flag != 'N' && flag != 'n');
}