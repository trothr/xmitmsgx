#
#
#         Name: makefile (rules file for 'make')
#       Author: Rick Troth, rogue programmer
#         Date: 2017-Nov-25 (Sat) Thanksgiving 2017 roughly
#
#


# first target serves as the default, but name it that way anyway
_default:	xmsgtest
		./xmsgtest

# this object is the library
xmitmsgx.o:	xmitmsgx.c xmitmsgx.h
		$(CC) -o xmitmsgx.o -c xmitmsgx.c

# this target is an 'ar' archive static library
libxmitmsgx.a:  xmitmsgx.o
#	$(LD) -static -o $@ $+
	$(AR) -r $@ $+

# this target is the shared library, if you just gotta
libxmitmsgx.so:  xmitmsgx.o
	$(LD) -shared -o $@ $+

# this target is left over from the original POSIX implementation
xmitmsg:	makefile xmitmsg.c dfopen.c
		cc -O -c dfopen.c
		cc -O -c xmitmsg.c
		cc -o xmitmsg xmitmsg.o dfopen.o
		strip xmitmsg

# 'xmiterr' is a utility program which reports ERRNO messages
xmiterr:	xmiterr.o libxmitmsgx.a
		$(CC) -o xmiterr xmiterr.o -L. -lxmitmsgx

# object deck for the 'xmiterr' program
xmiterr.o:	xmiterr.c xmitmsgx.h
		$(CC) -o xmiterr.o -c xmiterr.c

# yeah, we need an "install" target
#install:
#		mv xmitmsg $(HOME)/.`platform`/bin/.

# pseudo target to build static and shared libraries
libraries:  libxmitmsgx.a libxmitmsgx.so

xmsgtest:	xmsgtest.o libxmitmsgx.a
		$(CC) -o xmsgtest xmsgtest.o -L. -lxmitmsgx

xmsgtest.o:	xmsgtest.c xmitmsgx.h
		$(CC) -o xmsgtest.o -c xmsgtest.c

# pseudo target to run tests
tests:		test-xmiterr test-xmsgtest
		@echo "$(MAKE): ##### all tests passed, whoop! #####"

# arbitrarily drive several ERRNO messages by number
# might need 'LANG=en_US ; export LANG' until smarter file search
test-xmiterr:	xmiterr
		./xmiterr 123 
		./xmiterr 77 

# run the primary test program
test-xmsgtest:	xmsgtest
		./xmsgtest

# reset things for a fresh build from source
clean:
		rm -f *.o *.a *.so \
			msgtest xmsgtest xfortune xmiterr


