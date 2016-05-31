#ifndef expression
#define expression

typedef struct {
    union {
        float num;
        char op;
    } value;
    bool isNum;
} ExprUnit;

bool MidPost(char* expr, ExprUnit* postExpr);
float PostCount(ExprUnit* postExpr);

#endif