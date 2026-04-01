CC = clang
LDLIBS = -lGL -lGLU -lglut -lm

a.out: main.o utils.o
	$(CC) main.o utils.o $(LDLIBS)

main.o: main.c utils.h
	$(CC) -c main.c

utils.o: utils.c utils.h
	$(CC) -c utils.c

clean:
	rm -f a.out a.exe *.o