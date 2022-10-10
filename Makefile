CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

9cc: $(OBJS)
	$(CC) -o 9cc $(OBJS) $(LDFLAGS)


$(OBJS): 9cc.h

test: 9cc
	./9cc test/test.c > tmp.s && cc -o tmp tmp.s test/print.c && ./tmp

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean