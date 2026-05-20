CC = clang
LDLIBS = -lGL -lGLU -lglut -lm

all: a.out a2.out

a2.out: main2.o
	$(CC) main2.o $(LDLIBS) -o a2.out

main2.o: main2.c
	$(CC) -c main2.c

a.out: main.o
	$(CC) main.o $(LDLIBS)

main.o: main.c
	$(CC) -c main.c

clean:
	rm -f *.out *.exe *.o