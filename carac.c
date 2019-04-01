#include <math.h>
#include "MLVengine/object.h"
#include <stdlib.h>
#include <MLV/MLV_all.h>

typedef struct carac {
	int lifePoint;
	int maxLifePoint;
	int group;
	int move;
	Object* target;
	int moveTime;
} Carac;

Carac* newCarac(int lifePoint, int maxLifePoint, int group, int move, Object* target) {
	Carac* c=(Carac*)malloc(sizeof(Carac));
	c->lifePoint=lifePoint;
	c->group=group;
	c->move=move;
	c->target=target;
	c->moveTime=0;
	return c;
}

void vitClockW(Object* o, int t) {
	Vector* v=getVit(o);
	t*=FPS;
	t/=30;
	setY(v, -cosf(t/100.0));
	setX(v, -sinf(t/100.0));
}

void vitPlayer(Object* o) {
	static float max=300.0/FPS;
	static float acc=15.0/FPS;
	Vector* vit=getVit(o);
	float x=getX(*vit);
	float y=getY(*vit);
	float targetX=0;
	float targetY=0;
	if (MLV_get_keyboard_state(MLV_KEYBOARD_LEFT)==MLV_PRESSED) targetX-=max;
	if (MLV_get_keyboard_state(MLV_KEYBOARD_DOWN)==MLV_PRESSED) targetY+=max;
	if (MLV_get_keyboard_state(MLV_KEYBOARD_UP)==MLV_PRESSED) targetY-=max;
	if (MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT)==MLV_PRESSED) targetX+=max;
	if (targetX!=0 && targetY!=0) {
		targetX*=0.707107;
		targetY*=0.707107;
	}
	float accX=targetX-x;
	float accY=targetY-y;
	if (accX>acc)accX=acc;
	if (accX<-acc)accX=-acc;
	if (accY>acc)accY=acc;
	if (accY<-acc)accY=-acc;
	x+=accX;
	y+=accY;
	setX(vit, x);
	setY(vit, y);
}

void moveObj (Object* o) {
	Carac* c=getCarac(o);
	if (NULL==c) return;
	switch (c->move) {
		case 1: vitClockW(o, c->moveTime);
			break;
		case 2: vitClockW(o, -c->moveTime);
			break;
		case 3: vitPlayer(o);
			break;
	}
	c->moveTime++;
}
