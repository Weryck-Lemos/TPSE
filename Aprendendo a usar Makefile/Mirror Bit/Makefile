all: bin/mirror
	./bin/mirror

bin/mirror: obj/main.o obj/mirror.o
	gcc -o bin/mirror obj/main.o obj/mirror.o

obj/main.o: src/main.c src/mirror.h
	mkdir -p obj bin
	gcc -c src/main.c -o obj/main.o  

obj/mirror.o: src/mirror.c src/mirror.h
	mkdir -p obj bin
	gcc -c src/mirror.c -o obj/mirror.o

clean:
	rm -rf obj/* bin/*