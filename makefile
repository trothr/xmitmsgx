#
#
#
#
#


_default:	msgtest
		./msgtest

msgtest:	msgtest.o libxmitmsgx.a
		$(CC) -o msgtest msgtest.o -L. -lxmitmsgx

msgtest.o:	msgtest.c xmitmsgx.h
		$(CC) -o msgtest.o -c msgtest.c

xmitmsgx.o:	xmitmsgx.c xmitmsgx.h
		$(CC) -o xmitmsgx.o -c xmitmsgx.c

libxmitmsgx.a:  xmitmsgx.o
#	$(LD) -static -o $@ $+
	$(AR) -r $@ $+

libprotect.so:  xmitmsgx.o
	$(LD) -shared -o $@ $+


xmitmsg:	makefile xmitmsg.c dfopen.c
		cc -O -c dfopen.c
		cc -O -c xmitmsg.c
		cc -o xmitmsg xmitmsg.o dfopen.o
		strip xmitmsg


#install:
#		mv xmitmsg $(HOME)/.`platform`/bin/.

libraries:  libxmitmsgx.a libprotect.so

clean:
		rm -f *.o *.a *.so \
			msgtest xfortune


