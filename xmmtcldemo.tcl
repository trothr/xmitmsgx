#!/usr/bin/env tclsh
#
#         Name: xmmtcldemo.tcl (Tcl script)
#               Sample Message Handler script in Tcl, Tool Command Language
#         Date: 2023-07-27 (Thu)
#     See also: xmmdemoc.c, xmmrexx.rx, MessageDemo.java
#

#
# this means that the shared object must be found in library search
load xmmtcl.so

#if [catch { riskything } result] {
#  puts stderr "error: $result"
#} else {
#  puts stdout "okay: $result"
#}

#
# Print the sample name, also message number 205 (below)
puts "Message Handler demonstration program in Tcl"

#
# Report API version
puts [xmversion]

#
#/* initialize the messages file of interest */
xminit "xmitmsgx" # options can be skipped for now

# do it a second time and get a 302 error
#xminit "xmitmsgx"

#
# XMMTCL201I Hello, World!
xmprint 201

#
# message #119 only has nine (9) substitutions so the last is ignored
xmprint 119 "#1" "#2" "#3" "#4" "#5" "#6" "#7" "#8" "#9" "#10"

#
# message #119 only has nine (9) substitutions, same effect by by string
set RS [xmstring 119 "#1" "#2" "#3" "#4" "#5" "#6" "#7" "#8" "#9" "#10"]
puts "$RS (string)"

#
# as mentioned above, do the sample name thing using the utility
xmprint 205 "Tcl"

#
# shut er down
xmquit

exit


