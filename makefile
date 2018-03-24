final_project: main.o pgms.o segment.o pgmfiles.o
	gcc main.o pgms.o segment.o pgmfiles.o -o final_project

main.o: main.c
	gcc -c main.c
	
segment.o: segment.h segment.c
	gcc -c segment.c

pgmfiles.o: pgmfiles.c pgmfiles.h
	gcc -c pgmfiles.c

pgms.o: pgms.c pgms.h
	gcc -c pgms.c

clean:
	rm main.o segment.o pgmfiles.o pgms.o
