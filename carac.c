#include <math.h>
#include "MLVengine/object.h"
#include <stdlib.h>
#include <MLV/MLV_all.h>

typedef struct carac {
	int lifePoint;
	int maxLifePoint;
	int moveSet;
	Object* target;
	int moveTime;
	int fireSet;
	int fireTime;
	int attack;
	Vector init;
} Carac;

Carac* newCarac(int lifePoint, int maxLifePoint, int moveSet, int fireSet, int fireTime, int attack) {
	Carac* c=(Carac*)malloc(sizeof(Carac));
	c->lifePoint=lifePoint;
	c->maxLifePoint=maxLifePoint;
	c->moveSet=moveSet;
	c->fireSet=fireSet;
	c->moveTime=0;
	c->fireTime=fireTime;
	c->attack=attack;
	c->init=newVector(0, 0);
	return c;
}

void moveTo(Object* o, Vector dest, float max, float acc) {
	Vector v = multiplie(addVector(dest, multiplie(*(getPos(o)), -1)), 0.1);
	if (norme(v)>max) v=multiplie(normalize(v), max);
	Vector a = multiplie(addVector(v, multiplie(*(getVit(o)), -1)), 1);
	if (norme(a)>acc) a=multiplie(normalize(a), acc);
	setVit(o, addVector(*(getVit(o)), a));
}

Vector posClockA(Vector init, int t) {
	return addVector(init, newVector(cosf(t/(float)FPS)*300.0-300, sinf(t/(float)FPS)*300.0));
}

Vector posClockW(Vector init, int t) {
	return addVector(init, newVector(cosf(t/(float)FPS)*-300.0+300, sinf(t/(float)FPS)*300.0));
}

Vector posSinH(Vector init, int t) {
	return addVector(init, newVector(sinf(t/(float)FPS)*400.0, t*100.0/FPS));
}

void vitPlayer(Object* o) {
	static float max=500.0/FPS;
	static float acc=30.0/FPS;
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
	if (getX(*getPos(o))>(SIZE_X/2-UI_SIZE/2)) {
		setX(getPos(o), (SIZE_X/2-UI_SIZE/2));
		setX(getVit(o), 0);
	}
	if (getX(*getPos(o))<(-SIZE_X/2+UI_SIZE/2)) {
		setX(getPos(o), (-SIZE_X/2+UI_SIZE/2));
		setX(getVit(o), 0);
	}
	if (getY(*getPos(o))>(SIZE_Y/2)) {
		setY(getPos(o), (SIZE_Y/2));
		setY(getVit(o), 0);
	}
	if (getY(*getPos(o))<(-SIZE_Y/2)) {
		setY(getPos(o), -(SIZE_Y/2));
		setY(getVit(o), 0);
	}
}

void moveObj (Object* o) {
	Carac* c=getCarac(o);
	if (NULL==c) return;
	switch (c->moveSet) {
		/*
		case 3: vitClockW(o, c->moveTime);
			break;
		case 2: vitClockW(o, -c->moveTime);
			break;
			* */
		case 3: moveTo(o, posClockW(c->init, c->moveTime), 500.0/FPS, 30.0/FPS);
			break;
		case 2: moveTo(o, posClockA(c->init, c->moveTime), 500.0/FPS, 30.0/FPS);
			break;
		case 1: vitPlayer(o);
			break;
		case 4: moveTo(o, posSinH(c->init, c->moveTime), 500.0/FPS, 30.0/FPS);
			break;
	}
	c->moveTime++;
}

void fireEnemy1 (Object* o) {
	Carac* c = (Carac*)getCarac(o);
	if (c->fireTime<=0) {
		addToSet(enemyM, createMissile(o, newVector(0, 7), newVector(-5, 0)));
		addToSet(enemyM, createMissile(o, newVector(0, 7), newVector(5, 0)));
		c->fireTime=FPS;
	}
}

void fireEnemy2 (Object* o) {
	Carac* c = (Carac*)getCarac(o);
	if (c->fireTime<=0) {
		addToSet(enemyM, createMissile(o, newVector(0, 7), newVector(-5, 0)));
		addToSet(enemyM, createMissile(o, newVector(0, 7), newVector(5, 0)));
		c->fireTime=FPS;
	}
}

void firePlayer (Object* o) {
	Carac* c = (Carac*)getCarac(o);
	if (c->fireTime<=0) {
		if (MLV_get_keyboard_state(MLV_KEYBOARD_SPACE)==MLV_PRESSED) {
			addToSet(allyM, createMissile(o, newVector(0, -15), newVector(-5, 0)));
			addToSet(allyM, createMissile(o, newVector(0, -15), newVector(5, 0)));
			c->fireTime=FPS/6;
		} else {
			c->fireTime=0;
		}
	}
}

void fireObj(Object* o) {
	Carac* c = getCarac(o);
	if (c->fireSet==0) return;
	switch (c->fireSet) {
		case 1: firePlayer(o);
			break;
		case 2: fireEnemy1(o);
			break;
	}
	c->fireTime--;
}
