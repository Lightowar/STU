void randomiseStar(Object* o) {
	Vector* scale=getDrawScale(o);
	Vector* pos=getPos(o);
	Vector* vit=getVit(o);
	int rand=MLV_get_random_integer(0, 100);
	if (rand<=1) {
		setY(pos, MLV_get_random_double(MIN_Y, MAX_Y));
		setX(pos, MIN_X);
		setX(vit, MLV_get_random_double(5, 20)*60/FPS);
		setY(vit, MLV_get_random_double(0.1, 1.5)*60/FPS);
		setX(scale, MLV_get_random_double(1, 3));
		setY(scale, MLV_get_random_double(1, 3));
		setDrawType(o, DRAW_IMAGE);
		setDrawString(o, "DATA/cat");
	} else {
		setX(pos, MLV_get_random_double(MIN_X, MAX_X));
		setY(pos, MIN_Y);
		setX(vit, 0);
		setY(vit, MLV_get_random_double(0.3, 2)*60/FPS);
		setX(scale, MLV_get_random_double(1, 3));
		setY(scale, MLV_get_random_double(1, 3));
		setDrawType(o, DRAW_OVAL);
		setDrawString(o, "starColor");
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
