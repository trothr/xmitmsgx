#!/bin/sh
#
#         Name: xmmtcldemo.sh (shell script)
#         Date: 2023-07-27 (Thu)
#

#
# make some detection about this environment
cd `dirname "$0"`
D=`pwd`                         # the directory where these files reside
E=`sh -c ' cd .. ; exec pwd '`  # presumed package root (one level up)

#
# set or expand the loadable library search
if [ -z "$LD_LIBRARY_PATH" ] ; then LD_LIBRARY_PATH="$E/lib"
                               else LD_LIBRARY_PATH="$E/lib:$LD_LIBRARY_PATH" ; fi
if [ -z "$DYLD_LIBRARY_PATH" ] ; then DYLD_LIBRARY_PATH="$E/lib"
                                 else DYLD_LIBRARY_PATH="$E/lib:$DYLD_LIBRARY_PATH" ; fi
if [ -z "$SHLIB_PATH" ] ; then SHLIB_PATH="$E/lib"
                          else SHLIB_PATH="$E/lib:SHLIB_PATH" ; fi
# the rationale here is that we set all known library search variables
# rather than attemp OS detection - more than one way to skin that cat

#
# conditionally add Regina Rexx lodable libraries to the search
for LD in /usr/opt/tcl/lib /usr/opt/tcl/lib64 ; do
    if [ -d $LD ] ; then
        LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$LD"
        DYLD_LIBRARY_PATH="$DYLD_LIBRARY_PATH:$LD"
        SHLIB_PATH="$SHLIB_PATH:$LD"
    fi
done
export LD_LIBRARY_PATH DYLD_LIBRARY_PATH SHLIB_PATH

#
# conditionally augment the command search path
if [ -d /usr/opt/tcl/bin ] ; then PATH=$PATH:/usr/opt/tcl/bin ; fi
export PATH

#
# run the demo script under the interpreter (replacing this shell)
exec tclsh xmmtcldemo.tcl $*


