INC1 = include/
SRCDIR = src/merge_sort.c
INCDIRS = -I${INC1}
LIBOPENMP = -fopenmp
CFLAGS = ${INCDIRS} ${LIBOPENMP}
build: 
	gcc ${CFLAGS} -o test.exe -std=c99 ${SRCDIR}

all: 
	make build
	./test.exe