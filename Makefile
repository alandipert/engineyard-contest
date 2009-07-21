all: osx

osx: engine2.c
	gcc -fast -lcrypto -o engine2 engine2.c

linux: engine2.c
	gcc -O4 -lcrypto -o engine2 engine2.c

check: check.c
	gcc -lcrypto -o check check.c

clean:
	rm -f *.o engine2
