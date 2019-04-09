#ifndef __HASHSET__
#define __HASHSET__

#define HASHSET_SIZE 256

typedef struct hashset Hashset;
Hashset* newHashset();
void destroyHashset(Hashset* map);

int addInHashset(Hashset* h, void* elem);
void* getFromHashset(Hashset* h, void* elem);
int removeFromHashset(Hashset* h, void* key);
void setHashHashset(Hashset* h, unsigned int hash(void*));
void setEqualHashset(Hashset* h, int equ(void*, void*));
void eachHashset(Hashset* h, void (*function)(void**));

#endif
