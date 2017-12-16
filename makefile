#
#
#         Name: makefile (rules file for 'make')
#       Author: Rick Troth, rogue programmer
#         Date: 2017-Nov-25 (Sat) Thanksgiving 2017 roughly
#
#


_default:	xmsgtest
		./xmsgtest

xmsgtest:	xmsgtest.o libxmitmsgx.a
		$(CC) -o xmsgtest xmsgtest.o -L. -lxmitmsgx

xmsgtest.o:	xmsgtest.c xmitmsgx.h
		$(CC) -o xmsgtest.o -c xmsgtest.c

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


xmiterr:	xmiterr.o libxmitmsgx.a
		$(CC) -o xmiterr xmiterr.o -L. -lxmitmsgx

xmiterr.o:	xmiterr.c xmitmsgx.h
		$(CC) -o xmiterr.o -c xmiterr.c


#install:
#		mv xmitmsg $(HOME)/.`platform`/bin/.

libraries:  libxmitmsgx.a libprotect.so

tests:		test-xmiterr test-xmsgtest

# might need 'LANG=en_US ; export LANG' until smarter file search
test-xmiterr:	xmiterr
		./xmiterr 123 
		./xmiterr 77 

test-xmsgtest:	xmsgtest
		./xmsgtest

clean:
		rm -f *.o *.a *.so \
			msgtest xmsgtest xfortune xmiterr


