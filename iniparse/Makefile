CC      = gcc
INIHEAD = -I/root/programs/assignments/INI_PARSER/iniparser/src/          # "iniparser.h" header file path
INILIB  = -L/root/programs/assignments/INI_PARSER/iniparser/ -liniparser  # library location 

all : demo1 

demo1 : 
        $(CC) -g -o demoexe demo1.c $(INIHEAD) $(INILIB)   
        
clean :
        rm -f demoexe
