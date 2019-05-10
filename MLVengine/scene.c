#include "scene.h"

#include <stdlib.h>
#include <stdio.h>

struct scene {
	Set* set[NBR_LAYER];
	Object* camera;
	Vector size;
};

Scene* initScene() {
	return (Scene*)malloc(sizeof(Scene));
}

void freeScene(Scene* s) {
	free(s);
}

int cmpEqu(Object* o1, Object* o2) {
	return (getObject(o1)==getObject(o2));
}

Scene* newScene(int x, int y) {
	Scene* s = initScene();
	s->camera=NULL;
	s->size=newVector(x, y);
	int i;
	for (i=0; i<NBR_LAYER; i++) {
		s->set[i]=newSet();
		setEqual(s->set[i], (int (*)(void*, void*))cmpEqu);
	}
	return s;
}

void destroyScene(Scene* s) {
	int i;
	for (i=0; i<NBR_LAYER; i++) {
		destroySet(s->set[i]);
	}
	freeScene(s);
}

void setCamera(Scene* s, Object* o) {
	s->camera=o;
}
Object* getCamera(Scene* s) {
	return s->camera;
}
void setSize(Scene* s, Vector v) {
	s->size=v;
}
Vector* getSize(Scene* s) {
	return &(s->size);
}

int addObject(Scene* s, Object* o, int layer) {
	return addToSet(s->set[layer], o);
}

int removeObject(Scene* s, Object* o, int layer) {
	return removeFromSet(s->set[layer], o);
}

void eachObjectScene(Scene* s, void (*function)(Object*)) {
	void l (void** o) {
		function(*o);
	}
	int i;
	for (i=0; i<NBR_LAYER; i++) eachSet(s->set[i], l);
}

void eachObjectLayer(Scene* s, int layer, void (*function)(Object*)) {
	void l (void** o) {
		function(*o);
	}
	eachSet(s->set[layer], l);
}

void iterate(Scene* s) {
	eachObjectScene(s, applyVit);
	eachObjectScene(s, (void(*)(Object*))liveAndDie);
}
