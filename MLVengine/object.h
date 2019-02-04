#ifndef __OBJECT__
#define __OBJECT__

#include "vector.h"

#define DRAW_TYPE int
#define DRAW_NONE 0
#define DRAW_IMAGE 1
#define DRAW_TEXT 2

typedef struct object Object;

Object* newObject();
void destroyObject(Object** o, int destroyVecs);
Vector* getPos(Object* o);
void setPos(Object* o, Vector* pos);
Vector* getVit(Object* o);
void setVit(Object* o, Vector* vit);
int getKillTime(Object* o);
void setKillTime(Object* o, int t);
DRAW_TYPE getDrawType(Object* o);
void setDrawType(Object* o, DRAW_TYPE type);
Vector* getDrawScale(Object* o);
void setDrawScale(Object* o, Vector* scale);
char* getDrawString(Object* o);
void setDrawString(Object* o, char* string);
void applyVit(Object* o);
void liveAndDie(Object** o);
void printObject(Object* o);

#endif
