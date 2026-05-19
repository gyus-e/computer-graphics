CC = clang
LDLIBS = -lGL -lGLU -lglut -lm

all: a.out a2.out

a2.out: main2.o utils.o
	$(CC) main2.o utils.o $(LDLIBS) -o a2.out

a.out: main.o utils.o
	$(CC) main.o utils.o $(LDLIBS)

main2.o: main2.c utils.h
	$(CC) -c main2.c

main.o: main.c utils.h
	$(CC) -c main.c

utils.o: utils.c utils.h
	$(CC) -c utils.c

clean:
	rm -f a.out a.exe *.o