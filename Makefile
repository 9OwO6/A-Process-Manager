.phony all:
all: pman

pman: main.c linked_list.c
	gcc -Wall main.c -o pman

.PHONY clean:
clean:
	-rm -rf *.o *.exe
#  linked_list.c