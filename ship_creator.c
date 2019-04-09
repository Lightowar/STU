#include <stdio.h>

typedef struct shipDescriptor {
	int pv;
	char img[255];
	int moveSet;
	int missile;
	Vector hitbox;
} ShipDescriptor;

ShipDescriptor descriptor[255];

void readFile(char* file) {
	FILE* f = fopen(file, "r");
	int i=0;
	int x, y;
	while (fscanf(f, "%d %s %d %d %d-%d\n", 
		&descriptor[i].pv, descriptor[i].img, 
		&descriptor[i].moveSet, &descriptor[i].missile, &x, &y) != EOF) 
	{
        descriptor[i].hitbox = newVector(x, y);
		i++;
	}
	fclose (f);
	for (; i<255; i++) {
		descriptor[i] = (ShipDescriptor){0, "", 0, 0, newVector(0, 0)};
	}
}

Object* createSpaceShipe (int id) {
	Object* o = newObject();
	setDrawType(o, DRAW_IMAGE);
	setDrawString(o, descriptor[id].img);
	setHitboxType(o, HITBOX_RECT);
	setHitbox(o, descriptor[id].hitbox);
	setCarac(o, newCarac(descriptor[id].pv, descriptor[id].pv, 0, descriptor[id].moveSet, NULL));
	return o;
}
