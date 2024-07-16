all:
	g++ hotel.cc -o hotel -lsimlib -lm -Wextra -pedantic

pack: all
	zip 02_xkromp00_xoleks00.zip *

clean:
	rm -f hotel 02_xkromp00_xoleks00.zip

run: all
	./hotel

test1: all
	./hotel -c 26 -p 700 -e 2875 -g 0 -d 365 -o test1.dat -f 

exp1: all
	./hotel -c 26 -p 700 -e 2875 -g 0 -o exp1.dat

exp2: all
	./hotel -c 26 -p 700 -e 2875 -g 0 -o exp2.dat

exp3: all
	./hotel -c 26 -p 700 -e 2875 -g 0 -o exp3.dat

exp4: all
	./hotel -c 26 -p 700 -e 2875 -g 0 -o exp4.dat
