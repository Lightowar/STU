CFLAGS = -g -O2 -Wall -Werror `pkg-config --cflags MLV`
LDLIBS = `pkg-config --libs MLV`

main: main.o MLVengine/all
	gcc -o main main.o MLVengine/*.o $(CFLAGS) $(LDLIBS)

main.o: main.c carac.c MLVengine/scene.o
	gcc -o main.o -c main.c $(CFLAGS)

MLVengine/all:
	make -C MLVengine all

clean:
	rm -rf *.o
