FILE* level;

void openLevel (const char *pathname) {
	level=fopen(pathname, "r");
}

void closeLevel () {
	fclose(level);
}

char readed[TEXT_SIZE];

int readLine(int* type, void** object, int* time, int* prec) {
	int ret;
	int id, x, y;
	if (fscanf(level, "%d %d %d ", time, prec, type)==EOF) {
		return -2;
	}
	switch (*type) {
		case 1:
			ret = fscanf(level, "%d %d %d\n", &id, &x, &y);
			*object=createSpaceShipe(id);
			setX(getPos((Object*)*object), x);
			setY(getPos((Object*)*object), y);
			break;
		case 2:
			ret = fscanf(level, "%[^\n]\n", readed);
			*object=readed;
			int i;
			for (i=0; readed[i]!='\0'; i++) {
				if (readed[i]=='\\') readed[i]='\n';
			}
			break;
		case 3:
			ret = fscanf(level, "%[^\n]\n", readed);
			if (readed[0]=='.' && readed[1]=='\0') readed[0]='\0';
			*object=readed;
			break;
		default:
			return -2;
			break;
	}
	if (ret==EOF) return ret;
	return 1;
}
