TARGET=cva

all: cva	

cva:
	g++ test.cpp cva_cfgs.cpp cva_cfg.cpp cva.cpp ../util.cpp -o test -lgmp
	
test:	cva
	./test

clean:
	rm -f ${TARGET} *.o *~
