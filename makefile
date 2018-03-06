main.o: main.c hmap.o Structs.o Graph.o
	gcc -o main.c

Graph.o: Graph.c Graph.h
	gcc -c Graph.c

Structs.o: Structs.c Structs.h
	gcc -c Structs.c

hmap.o: hmap.c hmap.h
	gcc -c hmap.c

fakemake: main.o Graph.o Structs.o hmap.o
	gcc -o fakemake main.o Graph.o Structs.o hmap.o

clean:
	rm fakemake main.o Graph.o Structs.o hmap.o
