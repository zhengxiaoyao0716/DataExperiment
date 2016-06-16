#include "hashMap.h"

#include <stdlib.h>
#include <stdio.h>

#include "util.h"


/**
 * 抛出异常.
 */
void throwHashException(const char* err) {
    printf(err);
    fflush(stdin);
    getchar();
    exit(-1);
}
/*
 * ================================
 * 哈希键值接口default实现.
 * ================================
 */
int strHashCode(void* key) {
    int hash = 0;
    char* cursor = (char*)key;
    while (*cursor != 0) {
        hash = (hash << 5) - hash + *cursor;
        cursor += 1;
    }
    return hash;
}
bool strEquals(void* self, void* compareTo) {
    char* cursorL = (char*)self;
    char* cursorR = (char*)compareTo;
    while (true)  {
        if (*cursorL == 0) return *cursorR == 0 ? true : false;
        else if (*cursorR == 0) return false;
        else if (*cursorL != *cursorR) return false;
        cursorL++;
        cursorR++;
    }
    // return true;
}
void* simpleReplaceUpdate(void* oldVal, void* newVal) {
    return newVal;
}


/*
 * ================================
 * 哈希表模块.
 * @deprecated 废弃多表支持：创建哈希字典开销不大，一个哈希字典一张表足矣
 * ================================
 */
Hashtable newHashtable(HashKey keyInterface, HashVal valInterface, int initialCapacity, float factor) {
    //需要默认置零
    int capacity = 1;
    while (capacity < initialCapacity) capacity <<= 1;
    
    HashData** dataSet = (HashData**)calloc(capacity, sizeof(HashData*));
    // HashData* dataSet = (HashData*)malloc(capacity * sizeof(HashData));
    // memset(dataSet, 0, capacity * sizeof(HashData));
    // if (dataSet == NULL) ) throwHashException("哈希表创建失败，可能内存不足\n");
    Hashtable table = {
        dataSet, keyInterface, valInterface, capacity, factor,
        {0, (int)(factor * capacity), keyInterface.hashCode}
    };
    hashMap.table = table;
    return table;
}
void bindHashtable(Hashtable table) {
    hashMap.table = table;
}

static const int INTEGER_MAX_VALUE = (~(unsigned int)0)/2;
int calculateHashtableCapacity(int max) {
    int capacity = 1;
    while (capacity < max) capacity <<= 1;
    return capacity;
}
static const int MAXIMUM_CAPACITY = calculateHashtableCapacity(INTEGER_MAX_VALUE / 2);
/**
 * 重哈希.
 * @deprecated 废弃异步支持：会导致系统相关，实现起来也比较麻烦
 */
void resizeHashtable(int newCapacity) {
    if (hashMap.table.capacity == MAXIMUM_CAPACITY) {
        hashMap.table.innerUtil.threshold = INTEGER_MAX_VALUE;
        return;
    }
    //requireLock(hashMap);
    Hashtable table = hashMap.table;
    HashKey newKeyInterface = {
        table.innerUtil.hashCode1Resize,
        table.keyInterface.equals
    };
    Hashtable newTable = newHashtable(
        newKeyInterface, table.valInterface,
        newCapacity, table.factor
    );
    if (newTable.dataSet == NULL) return;  //throwHashException("哈希表创建失败，可能内存不足\n")
    hashMap.table = newTable;

    for (int index = 0; index < table.capacity; index++) {
        HashData* iterator = *(table.dataSet + index);
        while (iterator != NULL) {
            hashMap.put(iterator -> key, iterator -> value);
            HashData* next = iterator -> next;
            free(iterator);
            iterator = next;
        }
    }
    free(table.dataSet);
    hashMap.table.keyInterface.hashCode = table.keyInterface.hashCode;
    //releaswLock(hashMap);
}

/*
 * ================================
 * 哈希字典模块.
 * ================================
 */
void* putIntoTable(void* key, void* value) {
    if (hashMap.table.innerUtil.size > hashMap.table.innerUtil.threshold)
        resizeHashtable(hashMap.table.capacity << 1);
    Hashtable table = hashMap.table;
    int index = table.keyInterface.hashCode(key) & (table.capacity - 1);
    HashData* iterator = *(table.dataSet + index);
    HashData* parent = NULL;
    while (iterator != NULL) {
        if (table.keyInterface.equals((*iterator).key, key)) {
            hashMap.throwKey = (*iterator).key;
            hashMap.throwVal = (*iterator).value;
            (*iterator).key = key;
            void* result = table.valInterface.update((*iterator).value, value);
            (*iterator).value = result;
            return result;
        }
        parent = iterator;
        iterator = iterator -> next;
    }
    hashMap.throwKey = NULL;
    hashMap.throwVal = NULL;
    HashData* node = (HashData*)malloc(sizeof(HashData));
    if (node == NULL) throwHashException("哈希分离链表节点分配失败，可能内存不足\n");
    (*node).key = key;
    (*node).value = value;
    node -> next = iterator;
    if (parent == NULL) *(table.dataSet + index) = node;
    else parent -> next = node;
    hashMap.table.innerUtil.size++;
    return value;
}
void* getFromTable(void* key, void* notFound) {
    Hashtable table = hashMap.table;
    int index = table.keyInterface.hashCode(key) & (table.capacity - 1);
    HashData* iterator = *(table.dataSet + index);
    while (iterator != NULL) {
        if (table.keyInterface.equals((*iterator).key, key)) {
            return (*iterator).value;
        }
        iterator = iterator -> next;
    }
    return notFound;
}
void* removeFromTable(void* key) {
    Hashtable table = hashMap.table;
    int index = table.keyInterface.hashCode(key) & (table.capacity - 1);
    HashData* iterator = *(table.dataSet + index);
    HashData* parent = NULL;
    while (iterator != NULL) {
        if (table.keyInterface.equals((*iterator).key, key)) {
            if (parent == NULL) *(table.dataSet + index) = iterator -> next;
            else parent -> next = iterator -> next;
            void* value = (*iterator).value;
            free(iterator);
            table.innerUtil.size--;
            return value;
        }
        parent = iterator;
        iterator = iterator -> next;
    }
    return NULL;
}
bool containsKeyInTable(void* key) {
    Hashtable table = hashMap.table;
    int index = table.keyInterface.hashCode(key) & (table.capacity - 1);
    HashData* iterator = *(table.dataSet + index);
    while (iterator != NULL) {
        if (table.keyInterface.equals((*iterator).key, key)) return true;
        iterator = iterator -> next;
    }
    return false;
}
bool isHashtableEmpty() {
    return hashMap.table.innerUtil.size <= 0;
}
int sizeOfHashtable() {
    return hashMap.table.innerUtil.size;
}
void listHashtable(void (* action)(void* key, void* value)) {
    for (int index = 0; index < hashMap.table.capacity; index++) {
        HashData* iterator = *(hashMap.table.dataSet + index);
        while (iterator != NULL) {
            action((*iterator).key, (*iterator).value);
            iterator = iterator -> next;
        }
    }
}

/**
 * 哈希字典构造器
 * @deprecated 废弃多实例支持：同时工作的只能有一个table，没有意义
 */
HashMap newHashMap(HashKey keyInterface, HashVal valInterface, int capacity, float factor) {
    Hashtable table = newHashtable(keyInterface, valInterface, capacity, factor);
    if (table.dataSet == NULL) throwHashException("哈希表创建失败，可能内存不足\n");
    HashMap hashMap = {
        putIntoTable,
        getFromTable,
        removeFromTable,
        containsKeyInTable,
        isHashtableEmpty,
        sizeOfHashtable,
        listHashtable,
        table,
        // newHashtable,
        // bindHashtable,
        NULL, NULL
    };
    return hashMap;
}

//单例初始化
HashMap hashMap = {
    putIntoTable,
    getFromTable,
    removeFromTable,
    containsKeyInTable,
    isHashtableEmpty,
    sizeOfHashtable,
    listHashtable,
    {
        (HashData**)calloc(16, sizeof(HashData*)),
        {
            strHashCode,
            strEquals
        },
        {
            simpleReplaceUpdate
        },
        16,
        0.75f,
        {0, 12, strHashCode}
    },
    // newHashtable,
    // bindHashtable,
    NULL, NULL
};  //这看着怎么那么像js。。。
