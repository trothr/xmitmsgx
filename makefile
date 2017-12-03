#
#
#
#
#


_default:	msgtest
		./msgtest

msgtest:	msgtest.o xmitmsgx.o
		$(CC) -o msgtest msgtest.o xmitmsgx.o

msgtest.o:	msgtest.c xmitmsgx.h
		$(CC) -o msgtest.o -c msgtest.c

xmitmsgx.o:	xmitmsgx.c xmitmsgx.h
		$(CC) -o xmitmsgx.o -c xmitmsgx.c




xmitmsg:	makefile xmitmsg.c dfopen.c
		cc -O -c dfopen.c
		cc -O -c xmitmsg.c
		cc -o xmitmsg xmitmsg.o dfopen.o
		strip xmitmsg

#install:
#		mv xmitmsg $(HOME)/.`platform`/bin/.


clean:
		rm -f *.o \
			msgtest xfortune


