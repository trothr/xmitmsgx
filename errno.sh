#!/bin/sh
#
#           Name: errno.sh (shell script)
#                 recover ERRNO constants for use as a message repository
#           Date: 2014-May-10
#

find /usr/include/. -type f -name "errno*" -print \
  | xargs grep '#' | grep define \
  | awk -F# '{print $2}' \
  | awk '{print $2 , $3}' | grep '^E' \
  | awk '{print $2 , $1}' | grep -v '^E' \
  | sort -n | uniq


