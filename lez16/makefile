CC = clang
LDLIBS = -lGL -lGLU -lglut -lm

all: a.out

a.out: main.o texture.o
	$(CC) main.o texture.o $(LDLIBS)

main.o: main.c
	$(CC) -c main.c

texture.o: texture.c
	$(CC) -c texture.c

clean:
	rm -f *.out *.exe *.o