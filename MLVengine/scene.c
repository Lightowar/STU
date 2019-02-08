#include "scene.h"

#include <stdlib.h>
#include <stdio.h>

struct scene {
	Set* set[NBR_LAYER];
	Object* camera;
	Vector* size;
};

Scene* initScene() {
	return (Scene*)malloc(sizeof(Scene));
}

Scene* newScene(int x, int y) {
	Scene* s = initScene();
	s->size=newVector(x, y);
	int i;
	for (i=0; i<NBR_LAYER; i++) s->set[i]=newSet();
	return s;
}
void setCamera(Scene* s, Object* o) {
	s->camera=o;
}
Object* getCamera(Scene* s) {
	return s->camera;
}
void setSize(Scene* s, Vector* v) {
	s->size=v;
}
Vector* getSize(Scene* s) {
	return s->size;
}

int addObject(Scene* s, Object* o, int layer) {
	return addToSet(s->set[layer], o);
}

int removeObject(Scene* s, Object* o, int layer) {
	return removeFromSet(s->set[layer], o);
}

void eachObjectScene(Scene* s, void (*function)(Object**)) {
	int i;
	for (i=0; i<NBR_LAYER; i++) eachSet(s->set[i], (void(*)(void**))function);
}

void eachObjectLayer(Scene* s, int layer, void (*function)(Object**)) {
	eachSet(s->set[layer], (void(*)(void**))function);
}

void applyVitPtr(Object** o) {
	applyVit(*o);
}

void iterate(Scene* s) {
	eachObjectScene(s, applyVitPtr);
	eachObjectScene(s, liveAndDie);
}
