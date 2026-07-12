#!/usr/bin/sed -Ef
#
#  This sed script convert compiler generated
#  dependency files to cygwin make file format
#
#  This script can be called:
#  - fix_dos_dep_file.sed < input_dep > output_dep
#  or
#  - sed -Ef fix_dos_dep_file.sed [<] input_dep > output_dep
#  or fix inplace '-i'
#  - sed -Eif fix_dos_dep_file.sed input_dep
#
# Tested platforms:
#  - IAR
#

# Erase file with ", which leads to system headers and/or blanks in path which we don't handle
s/"[^"]*"//g

# convert \ -> / except end-of-line
s?\\([^\r\n])?/\1?g

# convert d:\ -> /cygdrive/d/
s?(^|[[:blank:]]+)([A-Za-z]):/?\1/cygdrive/\l\2/?g

# erase empty target rules ( no sources )
s/^[^:]*:[[:blank:]]*$//g

# erase empty rules " : "
s/^[[:blank:]]*:[[:blank:]]*$//g

# erase empty lines
/^[[:blank:]]*$/d
