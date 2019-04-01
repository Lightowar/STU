Object* createMissile (Scene* s, float x, float y, float vitX, float vitY) {
	Object* m=newObject();
	setX(getVit(m), vitX);
	setY(getVit(m), vitY-(600/FPS));
	setX(getPos(m), x);
	setY(getPos(m), y-20);
	setKillTime(m, 2*FPS);
	setDrawType(m, DRAW_IMAGE);
	setDrawString(m, "DATA/laser");
	Vector* hit=getHitbox(m);
	setHitboxType(m, HITBOX_RECT);
	setX(hit, 10);
	setY(hit, 15);
	addObject(s, m, 1);
	return m;
}

/*Object* createEnemy () {
	Object* o = newObject();
	setDrawType(o, DRAW_IMAGE);
	setDrawString(o, "DATA/ss_enemy");
	setHitboxType(o, HITBOX_RECT);
	Vector* hit=getHitbox(o);
	setX(hit, 40);
	setY(hit, 25);
	setCarac(o, newCarac(0, 0, 0, 0, NULL));
	return o;
}
*/
