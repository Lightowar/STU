#ifndef __VECTOR__
#define __VECTOR__

typedef struct vector Vector;

Vector* newVector (float x, float y);
void destroyVector(Vector** v);
float getX (Vector* v);
float getY (Vector* v);
void setX (Vector* v, float x);
void setY (Vector* v, float y);
int getXInt (Vector* v);
int getYInt (Vector* v);
void printVector(Vector* v);
void addVector(Vector* v1, Vector* v2);

#endif
