ver = $(shell echo `cat version`)
rel = $(shell echo `cat release`)

rel2 = $(shell echo `(expr ${rel} + 1)`)
ver2 = $(shell echo `(expr ${ver} + 1)`)

printv = "${ver}.${rel}"

all:
        @echo current version ${ver} and current release ${rel}
        @echo Building Version ${printv}
        gcc -I header/ -o build -D MYVER=${printv} source/mainfun.c     
                
        echo ${rel2} > release  
ifeq (${rel},9)
                echo ${ver2} > version
                echo "0" > release
endif
