#include <stdio.h>
#include <math.h>

RenderElem* readDir(char* dir) {
	
	RenderElem* img=(RenderElem*)malloc(sizeof(RenderElem));
	strcpy(img->str, dir);
	
	char str[128] = {0};
	char c[128] = {0};
	strcpy(str, dir);
	strcat(str, "/descriptor.data");
	FILE* file = fopen(str, "r");
	int i;
	if (fscanf(file, "%lf\n", &(img->timeMax))==EOF) {
		fprintf(stdout, "error\n");
		return NULL;
	}
	if (fscanf(file, "%d\n", &i)==EOF) {
		fprintf(stdout, "error\n");
		return NULL;
	}
	if (i>=0) {
		img->nbrImg=i;
		img->elem=(MLV_Image**)malloc(sizeof(MLV_Image*)*i);
		int j;
		for (j=0; j<i; j++) {
			if (fscanf(file, "%s\n", c)==EOF) {
				fprintf(stdout, "error\n");
				return NULL;
			}
			strcpy(str, dir);
			strcat(str, "/");
			strcat(str, c);
			img->elem[j]=MLV_load_image(str);
		}
	} else {
		int x, y;
		if (fscanf(file, "%d %d\n", &x, &y)==EOF) {
			fprintf(stdout, "error\n");
			return NULL;
		}
		img->nbrImg=i;
		if (fscanf(file, "%s\n", c)==EOF) {
			fprintf(stdout, "error\n");
			return NULL;
		}
		strcpy(str, dir);
		strcat(str, "/");
		strcat(str, c);
		img->elem=(MLV_Image**)malloc(sizeof(MLV_Image*)*x*y);
		img->nbrImg=x*y;
		MLV_Image* image = MLV_load_image(str);
		int ii, jj, width, height;
		MLV_get_image_size(image, &width, &height);
		width=width/x;
		height=height/y;
		for (jj=0; jj<y; jj++) {
			for (ii=0; ii<x; ii++) {
				img->elem[ii+jj*y] = MLV_copy_partial_image(image, width*ii, height*jj, width, height);
			}
		}
		MLV_free_image(image);
		
	}
	fclose(file);
	if (img->nbrImg != 0) {
		MLV_get_image_size(img->elem[0], &img->decX, &img->decY);
	}
	else {
		img->decX = 0;
		img->decY = 0;
	}
	img->decX/=2;
	img->decY/=2;
	img->time=0;
	return img;
}

MLV_Image* getImageRender(RenderElem* img, int time) {
	MLV_Image* i = img->elem[(int)(time/img->timeMax)%img->nbrImg];
	return i;
}
