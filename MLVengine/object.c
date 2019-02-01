#include "object.h"

#include <stdlib.h>
#include <stdio.h>

struct object {
	Vector* pos;
	Vector* vit;
	int killTime;
};

Object* initObject() {
	Object* o = malloc(sizeof(Object));
	return o;
}
void freeObject(Object* o) {
	free(o);
}

Object* newObject() {
	Object* o = initObject();
	o->pos=newVector(0, 0);
	o->vit=newVector(0, 0);
	o->killTime=-1;
	return o;
}
void destroyObject(Object** o, int destroyVecs) {
	if (destroyVecs) {
		Vector* pos=getPos(*o);
		Vector* vit=getVit(*o);
		destroyVector(&pos);
		destroyVector(&vit);
	}
	freeObject(*o);
	*o=NULL;
}

Vector* getPos(Object* o) {
	return o->pos;
}
void setPos(Object* o, Vector* pos) {
	o->pos=pos;
}

Vector* getVit(Object* o) {
	return o->vit;
}
void setVit(Object* o, Vector* vit) {
	o->vit=vit;
}

int getKillTime(Object* o) {
	return o->killTime;
}
void setKillTime(Object* o, int t) {
	o->killTime = t;
}

void applyVit(Object* o) {
	addVector(o->pos, o->vit);
}

void liveAndDie (Object** o) {
	int i = getKillTime(*o);
	if (i<0) return;
	if (i==0) {
		destroyObject(o, 1);
	} else {
		setKillTime(*o, i-1);
	}
}

void printObject(Object* o) {
	printVector(o->pos);
	printf(" - ");
	printVector(o->pos);
	putchar('\n');
}
