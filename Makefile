# Make file of jackcompiler
# for Hack computer
SOURCES=jackcompiler.o jacktokenizer.o compilationengine.o
CC=gcc

all: $(SOURCES) link

clean:
	-rm *.o jackcompiler
link:
	gcc -o jackcompiler $(SOURCES)

%.o: %.c
	$(CC) -c $< -o $@ 
