CC = clang
LDLIBS = -lGL -lGLU -lglut -lm

a.out: main.o checkerror.o
	$(CC) main.o checkerror.o $(LDLIBS)

main.o: main.c
	$(CC) -c main.c

checkerror.o: checkerror.c checkerror.h
	$(CC) -c checkerror.c

clean:
	rm -f a.out a.exe *.o