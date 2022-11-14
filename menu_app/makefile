INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -Wall -g -Werror
CFLAGS += -I$(INCLDIR)
LINKEXTRA = -lpthread

OBJDIR = obj

_DEPS = debugger.h database.h linkedList.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = main.o debugger.o database.o linkedList.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))


$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

all: main

main: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LINKEXTRA)

.PHONY: clean
clean: 
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~ *.a debug_*.txt
