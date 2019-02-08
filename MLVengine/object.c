#include "object.h"

#include <stdlib.h>
#include <stdio.h>

struct object {
	Vector* pos;
	Vector* vit;
	int killTime;
	DRAW_TYPE drawType;
	Vector* drawScale;
	HITBOX_TYPE hitboxType;
	Vector* hitbox;
	char* drawString;
	void* image;
};

Object* initObject() {
	Object* o = malloc(sizeof(Object));
	return o;
}
void freeObject(Object* o) {
	free(o);
}

Object* newObject() {
	Object* o = initObject();
	o->pos=newVector(0, 0);
	o->vit=newVector(0, 0);
	o->killTime=-1;
	o->drawType=DRAW_NONE;
	o->drawScale=newVector(1, 1);
	o->hitboxType=HITBOX_NONE;
	o->hitbox=newVector(0, 0);
	o->image=NULL;
	
	return o;
}
void destroyObject(Object** o, int hard) {
	if (hard) {
		Vector* pos=getPos(*o);
		Vector* vit=getVit(*o);
		destroyVector(&pos);
		destroyVector(&vit);
		free((*o)->image);
	}
	freeObject(*o);
	*o=NULL;
}

Vector* getPos(Object* o) {
	return o->pos;
}
void setPos(Object* o, Vector* pos) {
	o->pos=pos;
}
Vector* getVit(Object* o) {
	return o->vit;
}
void setVit(Object* o, Vector* vit) {
	o->vit=vit;
}
int getKillTime(Object* o) {
	return o->killTime;
}
void setKillTime(Object* o, int t) {
	o->killTime = t;
}
DRAW_TYPE getDrawType(Object* o) {
	return o->drawType;
}
void setDrawType(Object* o, DRAW_TYPE type) {
	o->drawType = type;
}
Vector* getDrawScale(Object* o) {
	return o->drawScale;
}
void setDrawScale(Object* o, Vector* v) {
	o->drawScale = v;
}
char* getDrawString(Object* o) {
	return o->drawString;
}
void setDrawString(Object* o, char* string) {
	o->drawString = string;
}
HITBOX_TYPE getHitboxType(Object* o) {
	return o->hitboxType;
}
void setHitboxType(Object* o, HITBOX_TYPE type) {
	o->hitboxType = type;
}
Vector* getHitbox(Object* o) {
	return o->hitbox;
}
void setHitbox(Object* o, Vector* v) {
	o->hitbox = v;
}
void* getImage(Object* o) {
	return o->image;
}
void setImage(Object* o, void* i) {
	o->image = i;
}

void applyVit(Object* o) {
	addVector(o->pos, o->vit);
}

void liveAndDie(Object** o) {
	int i = getKillTime(*o);
	if (i<0) return;
	if (i==0) {
		destroyObject(o, 1);
	} else {
		setKillTime(*o, i-1);
	}
}

int touch(Object* o1, Object* o2) {
	if (o1->hitboxType==HITBOX_RECT && o1->hitboxType==HITBOX_RECT) {
		float x1=getX(getPos(o1));
		float y1=getY(getPos(o1));
		float x2=getX(getPos(o2));
		float y2=getY(getPos(o2));
		
		float w1=getX(getHitbox(o1))/2;
		float h1=getY(getHitbox(o1))/2;
		float w2=getX(getHitbox(o2))/2;
		float h2=getY(getHitbox(o2))/2;
		
		float l1=x1-w1;
		float r1=x1+w1;
		float u1=y1-h1;
		float d1=y1+h1;
		float l2=x2-w2;
		float r2=x2+w2;
		float u2=y2-h2;
		float d2=y2+h2;
		
		return (!(r1<l2||r2<l1||d1<u2||d2<u1));
	}
	return 0;
}

void printObject(Object* o) {
	printVector(o->pos);
	printf(" - ");
	printVector(o->pos);
	putchar('\n');
}
