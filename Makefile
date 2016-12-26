all: jc

token:
	clang -Wall -g -c token.c

jc: token jc.c
	clang -g -Wall -o jc jc.c token.o

#
# Add more if necessary.
#
clean:
	rm -f *.o
	rm -f jc
