#include "hashmap.h"

#include "set.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

struct hashmap {
	Set* map[HASHMAP_SIZE];
	int (*hash)(void*);
	int (*equ)(void*, void*);
};

Hashmap* initHashmap() {
	return (Hashmap*)(malloc(sizeof(Hashmap)));
}

Hashmap* newHashmap() {
	Hashmap* h = initHashmap();
	int i;
	for (i=0; i<HASHMAP_SIZE; i++) {
		h->map[i]=newSet();
	}
	h->hash=NULL;
	h->equ=NULL;
	return h;
}

int equal(Hashmap* map, void* elem1, void* elem2) {
	if (map->equ==NULL) return elem1==elem2;
	return map->equ(elem1, elem2);
}
int hash(Hashmap* map, void* elem) {
	if (map->hash==NULL) return ((intptr_t)elem)%HASHMAP_SIZE;
	return map->hash(elem)%HASHMAP_SIZE;
}


void addInSet(Hashmap* map, void* elem) {
	
}

int addInMap(Hashmap* map, void* elem) {
	int h=hash(map, elem);
	return addToSet(map->map[h], elem);
}

void* getFromMap(Hashmap* map, void* elem) {
	int h=hash(map, elem);
	return isInSet(map->map[h], elem);
}

int removeFromMap(Hashmap* map, void* key) {
	int h=hash(map, key);
	return removeFromSet(map->map[h], key);
}

void setHashFunc(Hashmap* map, int (*hash)(void*)) {
	map->hash = hash;
}

void setEqualFunc(Hashmap* map, int (*equ)(void*, void*)) {
	map->equ=equ;
	int i;
	for (i=0; i<HASHMAP_SIZE; i++) {
		setEqual(map->map[i], equ);
	}
}
