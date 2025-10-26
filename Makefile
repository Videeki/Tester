PWD := $(shell pwd)
RED := "\e[38;2;255;0;0m"
GREEN := "\e[38;2;0;255;0m"
BLUE := "\e[38;2;0;0;255m"
RESET := "\e[0m"


all:
	gcc -g -Wall -c $(PWD)/src/parser.c -o $(PWD)/bin/parser.o -I $(PWD)/includes
	gcc -g -Wall -c $(PWD)/src/stringUtils.c -o $(PWD)/bin/stringUtils.o -I $(PWD)/includes
	gcc -g -Wall -c $(PWD)/src/hashCalc.c -o $(PWD)/bin/hashCalc.o -I $(PWD)/includes
	gcc -g -Wall -c $(PWD)/src/sequencer.c -o $(PWD)/bin/sequencer.o -I $(PWD)/includes
	gcc -g -Wall ./main.c -o $(PWD)/bin/Tester $(PWD)/bin/parser.o $(PWD)/bin/stringUtils.o $(PWD)/bin/hashCalc.o $(PWD)/bin/sequencer.o
	rm $(PWD)/bin/stringUtils.o $(PWD)/bin/sequencer.o $(PWD)/bin/parser.o $(PWD)/bin/hashCalc.o

stringUtilsTest:
	gcc -Wall $(PWD)/test/stringUtilsTest.c -o $(PWD)/bin/stringUtilsTest $(PWD)/src/stringUtils.c -I $(PWD)/includes
	($(PWD)/bin/stringUtilsTest "    alma" 0 && echo $(GREEN)"PASS"$(RESET)) || echo $(RED)"FAIL"$(RESET)
	($(PWD)/bin/stringUtilsTest "    alma" 1 && echo $(GREEN)"PASS"$(RESET)) || echo $(RED)"FAIL"$(RESET)
	($(PWD)/bin/stringUtilsTest "    alma" 4 && echo $(GREEN)"PASS"$(RESET)) || echo $(RED)"FAIL"$(RESET)
	($(PWD)/bin/stringUtilsTest "    alma" 5 && echo $(GREEN)"PASS"$(RESET)) || echo $(RED)"FAIL"$(RESET)

hashCalcTest:
	gcc -Wall $(PWD)/test/hashCalcTest.c -o $(PWD)/bin/hashCalcTest $(PWD)/src/hashCalc.c -I $(PWD)/includes
	($(PWD)/bin/hashCalcTest InitPS 792116435 && echo $(GREEN)"PASS"$(RESET)) || echo $(RED)"FAIL"$(RESET)
	($(PWD)/bin/hashCalcTest InitGateway 993770905 && echo $(GREEN)"PASS"$(RESET)) || echo $(RED)"FAIL"$(RESET)

clearTests:
	rm $(PWD)/bin/*Test