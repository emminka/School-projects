#swipl -o flp23-log -g main -c flp23-log.pl
all:
	swipl -q -g main -o flp23-log -c flp23-log.pl

clean:
	rm -f flp23-log.hi flp23-log.o flp23-log flp-log-xkromp00.zip

pack: clean
	zip flp-log-xkromp00.zip Makefile flp23-log.pl README.md kocka1.txt kocka2.txt kocka3.txt kocka4.txt
