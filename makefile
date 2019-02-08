CFLAGS = -g -O2 -Wall -Werror `pkg-config --cflags MLV`
LDLIBS = `pkg-config --libs MLV`

main: main.o MLVengine/vector.o MLVengine/object.o MLVengine/scene.o MLVengine/render.o MLVengine/set.o
	gcc -o main main.o MLVengine/*.o $(CFLAGS) $(LDLIBS)

main.o: main.c MLVengine/scene.o
	gcc -o main.o -c main.c $(CFLAGS)

MLVengine/vector.o:
	make -C MLVengine vector.o

MLVengine/object.o:
	make -C MLVengine object.o

MLVengine/scene.o:
	make -C MLVengine scene.o

MLVengine/render.o:
	make -C MLVengine render.o

MLVengine/set.o:
	make -C MLVengine set.o

clean:
	rm -rf *.o
