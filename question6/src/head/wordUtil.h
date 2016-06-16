#ifndef __WORD_UTIL_H
#define __WORD_UTIL_H

typedef struct {
    int hash;
    int wordLen;
    char* word;
    int index;
    /**
     * 单词生成器.
     * <p>
     * 如果遇到分隔符，且之前存入了一定字符，则变量word末尾补0完成字符串构建，<br />
     * 此时会执行回调监听器wordListener(); 之后返回true，表示组装完成。<br />
     * 默认监听器会释放word并将其置NULL，若需要继续使用word，请重写此监听器。<br />
     * 当你将新的监听器（可以是NULL）注入后，你需要自己对word的内存管理负责，<br />
     * 如果你这么做，请务必在下次调用此方法之前取走或释放word指针，以及hash，<br />
     * 该方法在下次调用时会重新分配word的内存地址，原内存将失去指向，导致泄露<br />
     * </p>
     * @see #wordListener(char*, int) 每当一个单词组装完成时会回调之
     * @param ch 任意字符
     * @return 是否组装完成一个单词
     */
    bool (* wordGenerator)(char ch);
    /**
     * 单词订阅者.
     * <p>
     * 处于安全考虑，默认监听器只会直接释放word指向的内存，你需要重写这一行为。<br />
     * 一个比较简单的方法是直接设置为NULL，并及时手动取走word和hash等所需变量
     * </p>
     * @see #wordGenerator(char)
     * @param word 单词字符串
     * @param hash 单词对应的hash值
     */
    void (* wordListener)(char* word, int hash);
} WordUtil;
extern WordUtil wordUtil;

#endif