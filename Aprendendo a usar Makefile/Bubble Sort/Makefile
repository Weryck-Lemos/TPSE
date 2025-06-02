all: bin/bubble
	./bin/bubble

bin/bubble: obj/main.o obj/bubble.o
	gcc -o bin/bubble obj/main.o obj/bubble.o

obj/main.o: src/main.c src/bubble.h
	gcc -c src/main.c -o obj/main.o

obj/bubble.o: src/bubble.c src/bubble.h
	gcc -c src/bubble.c -o obj/bubble.o

clean:
	rm -rf obj/* bin/*
