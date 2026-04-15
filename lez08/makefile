CC = clang
LDLIBS = -lGL -lGLU -lglut -lm

a.out: main.o
	$(CC) main.o $(LDLIBS)

main.o: main.c
	$(CC) -c main.c

clean:
	rm -f a.out a.exe *.o