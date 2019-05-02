#ifndef __OBJECT__
#define __OBJECT__

#include "vector.h"

#define DRAW_TYPE int
#define DRAW_NONE 0
#define DRAW_IMAGE 1
#define DRAW_TEXT 2
#define DRAW_OVAL 3
#define DRAW_RECT 4

#define HITBOX_TYPE int
#define HITBOX_NONE 0
#define HITBOX_RECT 1

typedef struct object Object;

typedef struct objColor {
	int r;
	int g;
	int b;
	int a;
} ObjColor;

Object* getObject(Object* o);

Object* newObject();
void destroyObject(Object* o, int hard);
Vector* getPos(Object* o);
void setPos(Object* o, Vector pos);
Vector* getVit(Object* o);
void setVit(Object* o, Vector vit);
int getKillTime(Object* o);
void setKillTime(Object* o, int t);
DRAW_TYPE getDrawType(Object* o);
void setDrawType(Object* o, DRAW_TYPE type);
Vector* getDrawScale(Object* o);
void setDrawScale(Object* o, Vector scale);
char* getDrawString(Object* o);
Vector* getTextBox(Object* o);
void setTextBox(Object* o, Vector v);
void setDrawString(Object* o, char* string);
char* getDrawText(Object* o);
void setDrawText(Object* o, char* string);
HITBOX_TYPE getHitboxType(Object* o);
void setHitboxType(Object* o, HITBOX_TYPE type);
Vector* getHitbox(Object* o);
void setHitbox(Object* o, Vector v);
void* getCarac(Object* o);
void setCarac(Object* o, void* c);
int getFrameForAnim(Object* o);
void setFrameForAnim(Object* o, int f);
void* getRender(Object* o);
void setRender(Object* o, void* render);
ObjColor getColor(Object* o);
void setColor(Object* o, ObjColor color);

void applyVit(Object* o);
int liveAndDie(Object* o);
int touch(Object* o1, Object* o2);
void printObject(Object* o);

#endif
