D = eccva cva

all:	; for d in $D; do ( cd $$d && echo $$d: && make $@ ); done
clean:	; for d in $D; do ( cd $$d && echo $$d: && make clean ); done
	rm -f *.o *~
test:	; for d in $D; do ( cd $$d && echo $$d: && make test ); done

