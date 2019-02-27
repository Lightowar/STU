#include "render.h"

#include <MLV/MLV_all.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include "vector.h"

#define MAP_SIZE 256

typedef struct maillon Maillon;

struct maillon {
	MLV_Image* elem;
	char* str;
	Maillon* next;
	int decX;
	int decY;
};

Maillon* renderMap[MAP_SIZE];

int hashStr(char* str) {
	if (str[0]==0) {
		return 0;
	}			
	int h = str[0]+hashStr(str+1);
	if (h>=MAP_SIZE) {
		h-=MAP_SIZE;
	}
	return h;
}

Maillon* getImageFromIndex(char* str) {
	int h = hashStr(str);
	Maillon* m = renderMap[h];
	while (m!=NULL) {
		if (strcmp(m->str, str)==0) {
			return m;
		}
		m=m->next;
	}
	m=renderMap[h];
	Maillon* m2 = (Maillon*)malloc(sizeof(Maillon));
	m2->elem=MLV_load_image(str);
	m2->str=str;
	m2->next=m;
	int x, y;
	
	MLV_get_image_size(m2->elem, &x, &y);
	m2->decX=x/2;
	
	m2->decY=y/2;
	renderMap[h]=m2;
	return m2;
}

void renderInit(Scene* s) {
	MLV_create_window("test", "test", getXInt(getSize(s)), getYInt(getSize(s)));
}

MLV_Color stringToColor(char* str) {
	if (strcmp(str, "red")==0) {return MLV_COLOR_RED;}
	if (strcmp(str, "white")==0) {return MLV_COLOR_WHITE;}
	if (strcmp(str, "starColor")==0) {return MLV_rgba(255, 255, 200, 200);}
	return MLV_COLOR_BLACK;
}

typedef struct renderImg RenderImg;
struct renderImg {
	MLV_Image* image;
	char* string;
};

RenderImg* newRenderImg() {
	RenderImg* rimg=(RenderImg*)malloc(sizeof(RenderImg));
	rimg->image=NULL;
	rimg->string="";
	return rimg;
}

void drawObject(Scene* s, Object* o, int camEnable) {
	Vector* v = getPos(o);
	int x=0; int y=0;
	if (camEnable) {
		x+=getXInt(getSize(s))/2;
		y+=getYInt(getSize(s))/2;
		Object* cam=getCamera(s);
		if (cam!=NULL) {
			x-=getXInt(getPos(cam));
			y-=getYInt(getPos(cam));
		}
	}
	x+=getXInt(v);
	y+=getYInt(v);
	char* str = getDrawString(o);
	if (str!=NULL && getDrawType(o)==DRAW_IMAGE) {
		RenderImg* r=getImage(o);
		if (r==NULL) {
			r=newRenderImg();
			setImage(o, r);
		}
		
		if (strcmp(r->string, str)!=0) {
			r->image=getImageFromIndex(str)->elem;
			r->string=str;
		}
		Maillon* img = getImageFromIndex(str);
		MLV_draw_image(img->elem, x-img->decX, y-img->decY);
	}
	if (getDrawType(o)==DRAW_OVAL) {
		Vector* scale=getDrawScale(o);
		MLV_draw_filled_ellipse(x, y, getX(scale), getY(scale), stringToColor(str));
	}
	if (getDrawType(o)==DRAW_TEXT) {
		MLV_draw_text(x, y, str, MLV_COLOR_RED);
	}
}

void debugObject(Scene* s, Object* o, int camEnable) {
	Vector* v = getPos(o);
	int x=0; int y=0;
	if (camEnable) {
		x+=getXInt(getSize(s))/2;
		y+=getYInt(getSize(s))/2;
		Object* cam=getCamera(s);
		if (cam!=NULL) {
			x-=getXInt(getPos(cam));
			y-=getYInt(getPos(cam));
		}
	}
	x+=getXInt(v);
	y+=getYInt(v);
	MLV_draw_line(x-15, y, x+15, y, MLV_COLOR_RED);
	MLV_draw_line(x, y-15, x, y+15, MLV_COLOR_RED);
	if (getHitboxType(o)==HITBOX_RECT) {
		Vector* hit=getHitbox(o);
		int i=getXInt(hit);
		int j=getYInt(hit);
		MLV_draw_rectangle(x-i/2, y-j/2, i, j, MLV_COLOR_RED);
	}
}

void renderScene(Scene* s) {
	void drO(Object* o) {return drawObject(s, o, 1);};
	void drOp(Object* o) {return drawObject(s, o, 0);};
	MLV_clear_window( MLV_COLOR_BLACK );
	int i;
	for(i=0; i<NBR_LAYER-1; i++) eachObjectLayer(s, i, drO);
	eachObjectLayer(s, NBR_LAYER-1, drOp);
	MLV_update_window();
}

void debugScene(Scene* s) {
	void drO(Object* o) {return drawObject(s, o, 1);};
	void deO(Object* o) {return debugObject(s, o, 1);};
	void drOp(Object* o) {return drawObject(s, o, 0);};
	void deOp(Object* o) {return debugObject(s, o, 0);};
	MLV_clear_window( MLV_COLOR_BLACK );
	int i;
	for(i=0; i<NBR_LAYER-1; i++) eachObjectLayer(s, i, drO);
	eachObjectLayer(s, NBR_LAYER-1, drOp);
	for(i=0; i<NBR_LAYER-1; i++) eachObjectLayer(s, i, deO);
	eachObjectLayer(s, NBR_LAYER-1, deOp);
	MLV_update_window();
}
