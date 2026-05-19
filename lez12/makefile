CC = clang
LDLIBS = -lGL -lGLU -lglut -lm

all: a.out es3.out

es3.out: es3.o utils.o
	$(CC) es3.o utils.o $(LDLIBS) -o es3.out

a.out: main.o utils.o
	$(CC) main.o utils.o $(LDLIBS)

es3.o: es3.c utils.h
	$(CC) -c es3.c

main.o: main.c utils.h
	$(CC) -c main.c

utils.o: utils.c utils.h
	$(CC) -c utils.c

clean:
	rm -f a.out a.exe *.o