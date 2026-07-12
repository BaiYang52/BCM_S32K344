#######################################################################################################################
# File Name  : path_mk_canonical.sh                                                                                   #
# Description: Script to set path spec to canonical form. This means avoid '..' in path if not needed.                #
#              This script is used to work without access to file system like realpath.                                #
#                                                                                                                     #
#              The arguments can be set as command line:                                                               #
#                $ path_mk_canonical.sh path1 [path2]..[pathn]                                                        #
#              or as stdin:                                                                                           #
#                $ echo "path1 [path2]..[pathn]" | path_mk_canonical.sh                                               #
#                $ path_mk_canonical.sh <<< "path1 [path2]..[pathn]"                                                  #
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
# 04.00.00  2018-04-04  vircvo  Initial Release                                                                       #
#######################################################################################################################

########################################################################################################################
#    EXAMPLE CODE ONLY
#    -------------------------------------------------------------------------------------------------------------------
#    This Example Code is only intended for illustrating an example of a possible BSW integration and BSW configuration.
#    The Example Code has not passed any quality control measures and may be incomplete. The Example Code is neither
#    intended nor qualified for use in series production. The Example Code as well as any of its modifications and/or
#    implementations must be tested with diligent care and must comply with all quality requirements which are necessary
#    according to the state of the art before their use.
########################################################################################################################

#
#  sed expressoins:
#
#  1.      reduce multiple '/<dir>/../' -> '/'
#  2.      reduce multiple '<dir>/../' -> './'
#  2. + 3. reduce <begin><dir>/../ -> '/'
#  4.      reduce <begin><dir>/.. -> '.'
#  5.      reduce '/<dir>/..' -> '/'
#  6.      reduce '/<dir>/..' -> '/'
#
#  Remark:
#    This script can be reduced to be more simple by calling sed multiple times.
#
#  sed -E 's?(/$(ANY)/../)+?/?g' | sed '....'    # do here begin and end with more simple expression.
#

set -e

SED_SCRIPT='

  # // -> /
  s?/+?/?g

  # /dir/../ -> /
  :again
  s?/[[:alnum:]_]+/\.\./?/?g
  t again
  
  # dir/.. -> .
  s?[[:alnum:]_]+/\.\.?.?g

  # ./<name> -> <name>
  s?\./([[:alnum:]_]+)?\1?g
  
  # <name>/. -> <name>
  s?([[:alnum:]_]+)/\.?\1?g
  
  # /./ -> /
  s?(/\./)+?+/+?g

  # ./. -> .
  s?\./\.?.?g
  
  # /. -> /
  s?/\.?/?g
'

if [ $# -ne 0 ]; then
  sed -E "$SED_SCRIPT" <<< $*
else
  sed -E "$SED_SCRIPT"
fi
