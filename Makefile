myshell: myshell.o builtin.o argparse.o
	gcc -g -o myshell myshell.o builtin.o argparse.o

builtin.o: builtin.c builtin.h
	gcc -g -c builtin.c
argparse.o: argparse.c argparse.h
	gcc -g -c argparse.c
myshell.o: myshell.c
	gcc -g -c myshell.c

clean:
	rm -f *.o *.bak *~* *.exe
