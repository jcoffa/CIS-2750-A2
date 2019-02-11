#####################################
#   Name: Joseph Coffa              #
#   Student #: 1007320              #
#   Due Date: February 27, 2019     #
#                                   #
#   Assignment 2, CIS*2750          #
#   Makefile                        #
#####################################

#############
# Variables #
#############

# files
LIBS = CalendarParser.h LinkedListAPI.h Parsing.h Initialize.h CalendarHelper.h
SHARED = list cal parsing init calhelp

# update VPATH to search directories for files specified in rule preqrequisites
SRC = src
OUT = bin
INCL = include
VPATH = $(SRC):$(INCL):$(OUT)

# compilation options
CC = gcc
CFLAGS := -std=c11 -Wall -Wpedantic $(addprefix -I,$(INCL)) -g
LDFLAGS := -L. -L$(OUT) $(addprefix -l,$(SHARED))


##############
# Make Rules #
##############

all: list parser libparsing.so libinit.so libcalhelp.so main

list: liblist.so

parser: libcal.so
	
liblist.so: LinkedListAPI.o
	$(CC) -shared $(OUT)/LinkedListAPI.o -o $(OUT)/liblist.so

LinkedListAPI.o: LinkedListAPI.c LinkedListAPI.h
	$(CC) $(CFLAGS) -c -fpic $(SRC)/LinkedListAPI.c -o $(OUT)/LinkedListAPI.o

libcal.so: CalendarParser.o
	$(CC) -shared $(OUT)/CalendarParser.o -o $(OUT)/libcal.so

CalendarParser.o: CalendarParser.c CalendarParser.h
	$(CC) $(CFLAGS) -c -fpic $(SRC)/CalendarParser.c -o $(OUT)/CalendarParser.o

libparsing.so: Parsing.o
	$(CC) -shared $(OUT)/Parsing.o -o $(OUT)/libparsing.so

Parsing.o: Parsing.c Parsing.h
	$(CC) $(CFLAGS) -c -fpic $(SRC)/Parsing.c -o $(OUT)/Parsing.o

libinit.so: Initialize.o
	$(CC) -shared $(OUT)/Initialize.o -o $(OUT)/libinit.so

Initialize.o: Initialize.c Initialize.h
	$(CC) $(CFLAGS) -c -fpic $(SRC)/Initialize.c -o $(OUT)/Initialize.o

libcalhelp.so: CalendarHelper.o
	$(CC) -shared $(OUT)/CalendarHelper.o -o $(OUT)/libcalhelp.so

CalendarHelper.o: CalendarHelper.c CalendarHelper.h
	$(CC) $(CFLAGS) -c -fpic $(SRC)/CalendarHelper.c -o $(OUT)/CalendarHelper.o

main: main.c $(addprefix lib,$(addsuffix .so,$(SHARED)))
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC)/main.c -o $(OUT)/main



#############
# Utilities #
#############

# make and run the main executable
run:
	$(MAKE) main
	LD_LIBRARY_PATH=.:$(OUT):$$LD_LIBRARY_PATH ./$(OUT)/main


# make and run the main executable with valgrind
memory:
	$(MAKE) main
	LD_LIBRARY_PATH=.:$(OUT):$$LD_LIBRARY_PATH valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(OUT)/main

# make and run the main executable with gdb
debug:
	$(MAKE) main
	LD_LIBRARY_PATH=.:$(OUT):$$LD_LIBRARY_PATH gdb ./$(OUT)/main

# removes all .o and .so files, and the main executable
clean:
	rm $(OUT)/*.o $(OUT)/*.so $(OUT)/main

