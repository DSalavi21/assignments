
CC	= gcc
# library libfileLog.so header path + iniparser.h path
LIBHEAD	= -I/root/programs/assignments/libraries/header/ -I/root/programs/assignments/INI_PARSER/iniparser/src/

# library libfileLog.so location + libiniparser.so path
LIBPATH   = -L/root/programs/assignments/libraries/libs/ -lfileLog -L/root/programs/assignments/INI_PARSER/iniparser/ -liniparser  

RPATH	= -Wl,-rpath=/root/programs/assignments/libraries/libs/
RM	= rm -f

all : 
	gcc -g -o writeLog source/configMain.c $(LIBHEAD) $(RPATH) $(LIBPATH)

clean :
	$(RM) writeLog
