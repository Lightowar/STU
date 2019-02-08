#ifndef __OBJECT__
#define __OBJECT__

#include "vector.h"

#define DRAW_TYPE int
#define DRAW_NONE 0
#define DRAW_IMAGE 1
#define DRAW_TEXT 2
#define DRAW_OVAL 3

#define HITBOX_TYPE int
#define HITBOX_NONE 0
#define HITBOX_RECT 1

typedef struct object Object;

Object* newObject();
void destroyObject(Object** o, int hard);
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
HITBOX_TYPE getHitboxType(Object* o);
void setHitboxType(Object* o, HITBOX_TYPE type);
Vector* getHitbox(Object* o);
void setHitbox(Object* o, Vector* v);
void* getImage(Object* o);
void setImage(Object* o, void* i);
void applyVit(Object* o);
void liveAndDie(Object** o);
int touch(Object* o1, Object* o2);
void printObject(Object* o);

#endif
