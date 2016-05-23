.PHONY: seq omp clean check
SRC		= func.c
CFLAGS	= -O3 -lm
PROG_SEQ	= lab4_seq
PROG_OMP	= lab4_omp
OPENMP = OPENMP  

ifdef GPROF
CFLAGS+= -pg
endif

ifdef MTRACE
CFLAGS+= -DMTRACE
endif

seq:
	gcc -o $(PROG_SEQ) $(CFLAGS) main.o $(SRC) 
omp:
	gcc -o $(PROG_OMP) $(CFLAGS) -D$(OPENMP) -fopenmp main_omp.o $(SRC) 
clean: 
	rm -f lab4_seq lab4_omp dump/*
check:
	@diff --brief ./compare/correct1.txt ./dump/Func1.txt || true
	@diff --brief ./compare/correct2.txt ./dump/Func2.txt || true
	@diff --brief ./compare/correct3.txt ./dump/Func3.txt || true
	@diff --brief ./compare/correct4.txt ./dump/Func4.txt || true
	@diff --brief ./compare/correct5.txt ./dump/Func5.txt || true
checkmem:
	@mtrace $(PROG) mtrace.out || true



