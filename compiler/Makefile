CC      = gcc
CFLAGS  = -Wall -g
FLEX    = win_flex
BISON   = win_bison
TARGET  = compiler
SRCS    = lex.yy.c parser.tab.c symtable.c tac.c main.c

all: $(TARGET)

parser.tab.c parser.tab.h: parser.y
	$(BISON) -d -v parser.y

lex.yy.c: lexer.l parser.tab.h
	$(FLEX) lexer.l

$(TARGET): $(SRCS) symtable.h tac.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	del /f lex.yy.c parser.tab.c parser.tab.h parser.output $(TARGET).exe 2>nul || true

rebuild: clean all