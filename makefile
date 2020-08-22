
all: clean comp
	gcc ./xct
comp:
	gcc xct.c -o xct

clean:
	rm *.o xct
