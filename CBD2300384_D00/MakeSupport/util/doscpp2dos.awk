#!awk -f
########################################################################
#
#  This script is used to postprocess cpp - preprocessing.
#
#  Problem:
#    cpp create a new line for each \r. Result is a \r\n are interpreted
#    as 2 lines.
#
#  This erases a new line after a normal line only. So Source can
#  containe empty lines as well.
#
#  the output is a windows \r\n file as well.
#
########################################################################

BEGIN                  {
                         has_line=1;
                       }

#
# empty line after a normal line -> erase
#
has_line==1 && /^$/    {
                         has_line=0;
                         next;
                       }

#
# normal line after a normal line -> print
#
/^.+$/                 {
                             printf "%s\r\n", $0;
                             has_line=1;
                             next
                       }

#
# empty line after an empty line -> print empty line
#
has_line==0 && /^$/    {
                         printf "\r\n";
                         has_line=1;
                         next;
                       }

