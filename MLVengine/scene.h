#ifndef __SCENE__
#define __SCENE__

#include "object.h"

typedef struct scene Scene;
Scene* newScene();
int addObject(Scene* s, Object* o);
int removeObject(Scene* s, Object* o);
void eachObjects(Scene* s, void (*function)(Object**));
void iterate(Scene* s);

#endif
