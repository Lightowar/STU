#include "vector.h"

#include <stdio.h>
#include <math.h>


Vector initVector () {
	Vector v = {0, 0};
	return v;
}

Vector newVector (float x, float y) {
	Vector v = initVector();
	v.x=x;
	v.y=y;
	return v;
}
Vector copyVector (Vector v) {
	Vector v2 = initVector();
	v2.x=v.x;
	v2.y=v.y;
	return v2;
}

void freeVector(Vector v) {
}

void destroyVector(Vector* v) {
}

float getX (Vector v) {
	return v.x;
}
float getY (Vector v) {
	return v.y;
}

void setX (Vector* v, float x) {
	v->x=x;
}
void setY (Vector* v, float y) {
	v->y=y;
}

int getXInt (Vector v) {
	float f = v.x;
	if (f<0) return -((int)(-f+1));
	return (int)f;
}
int getYInt (Vector v) {
	float f = v.y;
	if (f<0) return -((int)(-f+1));
	return (int)f;
}

void printVector(Vector v) {
	printf("%d, %d\n", getXInt(v), getYInt(v));
}

Vector addVector(Vector v1, Vector v2) {
	return newVector(v1.x+v2.x, v1.y+v2.y);
}

float norme(Vector v) {
	return sqrt(v.x*v.x+v.y*v.y);
}

Vector multiplie(Vector v, float f) {
	v.x*=f;
	v.y*=f;
	return v;
}

Vector normalize(Vector v) {
	return multiplie(v, 1/norme(v));
}
