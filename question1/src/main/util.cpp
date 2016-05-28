#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include "conio.h"


/**
 * 结束应用.
 */
void Finish() {
    printf("任意键退出");
    fflush(stdin);
    getch();
    exit(-1);
}