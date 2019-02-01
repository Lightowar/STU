#ifndef __OBJECT__
#define __OBJECT__

#include "vector.h"

typedef struct object Object;

Object* newObject();
void destroyObject(Object** o, int destroyVecs);
Vector* getPos(Object* o);
void setPos(Object* o, Vector* pos);
Vector* getVit(Object* o);
void setVit(Object* o, Vector* vit);
int getKillTime(Object* o);
void setKillTime(Object* o, int t);
void applyVit(Object* o);
void liveAndDie(Object** o);
void printObject(Object* o);

#endif
