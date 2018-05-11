
CC      = gcc
INIHEAD = -I/root/programs/assignments/INI_PARSER/iniparser/src/          # "iniparser.h" header file path
INILIB  = -L/root/programs/assignments/INI_PARSER/iniparser/ -liniparser  # library location 
OBJHEAD = -I../header/
SOPATH  = /root/programs/assignments/libraries/libs/libfileLog.so
RM      = rm -f

ISEXIST = $(shell echo `cat EXIST`)

all : fileLog	

fileLog :
	$(CC) $(OBJHEAD) -c -fPIC fileLog.c $(INIHEAD)	
	$(CC) -shared -o $(SOPATH) fileLog.o
ifeq ($(ISEXIST),0)
		echo "1" > EXIST
endif


clean :
ifeq ($(ISEXIST),1)
		$(RM) fileLog.o
		$(RM) $(SOPATH) 
		echo "0" > EXIST
endif


