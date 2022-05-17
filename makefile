FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS = -lm

all: cluster

clean:
	rm -rf *.o cluster

cluster: main.o stack.o group.o array_spmat.o errors.o parser.o alg_imp.o
	gcc $(FLAGS) main.o stack.o group.o array_spmat.o errors.o parser.o alg_imp.o -o cluster $(LIBS)

main.o: main.c spmat.h group.h stack.h parser.h alg_imp.h errors.h
	gcc $(FLAGS) -c main.c

stack.o: stack.c stack.h group.h errors.h
	gcc $(FLAGS) -c stack.c

group.o: group.c group.h errors.h
	gcc $(FLAGS) -c group.c

array_spmat.o: array_spmat.c spmat.h errors.h
	gcc $(FLAGS) -c array_spmat.c
	
errors.o: errors.c errors.h 
	gcc $(FLAGS) -c errors.c
	
parser.o: parser.c parser.h errors.h group.h spmat.h stack.h
	gcc $(FLAGS) -c parser.c

alg_imp.o: alg_imp.c alg_imp.h errors.h group.h spmat.h stack.h
	gcc $(FLAGS) -c alg_imp.c
	