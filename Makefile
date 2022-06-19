all: build

build:
	gcc -g main.c LinkedList.c tree.c -o sd_fs

clean:
	rm *.o sd_fs

run:
	./sd_fs
