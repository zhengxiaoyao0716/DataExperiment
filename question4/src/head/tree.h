#ifndef __TREE_H
#define __TREE_H

typedef struct {
    char *value;
} TreeData;

/**
 * 表达式数据单元，{'#', false} 表示空.
 */
typedef struct treeNode {
    TreeData data;
    struct treeNode* left;
    struct treeNode* right;
} Tree;

/**
 * 创建二叉树.
 * @param tree 二叉树
 * @param input 输入的语句
 * @return 成败
 */
bool CreateTree(Tree* tree, char* input);

/**
 * 遍历二叉树（中序）.
 * @param tree 二叉树
 * @param action 对每一节点的动作
 */
void ListTree(Tree* tree, void (* action)(TreeData data));

#endif