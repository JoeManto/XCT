
all: clean comp
	gcc ./build/xct
comp:
	gcc xct.c -o build/xct
clean:
	rm *.o xct
