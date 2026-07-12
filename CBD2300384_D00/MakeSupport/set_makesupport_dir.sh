########################################################################################################################
# File Name  : set_makesupport_dir.sh
# Description: This script sets the variable MAKESUPPORT_DIR to location where this file resists.
#              This file can be sourced from anywhere.
#              Please do *NOT* call this script without source command, because it has no effect on bash.
#
#              Usage
#               $  source <path>/set_makesupport_dir
#              or
#               $  .  <path>/set_makesupport_dir
#
#              There exists also a windows variant set_makesupport_dir.bat
#
#              This file MUST NOT BE CHANGED on project integration
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
# Claudia Buhl                  vircbl        Vector Informatik GmbH
#-----------------------------------------------------------------------------------------------------------------------
# REVISION HISTORY
#-----------------------------------------------------------------------------------------------------------------------
# Version   Date        Author  Description
# --------  ----------  ------  ----------------------------------------------------------------------------------------
# 04.00.00  2018-09-26  vircvo  Initial Release
# 04.00.02  2018-10-15  vircbl  Modified header
########################################################################################################################

########################################################################################################################
#    EXAMPLE CODE ONLY
#    -------------------------------------------------------------------------------------------------------------------
#    This Example Code is only intended for illustrating an example of a possible BSW integration and BSW configuration.
#    The Example Code has not passed any quality control measures and may be incomplete. The Example Code is neither
#    intended nor qualified for use in series production. The Example Code as well as any of its modifications and/or
#    implementations must be tested with diligent care and must comply with all quality requi#ents which are necessary
#    according to the state of the art before their use.
########################################################################################################################

echo
export MAKESUPPORT_DIR=`dirname $BASH_SOURCE`
echo MAKESUPPORT_DIR=$MAKESUPPORT_DIR
echo
