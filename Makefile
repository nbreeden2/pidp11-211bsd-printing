MYBIN="${HOME}/bin"

u2d: u2d.c
	cc -o u2d u2d.c

test1:
	./u2d u2d.c
test2:
	./u2d u2d.c -l
test3:
	./u2d u2d.c -i
test4:
	./u2d u2d.c -l -i
test5:
	./u2d u2d.c -p
test6:
	./u2d u2d.c -i -l -p	

install:
	cp u2d $(HOME)/bin
