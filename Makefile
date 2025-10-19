PWD := $(shell pwd)

all:
	gcc -g -Wall -c $(PWD)/src/parser.c -o $(PWD)/bin/parser.o -I$(PWD)/includes
	gcc -g -Wall -c $(PWD)/src/stringUtils.c -o $(PWD)/bin/stringUtils.o -I$(PWD)/includes
	gcc -g -Wall -c $(PWD)/src/sequencer.c -o $(PWD)/bin/sequencer.o -I$(PWD)/includes
	gcc -g -Wall ./main.c -o $(PWD)/bin/Tester $(PWD)/bin/parser.o $(PWD)/bin/stringUtils.o $(PWD)/bin/sequencer.o
	rm $(PWD)/bin/stringUtils.o $(PWD)/bin/sequencer.o $(PWD)/bin/parser.o

