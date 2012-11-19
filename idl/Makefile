CC = gcc
CFLAGS = -w
FLX = flex
FLX_FLAGS =
BSN = bison
BSN_FLAGS =

SRC = tools.c \
	  server_gen.c \
	  client_gen.c
OBJ = $(SRC:.c=.o)

TARGET = idl-compiler

all: lexic.yy.c parser.tab.c $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $^

lexic.yy.c:
	$(FLX) $(FLX_FLAGS) -o lexic.yy.c lexic.l

parser.tab.c:
	$(BSN) $(BSN_FLAGS) -d parser.y

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o *.yy.c *.tab.c *.tab.h *~ includes/*~
