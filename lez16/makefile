CC = clang
LDLIBS = -lGL -lGLU -lglut -lm

all: a.out a2.out

a2.out: main2.o texture.o
	$(CC) main2.o texture.o $(LDLIBS) -o a2.out

main2.o: main2.c
	$(CC) -c main2.c

a.out: main.o texture.o
	$(CC) main.o texture.o $(LDLIBS)

main.o: main.c
	$(CC) -c main.c

texture.o: texture.c
	$(CC) -c texture.c

clean:
	rm -f *.out *.exe *.o