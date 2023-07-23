#!/bin/sh
#
#         Name: xmmrexx.sh (shell script)
#               demonstrate the message handler called from Java
#         Date: 2023-07-21 (Fri)
#

#
# make some detection about this environment
cd `dirname "$0"`
D=`pwd`                         # the directory where these files reside
E=`sh -c ' cd .. ; exec pwd '`  # presumed package root (one level up)

#
# set or expand the loadable library search
if [ -z "$LD_LIBRARY_PATH" ] ; then LD_LIBRARY_PATH="$E/lib"
                               else LD_LIBRARY_PATH="$E/lib:." ; fi
if [ -z "$DYLD_LIBRARY_PATH" ] ; then DYLD_LIBRARY_PATH="$E/lib"
                                 else DYLD_LIBRARY_PATH="$E/lib:." ; fi
if [ -z "$SHLIB_PATH" ] ; then SHLIB_PATH="$E/lib"
                          else SHLIB_PATH="$E/lib:." ; fi
# the rationale here is that we set all known library search variables
# rather than attemp OS detection - more than one way to skin that cat

#
# conditionally add Regina Rexx lodable libraries to the search
if [ -d /usr/opt/regina/lib ] ; then
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/opt/regina/lib
    DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:/usr/opt/regina/lib
    SHLIB_PATH=$SHLIB_PATH:/usr/opt/regina/lib
  fi
export LD_LIBRARY_PATH DYLD_LIBRARY_PATH SHLIB_PATH

#
# conditionally augment the command search path
if [ -d /usr/opt/regina/bin ] ; then PATH=$PATH:/usr/opt/regina/bin ; fi
export PATH

regina xmmrexx.rx
RC=$? ; if [ $RC -ne 0 ] ; then exit $RC ; fi

exit


