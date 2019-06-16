#include "object.h"

#include <stdlib.h>
#include <stdio.h>
#include "hashset.h"

Hashset* objMap=NULL;

struct object {
	Vector pos;
	Vector vit;
	int killTime;
	DRAW_TYPE drawType;
	Vector drawScale;
	Vector textBox;
	HITBOX_TYPE hitboxType;
	Vector hitbox;
	char* drawString;
	char* drawText;
	void* image;
	void* carac;
    int frameForAnim;
    void* render;
    ObjColor color;
};

Object* getObject(Object* o) {
	if (o==NULL) return NULL;
	if (objMap==NULL) objMap=newHashset();
	return getFromHashset(objMap, o);
}

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
	o->textBox=newVector(-1, -1);
	o->hitboxType=HITBOX_NONE;
	o->hitbox=newVector(0, 0);
	o->image=NULL;
	o->carac=NULL;
    o->frameForAnim=0;
    o->render=NULL;
    o->color.r=0;
    o->color.g=0;
    o->color.b=0;
    o->color.a=0;
    o->drawString=NULL;
    o->drawText=NULL;
	if (objMap==NULL) {
		objMap=newHashset();
	}
	addInHashset(objMap, o);
	return o;
}
void destroyObject(Object* o, int hard) {
	if (getObject(o)==NULL) return;
	if (hard) {
		free((o)->carac);
	}
	if (objMap==NULL) objMap=newHashset();
	removeFromHashset(objMap, o);
	freeObject(o);
}

Vector* getPos(Object* o) {
	return &(o->pos);
}
void setPos(Object* o, Vector pos) {
	o->pos=pos;
}
Vector* getVit(Object* o) {
	return &(o->vit);
}
void setVit(Object* o, Vector vit) {
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
    o->render=NULL;
}
Vector* getDrawScale(Object* o) {
	return &(o->drawScale);
}
void setDrawScale(Object* o, Vector v) {
	o->drawScale = v;
    o->render=NULL;
}
Vector* getTextBox(Object* o) {
	return &(o->textBox);
}
void setTextBox(Object* o, Vector v) {
	o->textBox = v;
    o->render=NULL;
}
char* getDrawString(Object* o) {
	return o->drawString;
}
void setDrawString(Object* o, char* string) {
	o->drawString = string;
    o->frameForAnim=0;
    o->render=NULL;
}
char* getDrawText(Object* o) {
	return o->drawText;
}
void setDrawText(Object* o, char* string) {
	o->drawText = string;
}
HITBOX_TYPE getHitboxType(Object* o) {
	return o->hitboxType;
}
void setHitboxType(Object* o, HITBOX_TYPE type) {
	o->hitboxType = type;
}
Vector* getHitbox(Object* o) {
	return &(o->hitbox);
}
void setHitbox(Object* o, Vector v) {
	o->hitbox = v;
}
void* getCarac(Object* o) {
	return o->carac;
}
void setCarac(Object* o, void* c) {
	o->carac = c;
}
int getFrameForAnim(Object* o) {
	return o->frameForAnim;
}
void setFrameForAnim(Object* o, int f) {
	o->frameForAnim = f;
}

void applyVit(Object* o) {
	o->pos=addVector(o->pos, o->vit);
}

int liveAndDie(Object* o) {
    o->frameForAnim++;
	int i = getKillTime(o);
	if (i<0) return -1;
	if (i==0) {
		destroyObject(o, 1);
		return 1;
	}
	setKillTime(o, i-1);
	return 0;	
}

void* getRender(Object* o) {
	return o->render;
}
void setRender(Object* o, void* render) {
	o->render=render;
}

ObjColor getColor(Object* o) {
	return o->color;
}
void setColor(Object* o, ObjColor color) {
	o->color=color;
}

int touch(Object* o1, Object* o2) {
	if (o1->hitboxType==HITBOX_RECT && o1->hitboxType==HITBOX_RECT) {
		float x1=getX(*getPos(o1));
		float y1=getY(*getPos(o1));
		float x2=getX(*getPos(o2));
		float y2=getY(*getPos(o2));
		
		float w1=getX(*getHitbox(o1))/2;
		float h1=getY(*getHitbox(o1))/2;
		float w2=getX(*getHitbox(o2))/2;
		float h2=getY(*getHitbox(o2))/2;
		
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

int inside(Object* o, Vector v) {
		float w=getX(*getHitbox(o))/2;
		float h=getY(*getHitbox(o))/2;
		
		float x1=getX(*getPos(o));
		float y1=getY(*getPos(o));
		
		float x2=getX(v);
		float y2=getY(v);
		
		return (x1-w<x2 && x2<x1+w && y1-h<y2 && y2<y1+h);
}

void printObject(Object* o) {
	printVector(o->pos);
	printf(" - ");
	printVector(o->pos);
	putchar('\n');
}
