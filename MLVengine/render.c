#include "render.h"

#include <MLV/MLV_all.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include "vector.h"
#include "hashset.h"

#define MAX_TEXT_SIZE 4096

typedef struct renderElem RenderElem;

struct renderElem {
	MLV_Image** elem;
	int nbrImg;
	char str[255];
	int decX;
	int decY;
	double timeMax;
    int size;
    MLV_Font* font;
};

#include "read_dir.c"

Hashset* renderMap=NULL;
Hashset* fontMap=NULL;

unsigned int hashStr(char* str) {
	if (str[0]=='\0') {
		return 0;
	}
	int h = str[0]+hashStr(str+1)*31;
	return h;
}
unsigned int hashElem(void* elem) {
	RenderElem* e = (RenderElem*)elem;
	char* str = (char*)e->str;
	unsigned int h = (hashStr(str)+e->size)%HASHSET_SIZE;
	return h;
}
int equElem(void* elem1, void* elem2) {
	if (elem1==NULL || elem2==NULL) return (elem1==NULL && elem2==NULL);
	return (strcmp(((RenderElem*)elem1)->str, ((RenderElem*)elem2)->str)==0 && 
		((RenderElem*)elem1)->size == ((RenderElem*)elem2)->size);
}

RenderElem* getImageFromIndex(char* str) {
	RenderElem r;
	initElem(&r);
	strcpy(r.str, str);
	return getFromHashset(renderMap, &r);
}

RenderElem* getFontFromIndex(char* str, int size) {
	RenderElem r;
	initElem(&r);
	r.size = size;
	strcpy(r.str, str);
	return getFromHashset(fontMap, &r);
}

void renderInit(Scene* s) {
	MLV_create_window("test", "test", getXInt(*getSize(s)), getYInt(*getSize(s)));
	if (renderMap == NULL) renderMap=newHashset();
	setHashHashset(renderMap, hashElem);
	setEqualHashset(renderMap, equElem);
	if (fontMap == NULL) fontMap=newHashset();
	setHashHashset(fontMap, hashElem);
	setEqualHashset(fontMap, equElem);
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
MLV_Color getOjectColor(Object* o) {
	ObjColor c = getColor(o);
	return MLV_rgba(c.r, c.g, c.b, c.a);
}

void drawTextSimple(int x, int y, char line[], MLV_Font* font, MLV_Color color) {
	if (font==NULL)
		MLV_draw_text(x, y, line, color);
	else
		MLV_draw_text_with_font(x, y, line, font, color);
}

void drawText (Object* o, MLV_Font* font) {
	MLV_Color c = getOjectColor(o);
	int x = getX(*getPos(o));
	int y = getY(*getPos(o));
	int sizeY = getY(*getDrawScale(o));
	char line[MAX_TEXT_SIZE] = "";
	char* str = getDrawText(o);
	int pos = 0;
	int posLine=0;
	int prec=0;
	int max = getX(*getTextBox(o));
	while (str[pos]!='\0') {
		if (str[pos]==' ') {
			prec=posLine;
		}
		line[posLine]=str[pos];
		line[posLine+1]='\0';
		posLine++;
		pos++;
		if (str[pos]=='\n') {
			drawTextSimple(x, y, line, font, c);
			y+=sizeY;
			prec=0;
			posLine=0;
			line[0]='\0';
			pos++;
		} else {
			if (max>0) {
				int sizeX, tmp;
				if (font==NULL) {
					MLV_get_size_of_text(line, &sizeX, &tmp);
				} else {
					MLV_get_size_of_text_with_font(line, &sizeX, &tmp, font);
				}
				if (sizeX>=max) {
					line[prec]='\0';
					pos-=(posLine-prec);
					pos++;
					drawTextSimple(x, y, line, font, c);
					y+=sizeY;
					prec=0;
					posLine=0;
					line[0]='\0';
				}
			}
		}
		if (str[pos]=='\0') {
			line[posLine]='\0';		
			drawTextSimple(x, y, line, font, c);
		}
	}
}

void drawObject(Scene* s, Object* o, int camEnable, int time) {
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
	if (str!=NULL && str[0]!='\0' && getDrawType(o)==DRAW_IMAGE) {
		RenderElem* img = getRender(o);
		if (img==NULL) {
			img = getImageFromIndex(str);
			if (img==NULL) {
				img=readDir(str);
				addInHashset(renderMap, img);
			}
			setRender(o, img);
		}
		MLV_draw_image(getImageRender(img, time), x-img->decX, y-img->decY);
	}
	if (getDrawType(o)==DRAW_OVAL) {
		Vector* scale=getDrawScale(o);
		MLV_draw_filled_ellipse(x, y, getX(*scale), getY(*scale), getOjectColor(o));
	}
	if (getDrawType(o)==DRAW_RECT) {
		Vector* scale=getDrawScale(o);
		MLV_draw_filled_rectangle(x, y, getX(*scale), getY(*scale), getOjectColor(o));
	}
	if (getDrawType(o)==DRAW_TEXT) {
		int size = getYInt(*getDrawScale(o));
		RenderElem* font = getRender(o);
		if (font==NULL) {
			font = getFontFromIndex(str, size);
			if (font==NULL) {
				font=(RenderElem*)malloc(sizeof(RenderElem));
				initElem(font);
				strcpy(font->str, str);
				if (strcmp(font->str, "")==0) font->font = NULL;
				else font->font = MLV_load_font(str, size);
				font->size = size;
				addInHashset(fontMap, font);
			}
			setRender(o, font);
		}
		drawText(o, font->font);
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
	void drO(Object* o) {return drawObject(s, o, 1, getFrameForAnim(o));};
	void drOp(Object* o) {return drawObject(s, o, 0, getFrameForAnim(o));};
	MLV_clear_window( MLV_COLOR_BLACK );
	int i;
	for(i=0; i<NBR_LAYER-1; i++) eachObjectLayer(s, i, drO);
	eachObjectLayer(s, NBR_LAYER-1, drOp);
	MLV_update_window();
}

void debugScene(Scene* s, double time) {
	void drO(Object* o) {return drawObject(s, o, 1, getFrameForAnim(o));};
	void deO(Object* o) {return debugObject(s, o, 1, getFrameForAnim(o));};
	void drOp(Object* o) {return drawObject(s, o, 0, getFrameForAnim(o));};
	void deOp(Object* o) {return debugObject(s, o, 0, getFrameForAnim(o));};
	MLV_clear_window( MLV_COLOR_BLACK );
	int i;
	for(i=0; i<NBR_LAYER-1; i++) eachObjectLayer(s, i, drO);
	eachObjectLayer(s, NBR_LAYER-1, drOp);
	for(i=0; i<NBR_LAYER-1; i++) eachObjectLayer(s, i, deO);
	eachObjectLayer(s, NBR_LAYER-1, deOp);
	MLV_update_window();
}
