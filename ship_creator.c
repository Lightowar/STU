#include <stdio.h>

typedef struct shipDescriptor {
	int pv;
	char img[255];
	int moveSet;
	int missile;
	int attack;
	Vector hitbox;
} ShipDescriptor;

ShipDescriptor descriptor[255];

void readFile(char* file) {
	FILE* f = fopen(file, "r");
	int x, y;
	int id;
	ShipDescriptor sd;
	while (fscanf(f, "%d %d %s %d %d %d %d-%d\n", 
		&id, &sd.pv, sd.img, 
		&sd.moveSet, &sd.missile, &sd.attack, &x, &y) != EOF) 
	{
		descriptor[id] = sd;
        descriptor[id].hitbox = newVector(x, y);
	}
	fclose (f);
}

Object* createSpaceShipe (int id) {
	Object* o = newObject();
	setDrawType(o, DRAW_IMAGE);
	setDrawString(o, descriptor[id].img);
	setHitboxType(o, HITBOX_RECT);
	setHitbox(o, descriptor[id].hitbox);
	setCarac(o, newCarac(descriptor[id].pv, descriptor[id].pv, descriptor[id].moveSet, descriptor[id].missile, 100, descriptor[id].attack));
	return o;
}
