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
Vector posLeftRight(Vector init, int t) {
	if (t>100) return newVector(0, -100);
	if ((t%200)<100) return addVector(init, newVector(100, 0));
	return addVector(init, newVector(-100, 0));
}

Vector getTargetVitPlayer(Object* o) {
	if (mouse) {
		Vector v = addVector(getMousePos(), newVector(-(SIZE_X/2-UI_SIZE/2), -SIZE_Y/2));
		if (getX(v)>(SIZE_X/2-UI_SIZE/2)) setX(&v, SIZE_X/2-UI_SIZE/2);
		return v;
	}
	float targetX=0;
	float targetY=0;
	if (isPressed("LEFT")) targetX-=1;
	if (isPressed("DOWN")) targetY+=1;
	if (isPressed("UP")) targetY-=1;
	if (isPressed("RIGHT")) targetX+=1;
	if (targetX!=0 && targetY!=0) {
		targetX*=0.707107;
		targetY*=0.707107;
	}
	targetX*=100;
	targetY*=100;
	targetX+=getX(*getPos(o));
	targetY+=getY(*getPos(o));
	if (targetX>(SIZE_X/2-UI_SIZE/2)) {
		targetX=(SIZE_X/2-UI_SIZE/2);
	}
	if (targetX<(-SIZE_X/2+UI_SIZE/2)) {
		targetX=(-SIZE_X/2+UI_SIZE/2);
	}
	if (targetY>(SIZE_Y/2)) {
		targetY=(SIZE_Y/2);
	}
	if (targetY<(-SIZE_Y/2)) {
		targetY=(-SIZE_Y/2);
	}
	return newVector(targetX, targetY);
}

void vitPlayer(Object* o) {
}

void moveObj (Object* o) {
	Carac* c=getCarac(o);
	if (NULL==c) return;
	switch (c->moveSet) {
		case 3: moveTo(o, posClockW(c->init, c->moveTime), 500.0/FPS, 30.0/FPS);
			break;
		case 2: moveTo(o, posClockA(c->init, c->moveTime), 500.0/FPS, 30.0/FPS);
			break;
		case 1:
			if (mouse) moveTo(o, getTargetVitPlayer(o), 1000.0/FPS, 60.0/FPS);
			else moveTo(o, getTargetVitPlayer(o), 500.0/FPS, 30.0/FPS);
			break;
		case 4: moveTo(o, posSinH(c->init, c->moveTime), 500.0/FPS, 30.0/FPS);
			break;
		case 5: moveTo(o, posLeftRight(c->init, c->moveTime), 500.0/FPS, 30.0/FPS);
			break;
	}
	c->moveTime++;
}

void fireEnemy1 (Object* o) {
	Carac* c = (Carac*)getCarac(o);
	if (c->fireTime<=0) {
		if (getObject(player)==NULL)
			addToSet(enemyM, createMissile(o, newVector(0, 2), newVector(0, 0), 5, 1));
		else
			addToSet(enemyM, createMissile(o, multiplie(normalize(addVector(*getPos(player), multiplie(*getPos(o), -1))), 2), newVector(0, 0), 5, 1));
		c->fireTime=FPS;
	}
}

void fireEnemy2 (Object* o) {
	Carac* c = (Carac*)getCarac(o);
	if (c->fireTime<=0) {
		addToSet(enemyM, createMissile(o, newVector(0, 7), newVector(-5, 0), 3, 0));
		addToSet(enemyM, createMissile(o, newVector(0, 7), newVector(5, 0), 3, 0));
		c->fireTime=FPS;
	}
}

void firePlayer (Object* o) {
	Carac* c = (Carac*)getCarac(o);
	if (c->fireTime<=0) {
		if (isPressed("SPACE") || click(0)) {
			addToSet(allyM, createMissile(o, newVector(0, -15), newVector(-5, 0), 3, 0));
			addToSet(allyM, createMissile(o, newVector(0, -15), newVector(5, 0), 3, 0));
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
