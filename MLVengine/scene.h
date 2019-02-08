#ifndef __SCENE__
#define __SCENE__

#include "object.h"
#include "set.h"

#define NBR_LAYER 4

typedef struct scene Scene;
Scene* newScene(int x, int y);
int addObject(Scene* s, Object* o, int layer);
int removeObject(Scene* s, Object* o, int layer);
void setCamera(Scene* s, Object* o);
Object* getCamera(Scene* s);
void setSize(Scene* s, Vector* v);
Vector* getSize(Scene* s);
void eachObjectScene(Scene* s, void (*function)(Object**));
void eachObjectLayer(Scene* s, int layer, void (*function)(Object**));
void iterate(Scene* s);

#endif
