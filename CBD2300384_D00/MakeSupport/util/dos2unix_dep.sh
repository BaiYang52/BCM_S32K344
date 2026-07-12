#!sed -Ef

########################################################################################################################
# File Name  : dos2unix_dep.sh
# Description: Script to convert a dependency header file which contains dos path specs to cygwin path specs.
#
#
# Project    : Vector PES Buildenvironment
# Module     : GlobalMake 4.0
#
#-----------------------------------------------------------------------------------------------------------------------
# COPYRIGHT                                                                                                           
#-----------------------------------------------------------------------------------------------------------------------
# Copyright (c) 2018 by Vector Informatik GmbH.                                                  All rights reserved. 
#                                                                                                                     
#-----------------------------------------------------------------------------------------------------------------------
# AUTHOR IDENTITY                                                                                                     
#-----------------------------------------------------------------------------------------------------------------------
# Name                          Initials      Company                                                                 
# ----------------------------  ------------  --------------------------------------------------------------------------
# Clemens von Mann              vircvo        Vector Informatik GmbH
#-----------------------------------------------------------------------------------------------------------------------
# REVISION HISTORY                                                                                                    
#-----------------------------------------------------------------------------------------------------------------------
# Version   Date        Author  Description                                                                           
# --------  ----------  ------  ----------------------------------------------------------------------------------------
# 04.00.00  2018-09-26  vircvo  Initial Release
########################################################################################################################

########################################################################################################################
#    EXAMPLE CODE ONLY
#    -------------------------------------------------------------------------------------------------------------------
#    This Example Code is only intended for illustrating an example of a possible BSW integration and BSW configuration.
#    The Example Code has not passed any quality control measures and may be incomplete. The Example Code is neither
#    intended nor qualified for use in series production. The Example Code as well as any of its modifications and/or
#    implementations must be tested with diligent care and must comply with all quality requirements which are necessary
#    according to the state of the art before their use.
########################################################################################################################

################################################################################
#
# Usage:
#  dos2unix_dep.sh < file > outfile
# or
#  dos2unix_dep.sh file > outfile
# or
#  sed -i -E -f dos2unix_dep.sh file
#
#  sed - options:
#    -i     inplace
#    -E     use ERE (extended regular expressions)
#    -f     script file
#

#  convert \ -> / if not line continues
s%\\[^\r]?$%/%g

# D:/ -> /cygdrive/d/
s?(^|[[:blank:]]*)([A-Za-z]):/?\1/cygpath/\l\2/?g
