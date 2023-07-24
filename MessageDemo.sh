#!/bin/sh
#
#         Name: MessageDemo.sh (shell script)
#               demonstrate the message handler called from Java
#         Date: 2023-07-21 (Fri)
#

#
# make some detection about this environment
cd `dirname "$0"`
D=`pwd`                         # the directory where these files reside
E=`sh -c ' cd .. ; exec pwd '`  # presumed package root (one level up)
F=/tmp/$$ ; mkdir $F            # a temporary directory

#
# work from the temporary directory
cd $F

#
# prepare a Java-compatible hierarchy
rm -rf com
mkdir -p com
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi
ln -sf . com/casita
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi
ln -s $E/lib com/casita/xmitmsgx
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi

#
# compile the demo program
cp -p $D/MessageDemo.java . 2> /dev/null
javac MessageDemo.java
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi

#
# augment loadable library search the Linux way
LD_LIBRARY_PATH=$E/lib
export LD_LIBRARY_PATH

#
# run the compiled program
#cp -p $D/xmitmsgx.msgs . 2> /dev/null
cp -p $E/share/locale/*/xmitmsgx.msgs . 2> /dev/null
java -classpath . MessageDemo
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi

#
# clean-up
rm MessageDemo.class
rm -rf com
cd $D ; rm -r $F

exit


