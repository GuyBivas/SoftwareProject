CC = gcc
OBJS = SPMinimaxNode.o SPMinimax.o SPFIARGame.o SPArrayList.o SPFIARParser.o SPMainAux.o main.o
EXEC = SPFIAR
GAME_TEST_OBJS = SPArrayList.o SPFIARGame.o SPFIARGameUnitTest.o
ARRAY_LIST_TEST_OBJS = SPArrayListUnitTest.o SPArrayList.o
PARSER_TEST_OBJS = SPFIARParser.o SPFIARParserUnitTest.o
MINMAXNode_TEST_OBJ = SPMinimaxNode.o SPMinimaxNodeUnitTest.o SPFIARGame.o SPArrayList.o
MINMAX_TEST_OBJ = SPMinimaxNode.o SPMinimax.o SPMinimaxUnitTest.o SPFIARGame.o SPArrayList.o
MAINAUX_TEST_OBJS = SPMainAux.o SPArrayList.o SPFIARGame.o SPMinimax.o SPMinimaxNode.o
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
UNIT_TESTS = SPFIARGameUnitTest SPArrayListUnitTest SPFIARParser SPMinimaxUnitTest\
SPMinimaxNodeUnitTest SPMainAuxUnitTest

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
SPFIARGameUnitTest: $(GAME_TEST_OBJS)
	$(CC) $(GAME_TEST_OBJS) -o $@
SPArrayListUnitTest: $(ARRAY_LIST_TEST_OBJS)
	$(CC) $(ARRAY_LIST_TEST_OBJS) -o $@
SPFIARParserUnitTest: $(PARSER_TEST_OBJS)
	$(CC) $(PARSER_TEST_OBJS) -o $@
SPMinimaxUnitTest: $(MINMAX_TEST_OBJ)
	$(CC) $(MINMAX_TEST_OBJ) -o $@
SPMinimaxNodeUnitTest: $(MINMAXNode_TEST_OBJ)
	$(CC) $(MINMAXNode_TEST_OBJ) -o $@
SPMainAuxUnitTest: $(MAINAUX_TEST_OBJS)
	$(CC) $(MAINAUX_TEST_OBJS) -o $@
SPFIARGameUnitTest.o: SPFIARGameUnitTest.c unit_test_util.h SPArrayList.h SPFIARGame.h
	$(CC) $(COMP_FLAG) -c $*.c
SPArrayListUnitTest.o: SPArrayListUnitTest.c SPArrayList.h unit_test_util.h
	$(CC) $(COMP_FLAG) -c $*.c
SPFIARParserUnitTest.o: SPFIARParserUnitTest.c SPFIARParser.h unit_test_util.h
	$(CC) $(COMP_FLAG) -c $*.c
SPMinimaxUnitTest.o: SPMinimaxUnitTest.c SPMinimax.h SPFIARGame.h SPMinimaxNode.h unit_test_util.h
	$(CC) $(COMP_FLAG) -c $*.c
SPMinimaxNodeUnitTest.o: SPMinimaxNodeUnitTest.c SPMinimaxNode.h unit_test_util.h SPFIARGame.h
	$(CC) $(COMP_FLAG) -c $*.c
SPArrayList.o: SPArrayList.h SPArrayList.c
	$(CC) $(COMP_FLAG) -c $*.c
SPFIARGame.o: SPFIARGame.c SPFIARGame.h SPArrayList.h
	$(CC) $(COMP_FLAG) -c $*.c
SPFIARParser.o: SPFIARParser.c SPFIARParser.h
	$(CC) $(COMP_FLAG) -c $*.c
SPMinimaxNode.o: SPMinimaxNode.c SPMinimaxNode.h SPFIARGame.h
	$(CC) $(COMP_FLAG) -c $*.c 
SPMinimax.o: SPMinimax.c SPMinimax.h SPMinimaxNode.h SPFIARGame.h
	$(CC) $(COMP_FLAG) -c $*.c
SPMainAux.o: SPMainAux.c SPMainAux.h SPFIARGame.h SPMinimax.h
	$(CC) $(COMP_FLAG) -c $*.c
main.o: main.c SPMainAux.h SPFIARGame.h SPMinimax.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)
