all: test_config

CC = gcc
CFLAGS = -g -Wall
INCLUDE = .

MYLIB = oper_config.a

test_config: test_config.o $(MYLIB)
	$(CC) $(CFLAGS) -o test_config test_config.o $(MYLIB)
$(MYLIB): $(MYLIB)(oper_config.o)
test_config.o: test_config.c oper_config.h
oper_config.o: oper_config.c oper_config.h

clean:
	rm *.o



