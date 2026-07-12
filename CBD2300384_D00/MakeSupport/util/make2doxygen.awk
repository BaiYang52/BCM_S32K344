#!/usr/bin/awk -f
#######################################################################################################################
# File Name  : make2doxygen.awk                                                                                       #
# Description: This awk-script is used to extract documentation from a Makefile and make it usabe to doxygen.         #
#              doxygen can handle comments from c or other languages, but not from Makefile.                          #
#              To solve this problem the comment specifiers '#' are modified to '///' comments from C++                #
#              when the first line has a                                                                              #
#                ## @file                                                                                             #
#              or                                                                                                     #
#                ## @defgroup                                                                                         #
#              pattern                                                                                                #
#                                                                                                                     #
#              Usage:                                                                                                 #
#              $  make2doxygen.awk < Makefile > Makefile.c                                                            #
#              or                                                                                                     #
#              $  awk -f make2doxygen.awk < Makefile > Makefile.c                                                     #
#                                                                                                                     #
#                                                                                                                     #
#              Doxyfile:                                                                                              #
#                INPUT=Makefile.c ...                                                                                 #
#                                                                                                                     #
#              This file MUST NOT BE CHANGED on project integration                                                   #
#                                                                                                                     #
# Project    : Vector PES Buildenvironment                                                                            #
# Module     : GlobalMake 4.0                                                                                         #
#                                                                                                                     #
#---------------------------------------------------------------------------------------------------------------------#
# COPYRIGHT                                                                                                           #
#---------------------------------------------------------------------------------------------------------------------#
# Copyright (c) 2018 by Vector Informatik GmbH.                                                  All rights reserved. #
#                                                                                                                     #
#---------------------------------------------------------------------------------------------------------------------#
# AUTHOR IDENTITY                                                                                                     #
#---------------------------------------------------------------------------------------------------------------------#
# Name                          Initials      Company                                                                 #
# ----------------------------  ------------  ------------------------------------------------------------------------#
# Clemens von Mann              vircvo        Vector Informatik GmbH                                                  #
#---------------------------------------------------------------------------------------------------------------------#
# REVISION HISTORY                                                                                                    #
#---------------------------------------------------------------------------------------------------------------------#
# Version   Date        Author  Description                                                                           #
# --------  ----------  ------  --------------------------------------------------------------------------------------#
# 04.00.00  2018-09-26  vircvo  Initial Release                                                                       #
#######################################################################################################################

######################################################################
#
#  This AWK script extracts documentation from Makefile
#  to make it available in doxygen
#
#  Each documentation section begins with a
#    ## @file
#  or
#    ## @defgroup
#  and continues every line start with '#'
#
#  There could be more than one section in a file.
#
#  Example:
#  --------
#  ## @file
#  #
#  #  This is a doxygen documentation
#  #
#  #  This is the last line of doxygen documentation
#  <any other line>
#
######################################################################

BEGIN				                   {
                                  flag=0;
                               }
/^##[ \t]*(@file|@defgroup)/  {
                                  flag=1;
                                  sub(/##[ \t]*@file/,"/// @file");
                                  sub(/##[ \t]*@defgroup/,"/// @defgroup");
                                  print $0
                                  next;
                                }
/^#/                            {
                                  if(flag == 0)
                                    next;
                                  sub(/^#/,"///");
                                  print $0
                                  next
                                }
/^[^#]|^$$/                     {
                                  if(flag == 1)
                                    print ""
                                  flag=0;
                                  next;
                                }
