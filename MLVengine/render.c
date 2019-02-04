
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

Maillon* map[MAP_SIZE];

int hash(char* str) {
	if (str[0]==0) {
		return 0;
	}			
	int h = str[0]+hash(str+1);
	if (h>=MAP_SIZE) {
		h-=MAP_SIZE;
	}
	return h;
}

Maillon* getImage(char* str) {
	int h = hash(str);
	Maillon* m = map[h];
	while (m!=NULL) {
		if (strcmp(m->str, str)==0) {
			return m;
		}
		m=m->next;
	}
	m=map[h];
	Maillon* m2 = (Maillon*)malloc(sizeof(Maillon));
	m2->elem=MLV_load_image(str);
	m2->str=str;
	m2->next=m;
	int x, y;
	MLV_get_image_size(m2->elem, &x, &y);
	m2->decX=x/2;
	m2->decY=y/2;
	map[h]=m2;
	return m2;
}

void renderInit() {
	MLV_create_window("test", "test", 400, 400);
}

void drawObject(Object** o) {
	Vector* v = getPos(*o);
	char* str = getDrawString(*o);
	if (str!=NULL && getDrawType(*o)==DRAW_IMAGE) {
		Maillon* img = getImage(str);
		MLV_draw_image(img->elem, getXInt(v)-img->decX, getYInt(v)-img->decY);
	} else {
		MLV_draw_filled_circle(getXInt(v), getYInt(v), 3, MLV_COLOR_RED);
	}
}

void renderScene(Scene* s) {
	MLV_clear_window( MLV_COLOR_BLACK );
	eachObjects(s, drawObject);
	MLV_update_window();
}
