#include "hashmap.h"

#include "hashset.c"

Hashmap* initHashmap() {
	Hashmap* map = (Hashmap*)newHashset();
	
	return map;
}

Hashmap* newHashmap() {
	Hashmap* map = (Hashmap*)newHashset();
	
	return map;
}

void destroyHashmap(Hashmap* map) {
	//TODO
}

int setInHashmap(Hashmap* map, void key, void* elem) {
	
}

int equalEntry(void* e1, void* e2) {
	return getKey((Entry*)e1==(Entry*)e1);
}
int hashEntry(void* e1, void* e2) {
	return getKey((Entry*)e1==(Entry*)e1);
}
