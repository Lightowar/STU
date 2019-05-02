Object* createMissile (Object* o, Vector vit, Vector dec) {
	Object* m=createSpaceShipe(3);
	setVit(m, addVector(*getVit(o), vit));
	setPos(m, addVector(*getPos(o), dec));
	setKillTime(m, 2*FPS);
	addObject(s, m, 1);
	((Carac*)getCarac(m))->attack=((Carac*)getCarac(o))->attack;
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
