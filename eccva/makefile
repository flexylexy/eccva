TARGETS=eccva test

all:	eccva

eccva:
	g++ test.cpp eccva_cfgs.cpp eccva_cfg.cpp eccva.cpp ec_point.cpp ../util.cpp -o test -lgmp

test:	eccva
	./test

clean:
	rm -f ${TARGETS} *.o *~
