CFLAGS = -g -O2 -Wall -Werror `pkg-config --cflags MLV`
LDLIBS = `pkg-config --libs MLV`

all: vector.o scene.o object.o render.o set.o hashset.o
	
vector.o: vector.c
	gcc -o vector.o -c vector.c $(CFLAGS)

scene.o: scene.c object.o set.o
	gcc -o scene.o -c scene.c $(CFLAGS)

object.o: object.c vector.o hashset.o
	gcc -o object.o -c object.c $(CFLAGS)

render.o: render.c scene.o read_dir.c hashset.o
	gcc -o render.o -c render.c $(CFLAGS) $(LDLIBS)

set.o: set.c
	gcc -o set.o -c set.c $(CFLAGS)
	
hashset.o: hashset.c set.o
	gcc -o hashset.o -c hashset.c $(CFLAGS)
	
#hashmap.o: hashmap.c hashset.o
#	gcc -o hashmap.o -c hashmap.c $(CFLAGS)

clean:
	rm -rf *.o
