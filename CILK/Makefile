INC1 = include/
SRCDIR = src/merge_sort_cilk.c
INCDIRS = -I${INC1}
LIBOPENMP = -fopenmp
LIBCILK = -fcilkplus
CFLAGS = ${INCDIRS} ${LIBOPENMP} ${LIBCILK}
build_cilk: 
	gcc ${CFLAGS} -o test.exe -std=c99 ${SRCDIR}

all_cilk: 
	make build_cilk
	./test.exe