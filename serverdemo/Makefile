#!/bin/sh

CC=g++ -fPIC -g -D__64BIT__ 
GCC=g++ -fPIC -g -D__64BIT__

LIB_SFL_PATH=../sfl/
IFLAGS=-I${LIB_SFL_PATH} 
LFLAGS= -Wl,${LIB_SFL_PATH}/sfl.a -lpthread

OUTPUT=eservcln
OBJFILES=config.o eservcln.o eventlist.o genral.o
SOURCES=${OBJFILES:.o=.cpp}

.SUFFIXES: .cpp .o

.cpp.o:
	@echo "Compiling $*.cpp"
	@${CC} -c $< ${IFLAGS} -o $@

all: $(OBJFILES)
	@echo "Generating $(OUTPUT)"
	@$(GCC) -o $(OUTPUT) $(OBJFILES) $(LFLAGS)
#	@strip $(OUTPUT)
	@echo Done

clean:
	@echo "Cleaning..."
	@rm -vf $(OUTPUT) $(OBJFILES)
	@echo "Cleaned!"
