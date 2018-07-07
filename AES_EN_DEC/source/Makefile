
CC	= gcc
HEAD	= -I../header/
SSLHEAD	= -I/root/Downloads/openssl-1.1.1-pre8/include/openssl/
SSLRPATH= -Wl,-rpath=/root/Downloads/openssl-1.1.1-pre8/
SSLLIB	= -L/root/Downloads/openssl-1.1.1-pre8/ -lcrypto
MODULE	= ../en_dec_module

all:
	$(CC) -g -o $(MODULE) main_routine.c en_dec_routines.c $(HEAD) $(SSLHEAD) $(SSLRPATH) $(SSLLIB)

clean:
	rm -f $(MODULE)

