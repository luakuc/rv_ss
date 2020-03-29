CC 		:= gcc
CFLAGS	:= -std=c11 -I../ -I../include -ggdb3

all: fdt

fdt: fdt.o ../fdt.o ../string.o ../endian.o dummy_mm.o
	gcc $(CFLAGS) $^ -o $@

clean:
	make -C ../ clean
	rm *.o
