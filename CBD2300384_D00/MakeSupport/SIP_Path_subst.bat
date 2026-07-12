@echo off

rem "***************************************************************************"
rem "*"
rem "*  This script creates a substitution path which helps to reduce"
rem "*  compiler include path lengths."
rem "*"
rem "*  Please adapt SIP_PATH to location which is root for compiler and"
rem "*  assign ROOT in Makefile.project.part.defines to location"
rem "*"
rem "*  In Linux the problem of long path names could be solved by"
rem "*  using symbolic links via 'ln -s <target_dir> <local_short_dir>' command"
rem "*"
rem "***************************************************************************"

set SIP_PATH=%~dp0..
echo Set SIP Path='%SIP_PATH%' to B:
subst b: %SIP_PATH%
