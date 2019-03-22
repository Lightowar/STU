#include "render.h"

#include <MLV/MLV_all.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include "vector.h"
#include "hashset.h"


typedef struct renderElem RenderElem;

struct renderElem {
	MLV_Image** elem;
	int nbrImg;
	char str[128];
	int decX;
	int decY;
	double time;
	double timeMax;
};

#include "read_dir.c"

Hashset* renderMap=NULL;

int hashStr(char* str) {
	if (str[0]==0) {
		return 0;
	}			
	int h = str[0]+hashStr(str+1);
	if (h>=HASHSET_SIZE) {
		h-=HASHSET_SIZE;
	}
	return h;
}
int hashElem(void* elem) {
	char* str = (char*)((RenderElem*)elem)->str;
	return hashStr(str);
}
int equElem(void* elem1, void* elem2) {
	if (elem1==NULL || elem2==NULL) return (elem1==NULL && elem2==NULL);
	return (strcmp(((RenderElem*)elem1)->str, ((RenderElem*)elem2)->str)==0);
}

RenderElem* getImageFromIndex(char* str) {
	RenderElem r = {NULL, 0, {0}, 1, 1, 0, 0};
	strcpy(r.str, str);
	return getFromHashset(renderMap, &r);
}

void renderInit(Scene* s) {
	MLV_create_window("test", "test", getXInt(*getSize(s)), getYInt(*getSize(s)));
	if (renderMap == NULL) renderMap=newHashset();
	setHashHashset(renderMap, hashElem);
	setEqualHashset(renderMap, equElem);
}

void renderEnd() {
	void freeImg(void** img) {
		int i;
		for (i=0; i<(*(RenderElem**)img)->nbrImg; i++) {
			MLV_free_image(((RenderElem*)(*img))->elem[i]);
		}
		free((RenderElem*)(*img));
	}
	eachHashset(renderMap, freeImg);
	//destroyHashset(renderMap);
	return;
}

MLV_Color stringToColor(char* str) {
	if (strcmp(str, "red")==0) {return MLV_COLOR_RED;}
	if (strcmp(str, "white")==0) {return MLV_COLOR_WHITE;}
	if (strcmp(str, "starColor")==0) {return MLV_rgba(255, 255, 200, 200);}
	return MLV_COLOR_BLACK;
}

void drawObject(Scene* s, Object* o, int camEnable, double time) {
	Vector* v = getPos(o);
	int x=0; int y=0;
	if (camEnable) {
		x+=getXInt(*getSize(s))/2;
		y+=getYInt(*getSize(s))/2;
		Object* cam=getCamera(s);
		if (cam!=NULL) {
			x-=getXInt(*getPos(cam));
			y-=getYInt(*getPos(cam));
		}
	}
	x+=getXInt(*v);
	y+=getYInt(*v);
	char* str = getDrawString(o);
	if (str!=NULL && getDrawType(o)==DRAW_IMAGE) {
		RenderElem* img = getImageFromIndex(str);
		if (img==NULL) {
			img=readDir(str);
			addInHashset(renderMap, img);
		}
		MLV_draw_image(getImageRender(img, time), x-img->decX, y-img->decY);
	}
	if (getDrawType(o)==DRAW_OVAL) {
		Vector* scale=getDrawScale(o);
		MLV_draw_filled_ellipse(x, y, getX(*scale), getY(*scale), stringToColor(str));
	}
	if (getDrawType(o)==DRAW_TEXT) {
		MLV_draw_text(x, y, str, MLV_COLOR_RED);
	}
}

void debugObject(Scene* s, Object* o, int camEnable, double time) {
	Vector* v = getPos(o);
	int x=0; int y=0;
	if (camEnable) {
		x+=getXInt(*getSize(s))/2;
		y+=getYInt(*getSize(s))/2;
		Object* cam=getCamera(s);
		if (cam!=NULL) {
			x-=getXInt(*getPos(cam));
			y-=getYInt(*getPos(cam));
		}
	}
	x+=getXInt(*v);
	y+=getYInt(*v);
	MLV_draw_line(x-15, y, x+15, y, MLV_COLOR_RED);
	MLV_draw_line(x, y-15, x, y+15, MLV_COLOR_RED);
	if (getHitboxType(o)==HITBOX_RECT) {
		Vector* hit=getHitbox(o);
		int i=getXInt(*hit);
		int j=getYInt(*hit);
		MLV_draw_rectangle(x-i/2, y-j/2, i, j, MLV_COLOR_RED);
	}
}

void renderScene(Scene* s, double time) {
	void drO(Object* o) {return drawObject(s, o, 1, time);};
	void drOp(Object* o) {return drawObject(s, o, 0, time);};
	MLV_clear_window( MLV_COLOR_BLACK );
	int i;
	for(i=0; i<NBR_LAYER-1; i++) eachObjectLayer(s, i, drO);
	eachObjectLayer(s, NBR_LAYER-1, drOp);
	MLV_update_window();
}

void debugScene(Scene* s, double time) {
	void drO(Object* o) {return drawObject(s, o, 1, time);};
	void deO(Object* o) {return debugObject(s, o, 1, time);};
	void drOp(Object* o) {return drawObject(s, o, 0, time);};
	void deOp(Object* o) {return debugObject(s, o, 0, time);};
	MLV_clear_window( MLV_COLOR_BLACK );
	int i;
	for(i=0; i<NBR_LAYER-1; i++) eachObjectLayer(s, i, drO);
	eachObjectLayer(s, NBR_LAYER-1, drOp);
	for(i=0; i<NBR_LAYER-1; i++) eachObjectLayer(s, i, deO);
	eachObjectLayer(s, NBR_LAYER-1, deOp);
	MLV_update_window();
}
