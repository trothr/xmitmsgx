#
#
#         Name: makefile (rules file for 'make')
#       Author: Rick Troth, rogue programmer
#         Date: 2017-Nov-25 (Sat) Thanksgiving 2017 roughly
#
#

PREFIX          =       /usr

DELIVERABLES    =       xmitmsg xmiterr libxmitmsgx.a libxmitmsgx.so

#####

CFLAGS          =       -fPIC

#####

# first target serves as the default, but name it that way anyway
_default:	xmsgtest
		./xmsgtest

all:		$(DELIVERABLES)

# this object is the library
xmitmsgx.o:	xmitmsgx.c xmitmsgx.h
		$(CC) $(CFLAGS) -o xmitmsgx.o -c xmitmsgx.c

# this target is an 'ar' archive static library
libxmitmsgx.a:  xmitmsgx.o
#	$(LD) -static -o $@ $+
	$(AR) -r $@ $+

# this target is the shared library, if you just gotta
libxmitmsgx.so:  xmitmsgx.o
	$(LD) -shared -o $@ $+

# this target is left over from the original POSIX implementation
xmitmsg:	makefile xmitmsg.o xmitmsgx.o
#		$(CC) -o xmitmsg xmitmsg.o -L. -lxmitmsgx
		$(CC) -o xmitmsg xmitmsg.o xmitmsgx.o
#		strip xmitmsg

xmitmsg.o:      makefile xmitmsg.c xmitmsgx.h
		$(CC) -o xmitmsg.o -c xmitmsg.c

# 'xmiterr' is a utility program which reports ERRNO messages
xmiterr:	makefile xmiterr.o xmitmsgx.o
#		$(CC) -o xmiterr xmiterr.o -L. -lxmitmsgx
		$(CC) -o xmiterr xmiterr.o xmitmsgx.o

# object deck for the 'xmiterr' program
xmiterr.o:	makefile xmiterr.c xmitmsgx.h
		$(CC) -o xmiterr.o -c xmiterr.c

# yeah, we need an "install" target
#install:
#		mv xmitmsg $(HOME)/.`platform`/bin/.

# pseudo target to build static and shared libraries
libraries:  libxmitmsgx.a libxmitmsgx.so

xmsgtest:	makefile xmsgtest.o xmitmsgx.o
#		$(CC) -o xmsgtest xmsgtest.o -L. -lxmitmsgx
		$(CC) -o xmsgtest xmsgtest.o xmitmsgx.o

xmsgtest.o:	makefile xmsgtest.c xmitmsgx.h
		$(CC) -o xmsgtest.o -c xmsgtest.c

# pseudo target to run tests
tests:		test-xmitmsg test-xmiterr test-xmsgtest
		@echo "$(MAKE): ##### all tests passed, whoop! #####"

test-xmitmsg:   xmitmsg
		./xmitmsg 386
		./xmitmsg 408

# arbitrarily drive several ERRNO messages by number
# might need 'LANG=en_US ; export LANG' until smarter file search
test-xmiterr:	xmiterr
		./xmiterr 123 
		./xmiterr 77 

# run the primary test program
test-xmsgtest:	xmsgtest
		./xmsgtest

install:        $(DELIVERABLES)

# reset things for a fresh build from source
clean:
		rm -f *.o *.a *.so \
			msgtest xmsgtest xfortune xmiterr xmitmsg


