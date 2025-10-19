PWD := $(shell pwd)

all:
	gcc -g -Wall -c $(PWD)/src/stringUtils.c -o $(PWD)/bin/stringUtils.o -I$(PWD)/includes
	gcc -g -Wall -c $(PWD)/src/sequencer.c -o $(PWD)/bin/sequencer.o -I$(PWD)/includes
	gcc -g -Wall $(PWD)/bin/stringUtils.o $(PWD)/bin/sequencer.o -o $(PWD)/bin/Sequencer
	rm $(PWD)/bin/stringUtils.o $(PWD)/bin/sequencer.o

sequencer: $(PWD)/src/sequencer.c
	gcc -g -Wall -c $(PWD)/src/stringUtils.c -o $(PWD)/bin/stringUtils.o -I$(PWD)/includes
	gcc -g -Wall -c $(PWD)/src/sequencer.c -o $(PWD)/bin/Sequencer.o -I$(PWD)/includes
	gcc -g -Wall $(PWD)/bin/stringUtils.o $(PWD)/bin/Sequencer.o -o $(PWD)/bin/Sequencer