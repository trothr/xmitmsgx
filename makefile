#
#
#         Name: makefile (rules file for 'make')
#       Author: Rick Troth, rogue programmer
#         Date: 2017-Nov-25 (Sat) Thanksgiving 2017 roughly
#               2023-03-16 (Thu) converted to makefile.in configuration
#         Note: this makefile configured for Linux-x86_64
#
#

APPLID          =       xmitmsgx
VERSION         =       2.1.5

DELIVERABLES    =       xmitmsg xmiterr xmmlogin libxmitmsgx.a libxmitmsgxdyn.so
SOURCEURL       =       https://raw.githubusercontent.com/trothr/xmitmsgx/master
LOCALE          =       en_US

##### configuration #####

PREFIX          =       /usr
CFLAGS          =        -fPIC -DPREFIX=\"$(PREFIX)\"
LDFLAGS         =       
SHFLAGS         =        -shared
# we may later use CC, CXX, CPP, CPPFLAGS, and/or CXXFLAGS

##### configuration #####

# first target serves as the default, but name it that way anyway
#_default:	xmsgtest
#		./xmsgtest
_default:	$(DELIVERABLES)

all:		$(DELIVERABLES)

# this target is an 'ar' archive static library
libxmitmsgx.a:  xmitmsgx.o
#	$(LD) -static -o $@ $+
#		$(AR) -r $@ $+
		$(AR) -r libxmitmsgx.a xmitmsgx.o
#
# $@ is the target
# $+ is the list of dependencies but does not work everywhere
#

# this target is the shared library, if you just gotta
libxmitmsgxdyn.so:  xmitmsgx.o
#		$(LD) -shared -o $@ $+
#		$(CC) -shared -o $@ $+
		$(CC) $(SHFLAGS) -o libxmitmsgxdyn.so xmitmsgx.o

# this object is the library
xmitmsgx.o:     makefile xmitmsgx.c xmitmsgx.h
		$(CC) $(CFLAGS) -o xmitmsgx.o -c xmitmsgx.c

# fetch the source from GitHub
xmitmsgx.c:
		@echo "$(MAKE): you need $@"
		wget $(SOURCEURL)/$@

# fetch the source from GitHub
xmitmsgx.h:
		@echo "$(MAKE): you need $@"
		wget $(SOURCEURL)/$@

# this target is left over from the original POSIX implementation
xmitmsg:	makefile xmitmsg.o xmitmsgx.o
#		$(CC) -o xmitmsg xmitmsg.o -L. -lxmitmsgx
		$(CC) $(LDFLAGS) -o xmitmsg xmitmsg.o xmitmsgx.o
#		strip xmitmsg

xmitmsg.o:      makefile xmitmsg.c xmitmsgx.h
		$(CC) $(CFLAGS) -o xmitmsg.o -c xmitmsg.c

# fetch the source from GitHub
xmitmsg.c:
		@echo "$(MAKE): you need $@"
		wget $(SOURCEURL)/$@

# 'xmiterr' is a utility program which reports ERRNO messages
xmiterr:	makefile xmiterr.o xmitmsgx.o
#		$(CC) -o xmiterr xmiterr.o -L. -lxmitmsgx
		$(CC) $(LDFLAGS) -o xmiterr xmiterr.o xmitmsgx.o

# object deck for the 'xmiterr' program
xmiterr.o:      makefile xmiterr.c xmitmsgx.h
		$(CC) $(CFLAGS) -o xmiterr.o -c xmiterr.c

# fetch the source from GitHub
xmiterr.c:
		@echo "$(MAKE): you need $@"
		wget $(SOURCEURL)/$@


# 'xmmlogin' is a utility program to log an "I have signed on" message
xmmlogin:       makefile xmmlogin.o xmitmsgx.o
		$(CC) $(LDFLAGS) -o xmmlogin xmmlogin.o xmitmsgx.o

# object deck for the 'xmmlogin' program
xmmlogin.o:     makefile xmmlogin.c xmitmsgx.h
		$(CC) $(CFLAGS) -o xmmlogin.o -c xmmlogin.c

# fetch the source from GitHub
xmmlogin.c:
		@echo "$(MAKE): you need $@"
		wget $(SOURCEURL)/$@


# pseudo target to build static and shared libraries
libraries:  libxmitmsgx.a libxmitmsgxdyn.so

xmsgtest:	makefile xmsgtest.o xmitmsgx.o
#		$(CC) -o xmsgtest xmsgtest.o -L. -lxmitmsgx
		$(CC) $(LDFLAGS) -o xmsgtest xmsgtest.o xmitmsgx.o

xmsgtest.o:     makefile xmsgtest.c xmitmsgx.h
		$(CC) $(CFLAGS) -o xmsgtest.o -c xmsgtest.c

# fetch the source from GitHub
xmsgtest.c:
		@echo "$(MAKE): you need $@"
		wget $(SOURCEURL)/$@

########################################################################
# Rexx support
rexx:           libxmmrexx.so

xmmrexx.o:      makefile xmmrexx.c xmitmsgx.h
		@echo "$(MAKE): compiling Tcl support"
		$(CC) $(CFLAGS) -o xmmrexx.o -c xmmrexx.c

libxmmrexx.so:  xmmrexx.o xmitmsgx.o
		$(CC) $(SHFLAGS) -o libxmmrexx.so xmmrexx.o xmitmsgx.o

########################################################################
# Python support
python:
		@echo "$(MAKE): support for Python is planned"

########################################################################
# Java support (JNI)
java:           libxmmjava.so MessageService.class

#
# prepare a Java-compatible hierarchy
com/casita/xmitmsgx:
	@rm -rf com
	mkdir -p com
	ln -s . com/casita
	ln -s .. com/casita/xmitmsgx

MessageService.class:  MessageService.java
		javac MessageService.java

#
# this file is for comparison and discarded once xmmjava.c is up-to-date
xmmjava.h:      MessageService.class com/casita/xmitmsgx
		javah com.casita.xmitmsgx.MessageService
		mv com_casita_xmitmsgx_MessageService.h xmmjava.h

xmmjava.o:      xmmjava.c
		$(CC) $(CFLAGS) -I. \
		  -I/usr/lib/jvm/java/include -I/usr/lib/jvm/java/include/linux \
		  -o xmmjava.o -c xmmjava.c
#		  -I/usr/lib/jvm/java-1.8.0-openjdk/include -I/usr/lib/jvm/java-1.8.0-openjdk/include/linux
#		  -I/usr/lib/jvm/java-11-openjdk-11.0.14.1.1-1.el7_9.s390x/include -I/usr/lib/jvm/java-11-openjdk-11.0.14.1.1-1.el7_9.s390x/include/linux
#		  -I/usr/lib/jvm/java-11/include -I/usr/lib/jvm/java-11/include/linux

libxmmjava.so:  xmmjava.o xmitmsgx.o
		$(CC) $(SHFLAGS) -o libxmmjava.so xmmjava.o xmitmsgx.o

com/casita/xmitmsgx/MessageService.class:  com/casita/xmitmsgx/MessageService.java
	javac com/casita/xmitmsgx/MessageService.java

########################################################################
# COBOL support
cobol:
		@echo "$(MAKE): support for COBOL is planned"

########################################################################
# Tcl support
tcl:            xmmtcl.so

xmmtcl.o:       makefile xmmtcl.c xmitmsgx.h
		@echo "$(MAKE): compiling Tcl support"
		$(CC) $(CFLAGS) -o xmmtcl.o -c xmmtcl.c

xmmtcl.so:      xmmtcl.o xmitmsgx.o
		$(CC) $(SHFLAGS) -o xmmtcl.so xmmtcl.o xmitmsgx.o

########################################################################
# Go/Golang support
go golang :
		@echo "$(MAKE): support for Go/Golang is planned"

########################################################################
# pseudo target to run tests
tests:		test-xmitmsg test-xmiterr test-xmsgtest
		@echo ""
		@echo "$(MAKE): ##### all tests passed, whoop! #####"
		@echo ""

test-xmitmsg:   xmitmsg
		./xmitmsg 386
		./xmitmsg 408

# arbitrarily drive several ERRNO messages by number
test-xmiterr:	xmiterr
		./xmiterr 123
		./xmiterr 77

# run the primary test program
test-xmsgtest:	xmsgtest
		./xmsgtest

test-libraries:  libxmitmsgx.a libxmitmsgxdyn.so xmsgtest.o
		@echo "$(MAKE): testing static library"
		$(CC) $(LDFLAGS) -o xmsgtest xmsgtest.o -L. -lxmitmsgx
		./xmsgtest
		@echo "$(MAKE): testing shared library"
		$(CC) $(LDFLAGS) -o xmsgtest xmsgtest.o -L. -lxmitmsgxdyn
		sh -c ' LD_LIBRARY_PATH=. ; export LD_LIBRARY_PATH ; exec ./xmsgtest '

#
# yeah, we need an "install" target
install:        $(DELIVERABLES) xmitmsgx.msgs errno.msgs
		@mkdir -p $(PREFIX)/bin $(PREFIX)/lib $(PREFIX)/include \
		  $(PREFIX)/share/locale/$(LOCALE) $(PREFIX)/sbin $(PREFIX)/src
		cp -p xmitmsg xmiterr $(PREFIX)/bin/.
#		cp -p libxmitmsgx.a libxmitmsgxdyn.so $(PREFIX)/lib/.
#		cp -p libxmitmsgx.a *.so $(PREFIX)/lib/.
		cp -p *.a *.so $(PREFIX)/lib/.
		-cp -p *.class $(PREFIX)/lib/.
		cp -p xmitmsgx.h $(PREFIX)/include/.
		cp -p xmitmsgx.msgs errno.msgs $(PREFIX)/share/locale/$(LOCALE)/.
		cp -p xmitmivp.sh $(PREFIX)/sbin/.
		cp -p xmitmivp.c $(PREFIX)/src/.
#
		cp -p xmmdemoc.c xmmdemoc.sh $(PREFIX)/src/.
		cp -p xmmrexx.rx xmmrexx.sh $(PREFIX)/src/.
		cp -p MessageDemo.java MessageDemo.sh $(PREFIX)/src/.
#

########################################################################
# RPM support
rpm:            xmitmsgx.rpm

# http://ftp.rpm.org/max-rpm/s1-rpm-build-creating-spec-file.html
xmitmsgx.rpm:
#		rpmbuild -bb --nodeps xmitmsgx.spec
		./rpmbuild.sh $(APPLID) $(VERSION)

#
xmitmsgx.spec:  xmitmsgx.spec.in
		@echo "$(MAKE): checking required variables"
		test ! -z "$(UNAMEM)" # UNAMEM
		test ! -z "$(RELEASE)" # RELEASE
		test ! -z "$(STAGING)" # STAGING
		cat xmitmsgx.spec.in \
		  | sed 's#%SPEC_PREFIX%#$(PREFIX)#g' \
		  | sed 's#%SPEC_UNAMEM%#$(UNAMEM)#g' \
		  | sed 's#%SPEC_VERSION%#$(VERSION)#g' \
		  | sed 's#%SPEC_RELEASE%#$(RELEASE)#g' \
		  | sed 's#%SPEC_STAGING%#$(STAGING)#g' \
		  | sed 's#%SPEC_APPLID%#$(APPLID)#g' \
                  > xmitmsgx.spec

# fetch the source from GitHub
xmitmsgx.spec.in:
		@echo "$(MAKE): you need $@"
		wget $(SOURCEURL)/$@

#
makefile:	makefile.in
		@echo "$(MAKE): makefile is out of synch"
		@echo "$(MAKE): you need to re-run ./configure"
		@false

# fetch the sources from GitHub
makefile.in:
		@$(MAKE) configure
		@echo "$(MAKE): you need $@"
		wget $(SOURCEURL)/$@

configure:
		@echo "$(MAKE): you need $@"
		wget $(SOURCEURL)/$@

xmitmsgx.msgs:
		@echo "$(MAKE): you need $@"
		wget $(SOURCEURL)/$@

errno.msgs:
		@echo "$(MAKE): you need $@"
		wget $(SOURCEURL)/$@

# reset things for a fresh build from source
clean:
		rm -f *.o *.a *.so *.dylib *.rpm *.class \
			msgtest xmsgtest xfortune xmiterr xmitmsg xmmlogin \
			xmitmsgx.spec xmmjava.h
		rm -rf rpmbuild.d com

distclean:      clean
		rm -f xmmconfig.h xmmconfig.sh .rpmseq

veryclean:      distclean


