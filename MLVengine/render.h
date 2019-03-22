#ifndef __RENDER__
#define __RENDER__

#include "scene.h"

void renderInit(Scene* s);
void renderEnd();
void renderScene(Scene* s, double time);
void debugScene(Scene* s, double time);

#endif
