#include "vector.h"

#include <stdlib.h>
#include <stdio.h>

struct vector {
	float x;
	float y;
};

Vector* initVector () {
	return (Vector*)malloc(sizeof(Vector));
}

Vector* newVector (float x, float y) {
	Vector* v = initVector();
	v->x=x;
	v->y=y;
	return v;
}

void freeVector(Vector* v) {
	free(v);
}
void destroyVector(Vector** v) {
	freeVector(*v);
	*v=NULL;
}

float getX (Vector* v) {
	return v->x;
}
float getY (Vector* v) {
	return v->y;
}

void setX (Vector* v, float x) {
	v->x=x;
}
void setY (Vector* v, float y) {
	v->y=y;
}

int getXInt (Vector* v) {
	float f = v->x;
	if (f<0) return -((int)(-f+1));
	return (int)f;
}
int getYInt (Vector* v) {
	float f = v->y;
	if (f<0) return -((int)(-f+1));
	return (int)f;
}

void printVector(Vector* v) {
	printf("%d, %d", getXInt(v), getYInt(v));
}

void addVector(Vector* v1, Vector* v2) {
	v1->x+=v2->x;
	v1->y+=v2->y;
}
