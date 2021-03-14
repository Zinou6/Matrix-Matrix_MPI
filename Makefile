all:	app-hw2
app-hw2:	main.o initMatrix.o displayMatrix.o calcul.o
	mpicc main.o initMatrix.o displayMatrix.o calcul.o -o out
main.o:	main.c
	mpicc -c main.c -o main.o
initMatrix.o:	initMatrix.c
	gcc -c initMatrix.c -o initMatrix.o
displayMatrix.o:	displayMatrix.c
	gcc -c displayMatrix.c -o displayMatrix.o
calcul.o: calcul.c
	gcc -c calcul.c -o calcul.o
clean:
	rm -fr app-hw1 *.o
