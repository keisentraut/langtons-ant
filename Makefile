all: ant

clean:
	rm -f *.o ant

ant: ant.c
	gcc -O3 -o ant ant.c
	strip ant
