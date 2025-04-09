# GNU make手册：http://www.gnu.org/software/make/manual/make.html
# ************ 遇到不明白的地方请google以及阅读手册 *************

# 编译器设定和编译选项
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c99 -g

# 编译目标：src目录下的所有.c文件
CFILES = $(shell find ./ -name "*.c")
OBJS = $(CFILES:.c=.o)
LFILE = $(shell find ./ -name "*.l")
YFILE = $(shell find ./ -name "*.y")
LFC = $(shell find ./ -name "*.l" | sed s/[^/]*\\.l/lex.yy.c/)
YFC = $(shell find ./ -name "*.y" | sed s/[^/]*\\.y/syntax.tab.c/)
LFO = $(LFC:.c=.o)
YFO = $(YFC:.c=.o)

produce: parser
	cp parser ..

parser: syntax $(filter-out $(LFO),$(OBJS))
	$(CC) -o parser $(filter-out $(LFO),$(OBJS)) -lfl -ly -g

syntax: lexical syntax-c
	$(CC) -c $(YFC) -o $(YFO) -g

lexical: $(LFILE)
	$(FLEX) -o $(LFC) $(LFILE) 

syntax-c: $(YFILE)
	$(BISON) -o $(YFC) -d -v $(YFILE) -g

debug: 
	flex lexical.l
	bison -d -v -t syntax.y
	gcc -w main.c syntax.tab.c common.c semantic.c -lfl -ly -o parser -g

-include $(patsubst %.o, %.d, $(OBJS))

# 定义的一些伪目标
.PHONY: clean test pack produce clean-pack
test:
	./parser ../Tests/test1.cmm
	./parser ../Tests/test2.cmm
	./parser ../Tests/test3.cmm
	./parser ../Tests/test4.cmm
	./parser ../Tests/test5.cmm
	./parser ../Tests/test6.cmm
	./parser ../Tests/test7.cmm
	./parser ../Tests/test8.cmm
	./parser ../Tests/test9.cmm
	./parser ../Tests/test10.cmm
	./parser ../Tests/test11.cmm
	./parser ../Tests/test12.cmm
	./parser ../Tests/test13.cmm
	./parser ../Tests/test14.cmm
	./parser ../Tests/test15.cmm
	./parser ../Tests/test16.cmm
	./parser ../Tests/test17.cmm
	./parser ../Tests/test18.cmm
	./parser ../Tests/test19.cmm
	./parser ../Tests/test20.cmm
	./parser ../Tests/test21.cmm
	./parser ../Tests/test22.cmm
	./parser ../Tests/test23.cmm

clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
	rm -f *~

clean-all: clean
	rm -f ../parser

pack:produce clean
	cd ~/Lab && zip -r 201220195.zip Code Test README parser report.pdf

clean-pack:
	rm -rf ../201220195.zip