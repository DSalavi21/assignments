
LIB_SFL	= /home/dswork/source/sfl

BITS = $(shell echo `getconf LONG_BIT`)

# avoid sfl error for machine dependent compiler data type size 
# error "Cannot compile: must change definition of 'qbyte'."
ifeq (${BITS},64)
     MACFLAG = -D__64BIT__ ${BUILDFLAGS} 
endif
 
GCC = g++ ${MACFLAG} -g

all:
	$(GCC) -o run_sflDemo -I$(LIB_SFL) sfl_demo.cpp $(LIB_SFL)/sfl.a

clean:
	rm -rf run_sflDemo
