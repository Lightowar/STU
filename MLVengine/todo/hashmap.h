#ifndef __HASHMAP__
#define __HASHMAP__

typedef struct hashmap Hashmap;
typedef struct entry Entry;
Hashmap* newHashmap();
void destroyHashmap(Hashmap* map);

int setInHashmap(Hashmap* map, void key, void* elem);
void* getFromHashmap(Hashmap* map, void* key);
int removeFromHashmap(Hashmap* map, void* key);
void setHashHashmap(Hashmap* map, int hash(void*));
void setEqualHashmap(Hashmap* map, int equ(void*, void*));
void eachHashmap(Hashmap* h, void (*function)(void**));
void* getKey(Entry* e);
void* getValue(Entry* e);

#endif
