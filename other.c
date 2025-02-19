void randomiseStar(Object* o) {
	Vector* scale=getDrawScale(o);
	Vector* pos=getPos(o);
	Vector* vit=getVit(o);
	int rand=randomInt(0, 100);
	if (rand<=1) {
		setY(pos, randomDouble(MIN_Y, MAX_Y));
		setX(pos, MIN_X);
		setX(vit, randomDouble(5, 20)*60/FPS);
		setY(vit, randomDouble(0.1, 1.5)*60/FPS);
		setDrawType(o, DRAW_IMAGE);
		setDrawString(o, "DATA/cat");
	} else {
		setX(pos, randomDouble(MIN_X, MAX_X));
		setY(pos, MIN_Y);
		setX(vit, 0);
		setY(vit, randomDouble(0.3, 2)*60/FPS);
		setX(scale, randomDouble(1, 2.5));
		setY(scale, randomDouble(1, 2.5));
		setDrawType(o, DRAW_OVAL);
		ObjColor c = {255, 255, 200, 200};
		setColor(o, c);
		if (rand<=5) {
			setDrawType(o, DRAW_IMAGE);
			setDrawString(o, "DATA/star");
		}
	}
}

void moveStar(Object* o) {
	Vector* v=getPos(o);
	float y=getY(*v);
	float x=getX(*v);
	if (y>MAX_Y || y<MIN_Y || x>MAX_X || x<MIN_X) {
		randomiseStar(o);
	}
}

int dammage(Object* missile, Object* spaceShip) {
	Carac* c = (Carac*)getCarac(spaceShip);
	c->lifePoint-=((Carac*)getCarac(missile))->attack;
	if (c->lifePoint<0) c->lifePoint=0;
	if (((Carac*)getCarac(missile))->lifePoint<0) destroyObject((Object*)(missile), 1);
	if (c->lifePoint<=0) {
		return 1;
	}
	return 0;
}

void destroySpaceShip(Object* spaceShip) {
	setDrawString(spaceShip, "DATA/explosion");
	setKillTime(spaceShip, 80);
}

int collision (Object* o1, Object* o2) {
	if (!touch(o1, o2)) return 0;
	Vector v1 = *getVit(o2);
	Vector v2 = *getVit(o1);
	if (((Carac*)getCarac(o1))->maxLifePoint < 500)
		setVit(o1, addVector(*getVit(o1), multiplie(normalize(newVector(v1.x-v2.x, v1.y-v2.y)), 15)));
	if (((Carac*)getCarac(o2))->maxLifePoint < 500)
		setVit(o2, addVector(*getVit(o2), multiplie(normalize(newVector(v2.x-v1.x, v2.y-v1.y)), 15)));
	return 1;
}
