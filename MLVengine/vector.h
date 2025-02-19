#ifndef __VECTOR__
#define __VECTOR__

typedef struct vector Vector;
struct vector {
	float x;
	float y;
};

Vector newVector (float x, float y);
Vector copyVector (Vector v);
void destroyVector(Vector* v);
float getX (Vector v);
float getY (Vector v);
void setX (Vector* v, float x);
void setY (Vector* v, float y);
int getXInt (Vector v);
int getYInt (Vector v);
void printVector(Vector v);
Vector addVector(Vector v1, Vector v2);
float norme(Vector v);
Vector multiplie(Vector v, float f);
Vector normalize (Vector v);

#endif
