#
#
#         Name: makefile (rules file for 'make')
#       Author: Rick Troth, rogue programmer
#         Date: 2017-Nov-25 (Sat) Thanksgiving 2017 roughly
#               2023-03-16 (Thu) converted to makefile.in configuration
#         Note: this makefile configured for Linux-i386
#
#

DELIVERABLES    =       xmitmsg xmiterr libxmitmsgx.a libxmitmsgxdyn.so

##### configuration #####

CFLAGS          =       -fPIC
PREFIX          =       /usr
SYSTEM          =       Linux-i386

##### configuration #####

# first target serves as the default, but name it that way anyway
_default:	xmsgtest
		./xmsgtest

all:		$(DELIVERABLES)

# this target is an 'ar' archive static library
libxmitmsgx.a:  xmitmsgx.o
#	$(LD) -static -o $@ $+
		$(AR) -r $@ $+

# this target is the shared library, if you just gotta
libxmitmsgxdyn.so:  xmitmsgx.o
		$(LD) -shared -o $@ $+

# this object is the library
xmitmsgx.o:     makefile xmitmsgx.c xmitmsgx.h
		$(CC) $(CFLAGS) -o xmitmsgx.o -c xmitmsgx.c

# fetch the source from GitHub
xmitmsgx.c:
		wget https://raw.githubusercontent.com/trothr/xmitmsgx/master/$@

# fetch the source from GitHub
xmitmsgx.h:
		wget https://raw.githubusercontent.com/trothr/xmitmsgx/master/$@

# this target is left over from the original POSIX implementation
xmitmsg:	makefile xmitmsg.o xmitmsgx.o
#		$(CC) -o xmitmsg xmitmsg.o -L. -lxmitmsgx
		$(CC) -o xmitmsg xmitmsg.o xmitmsgx.o
#		strip xmitmsg

xmitmsg.o:      makefile xmitmsg.c xmitmsgx.h
		$(CC) $(CFLAGS) -o xmitmsg.o -c xmitmsg.c

# fetch the source from GitHub
xmitmsg.c:
		wget https://raw.githubusercontent.com/trothr/xmitmsgx/master/$@

# 'xmiterr' is a utility program which reports ERRNO messages
xmiterr:	makefile xmiterr.o xmitmsgx.o
#		$(CC) -o xmiterr xmiterr.o -L. -lxmitmsgx
		$(CC) -o xmiterr xmiterr.o xmitmsgx.o

# object deck for the 'xmiterr' program
xmiterr.o:      makefile xmiterr.c xmitmsgx.h
		$(CC) $(CFLAGS) -o xmiterr.o -c xmiterr.c

# fetch the source from GitHub
xmiterr.c:
		wget https://raw.githubusercontent.com/trothr/xmitmsgx/master/$@

# pseudo target to build static and shared libraries
libraries:  libxmitmsgx.a libxmitmsgxdyn.so

xmsgtest:	makefile xmsgtest.o xmitmsgx.o
#		$(CC) -o xmsgtest xmsgtest.o -L. -lxmitmsgx
		$(CC) -o xmsgtest xmsgtest.o xmitmsgx.o

xmsgtest.o:     makefile xmsgtest.c xmitmsgx.h
		$(CC) $(CFLAGS) -o xmsgtest.o -c xmsgtest.c

# fetch the source from GitHub
xmsgtest.c:
		wget https://raw.githubusercontent.com/trothr/xmitmsgx/master/$@
    
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

test-libraries:  libxmitmsgx.a libxmitmsgxdyn.so xmsgtest.o
		@echo "$(MAKE): testing static library"
		$(CC) -o xmsgtest xmsgtest.o -L. -lxmitmsgx
		./xmsgtest
		@echo "$(MAKE): testing shared library"
		$(CC) -o xmsgtest xmsgtest.o -L. -lxmitmsgxdyn
		sh -c ' LD_LIBRARY_PATH=. ; export LD_LIBRARY_PATH ; exec ./xmsgtest '

#
# yeah, we need an "install" target
install:        $(DELIVERABLES) xmitmsgx.msgs errno.msgs
		@mkdir -p $(PREFIX)/bin $(PREFIX)/lib $(PREFIX)/include \
			$(PREFIX)/share/locale/en_US # $(PREFIX)/src
		cp -p xmitmsg xmiterr $(PREFIX)/bin/.
		cp -p libxmitmsgx.a libxmitmsgxdyn.so $(PREFIX)/lib/.
		cp -p xmitmsgx.h $(PREFIX)/include/.
		cp -p xmitmsgx.msgs errno.msgs $(PREFIX)/share/locale/en_US/.
#		cp -p *.c *.h makefile $(PREFIX)/src/.

#
makefile:	makefile.in
		@echo "$(MAKE): you need to re-run ./configure"
		@false

# fetch the sources from GitHub
makefile.in:    configure
		wget https://raw.githubusercontent.com/trothr/xmitmsgx/master/$@
    
configure:
		wget https://raw.githubusercontent.com/trothr/xmitmsgx/master/$@
    
xmitmsgx.msgs:
		wget https://raw.githubusercontent.com/trothr/xmitmsgx/master/$@
    
errno.msgs:
		wget https://raw.githubusercontent.com/trothr/xmitmsgx/master/$@

# reset things for a fresh build from source
clean:
		rm -f *.o *.a *.so \
			msgtest xmsgtest xfortune xmiterr xmitmsg


