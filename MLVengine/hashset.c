#include "hashset.h"

#include "set.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

struct hashset {
	Set* map[HASHSET_SIZE];
	unsigned int (*hash)(void*);
	int (*equ)(void*, void*);
};

Hashset* initHashset() {
	return (Hashset*)(malloc(sizeof(Hashset)));
}

Hashset* newHashset() {
	Hashset* h = initHashset();
	int i;
	for (i=0; i<HASHSET_SIZE; i++) {
		h->map[i]=newSet();
	}
	h->hash=NULL;
	h->equ=NULL;
	return h;
}

int equal(Hashset* map, void* elem1, void* elem2) {
	if (map->equ==NULL) return elem1==elem2;
	return map->equ(elem1, elem2);
}
unsigned int hash(Hashset* map, void* elem) {
	if (map->hash==NULL) return ((intptr_t)elem)%HASHSET_SIZE;
	return map->hash(elem)%HASHSET_SIZE;
}

int addInHashset(Hashset* map, void* elem) {
	int h=hash(map, elem);
	return addToSet(map->map[h], elem);
}

void* getFromHashset(Hashset* map, void* elem) {
	int h=hash(map, elem);
	return isInSet(map->map[h], elem);
}

int removeFromHashset(Hashset* map, void* key) {
	int h=hash(map, key);
	return removeFromSet(map->map[h], key);
}

void setHashHashset(Hashset* map, unsigned int (*hash)(void*)) {
	map->hash = hash;
}

void setEqualHashset(Hashset* map, int (*equ)(void*, void*)) {
	map->equ=equ;
	int i;
	for (i=0; i<HASHSET_SIZE; i++) {
		setEqual(map->map[i], equ);
	}
}

void eachHashset(Hashset* h, void (*function)(void**)) {
	int i;
	for (i=0; i<HASHSET_SIZE; i++) {
		eachSet(h->map[i], function);
	}
}
