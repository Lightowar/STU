CFLAGS = -g -O2 -Wall -Werror `pkg-config --cflags MLV`
LDLIBS = `pkg-config --libs MLV`

all: vector.o scene.o object.o render.o
	
vector.o: vector.c
	gcc -o vector.o -c vector.c $(CFLAGS)

scene.o: scene.c object.h
	gcc -o scene.o -c scene.c $(CFLAGS)

object.o: object.c vector.h
	gcc -o object.o -c object.c $(CFLAGS)

render.o: render.c scene.h
	gcc -o render.o -c render.c $(CFLAGS) $(LDLIBS)

clean:
	rm -rf *.o
