#ifndef __HASHMAP__
#define __HASHMAP__

#define HASHMAP_SIZE 256

typedef struct hashmap Hashmap;
Hashmap* newHashmap();
void destroyHashmap(Hashmap* map);

int addInMap(Hashmap* map, void* elem);
void* getFromMap(Hashmap* map, void* elem);
int removeFromMap(Hashmap* map, void* key);
void setHashFunc(Hashmap* map, int hash(void*));
void setEqualFunc(Hashmap* map, int equ(void*, void*));

#endif
