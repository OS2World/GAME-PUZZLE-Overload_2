CFLAGS=-g -Wall
CC=gcc $(CFLAGS)

.SUFFIXES: .c .exe
#.PHONY: default all c-samples cpp-samples objc-samples os2-samples sign clean

.c.exe:
	$(CC) $<


default:        c-samples

over2.exe:        over2.c over2.h over2.def over2.res
	gcc -Zomf -Zsys -s -Wall over2.c over2.def over2.res

over2.res:        over2.rc over2.h over2.dlg
	rc -r -i \emx\include over2.rc
