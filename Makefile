# Standard compile flags
CC       = gcc
CFLAGS   = -std=c99 -pthread -lnsl -O3

Prog1 = TigerC
Source1 = lineParse.c
Prog2 = TigerS
Source2 = userDatabase.c TigerS.c

Objs1 = $(patsubst %.c, %.o, $(Source1))
Objs2 = $(patsubst %.c, %.o, $(Source2))


all: ./$(Prog1) ./$(Prog2)

.SILENT:

.PHONY: test help clean

test1: $(Prog1)
	@echo " "
	@echo "TigerC files"
	@echo " "
	./$(Prog1)


test2: $(Prog2)
	@echo " "
	@echo "TigerS files"
	@echo " "
	./$(Prog2)


.c.o: # Compiles the C and O files whenever its modified and keeps it up to date
	@echo " "
	@echo "Compiling  '$*'"


$(Prog1): $(Objs1)
	@echo " "
	@echo "Linking $(Prog1)"
	$(CC) $(CFLAGS) $(Objs1) -o $(Prog1)

$(Prog2): $(Objs2)
	@echo " "
	@echo "Linking $(Prog2)"
	$(CC) $(CFLAGS) $(Objs2) -o $(Prog2)

help:
	@echo " " 
	@echo "make options: all, test, clean, help"

clean:
	-rm -f *.o
	-rm -f TigerS
	-rm -f TigerC

test:	test1 test2

%.o:%.c
	@echo " "
	@echo "Dependency change is '$*'"
	@echo " "
	$(CC) $(CFLAGS) $*.c -c

#-------------------------------------------------------------------
