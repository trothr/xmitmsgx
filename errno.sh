#!/bin/sh
#
#           Name: errno.sh (shell script)
#                 recover ERRNO constants for use as a message repository
#           Date: 2014-May-10
#

find /usr/include/. -type f -name "errno*.h" -print \
  | xargs grep '#define' \
  | awk -F# '{print $2}' \
  | awk '{print $3 "~" $2 "~" $0}' | sed 's#/\*#~#' \
  | grep '~E' \
  | awk -F~ '{print $1 , $2 , $4}' \
  | sed 's#\*/##'

exit

  | awk '{print $2 , $3}' | grep '^E' \
  | awk '{print $2 , $1}' | grep -v '^E' \
  | sort -n | uniq


