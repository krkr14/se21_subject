main.exe : main.o f1.o rules.o alphaBeta.o
	cc -o main.exe main.o f1.o rules.o alphaBeta.o -lncurses
main.o : main.c
	cc -c main.c
f1.o : f1.c
	cc -c f1.c
rules.o : rules.c
	cc -c rules.c
alphaBeta.o : alphaBeta.c
	cc -c alphaBeta.c