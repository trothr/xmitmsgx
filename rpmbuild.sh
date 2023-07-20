#!/bin/sh
#
#         Name: rpmbuild.sh (shell script)
#               build an RPM from the "spec file"
#         Date: 2023-03-22 (Wed)
#

# run from the resident directory
cd `dirname "$0"`

# establish certain variables
UNAMEM=`uname -m | sed 's#^i.86$#i386#' | sed 's#^armv.l$#arm#'`
if [ ! -s .rpmseq ] ; then echo "0" > .rpmseq ; fi
RELEASE=`cat .rpmseq`
STAGING=`pwd`/rpmbuild.d

# I wish the following were not hard-coded
APPLID=$1
VERSION=$2

# configure the package normally
./configure
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi

# build all deliverables
make all # just short of doing 'make install'
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi

# override the PREFIX for the install step
make PREFIX=$STAGING install
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi

# remake the spec file
rm -f xmitmsgx.spec
make STAGING=$STAGING UNAMEM=$UNAMEM RELEASE=$RELEASE xmitmsgx.spec
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi

# dump the heavy lifting on the 'rpmbuild' command
rpmbuild -bb --nodeps xmitmsgx.spec
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi

# recover the resulting package file ... yay!
mv $HOME/rpmbuild/RPMS/$UNAMEM/$APPLID-$VERSION-$RELEASE.$UNAMEM.rpm .
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi
cp -p $APPLID-$VERSION-$RELEASE.$UNAMEM.rpm xmitmsgx.rpm

# increment the sequence number for the next build
expr $RELEASE + 1 > .rpmseq

exit


