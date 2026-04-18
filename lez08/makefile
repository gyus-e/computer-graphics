CC = clang
LDLIBS = -lGL -lGLU -lglut -lm

a.out: main.o utils.o cJSON.o
	$(CC) main.o utils.o cJSON.o $(LDLIBS)

main.o: main.c utils.h cJSON.h
	$(CC) -c main.c

utils.o: utils.c utils.h
	$(CC) -c utils.c

cJSON.o: cJSON.c cJSON.h
	$(CC) -c cJSON.c

clean:
	rm -f a.out a.exe *.o