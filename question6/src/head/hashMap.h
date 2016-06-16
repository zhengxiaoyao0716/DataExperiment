#ifndef __HASH_MAP_H
#define __HASH_MAP_H
#ifdef __cplusplus
extern "C" {
#endif


/**
 * 哈希键接口，模拟多态以及泛型.
 */
typedef struct {
    int (* hashCode)(void* key);
    bool (* equals)(void* self, void* compareTo);
} HashKey;
/**
 * 哈希值接口，模拟实现多态.
 */
typedef struct {
    /**
     * 哈希值的更新方法.
     * <p>
     * 即当put操作遇到同名键已存在情况下的处理规则<br />
     * <b>！【默认的规则为直接替换，且不对原先的值做内存释放，请注意内存管理】</b>
     * </p>
     * @param oldVal 原先存放的值
     * @param newVal 键冲突的新值
     * @return 最终决定更新为的值
     */
    void* (* update)(void* oldVal, void* newVal);
} HashVal;

/**
 * 哈希内建链表
 */
typedef struct node {
    void* key;
    void* value;
    struct node* next;
} HashData;

/**
 * 哈希表，当前实现为分离链表去重复
 * @param dataSet 数据集
 * @param keyInterface 哈希键接口
 * @param capacity 哈希表(初始)容量（内部强制转换为2的倍数）
 * @param factor 加载因子
 * @param innerUtil 内部辅助项，不用关心
 */
typedef struct {
    HashData** dataSet;
    HashKey keyInterface;
    HashVal valInterface;
    int capacity;
    float factor;
    struct {
        int size;
        int threshold;
        int (* hashCode1Resize)(void* key);
    } innerUtil;
} Hashtable;

/**
 * 哈希字典
 */
typedef struct {
    /* 基本方法 */
    /**
     * 置入键值对.
     * <p>
     * 当put遇到冲突（键已存在）时，会向外推出原先的key、value，<br />
     * 之后update方法会得到回调，此时可以进行新旧value的内存管理，<br />
     * 内部的value最终指向回调返回的指针，key也会指向新传的key的指针，<br />
     * 原先的key、value会被新的key、update返还的value覆盖，<br />
     * 导致失去内部指向而泄露,（value可能例外，比如update返还原先的指向）<br />
     * 请务必尽快（下次put之前）持有或释放掉推出的键值（throwKey与throwVal）
     * </p>
     */
    void* (* put)(void* key, void* value);
    void* (* get)(void* key, void* notFound);
    void* (* remove)(void* key);
    bool (* contains)(void* key);
    bool (* isEmpty)();
    int (* size)();
    void (* list)(void (* action)(void* key, void* value));
    /** 内建的哈希表 */
    Hashtable table;
    // Hashtable (* newTable)(HashKey keyInterface, HashVal valInterface, int capacity, float factor);
    // void (* bindTable)(Hashtable table);
    // void (* resize)(Hashtable table);
    void* throwKey;
    void* throwVal;
} HashMap;
/**
 * 哈希字典构造器
 * @deprecated 废弃多实例支持：同时工作的只能有一个table，没有意义
 */
HashMap newHashMap(HashKey keyInterface, HashVal valInterface, int capacity, float factor);
/** 哈希字典单例 */
extern HashMap hashMap;

#ifdef __cplusplus
}
#endif
#endif
