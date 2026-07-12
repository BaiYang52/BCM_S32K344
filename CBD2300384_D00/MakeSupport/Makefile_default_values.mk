########################################################################################################################
# File Name  : Makefile_default_values.mk
# Description: This Sub-Makefile contains default values for MakeSupport 4.
#
#              This file MUST NOT BE CHANGED on project integration
#
# Project    : Vector PES Build Environment
# Module     : GlobalMake 4.1
#
#-----------------------------------------------------------------------------------------------------------------------
# COPYRIGHT                                                                                                           
#-----------------------------------------------------------------------------------------------------------------------
# Copyright (c) 2019 by Vector Informatik GmbH.                                                  All rights reserved. 
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
# 04.00.02  2018-10-15  vircbl  Modified header, added BETA disclaimer
# 04.01.20  2019-06-03  vircbl  Updated file header and added doxygen documentation
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


## @file
#
#  Makefile_default_values.mk
#  ==========================
#
#  This Sub-Makefile contains default variable assignments for file extensions. It is included by Global.Makefile.target.\$(VERSION).mk.
#
#  @ingroup mod_Makefile
#


#
#  Extension default values
#

ifeq ($(OBJ_SUFFIX),)
  OBJ_SUFFIX=o
endif

ifeq ($(ASM_SUFFIX),)
  ASM_SUFFIX=asm
endif

ifeq ($(ERR_SUFFIX),)
  ERR_SUFFIX=err
endif

ifeq ($(LST_SUFFIX),)
  LST_SUFFIX=lst
endif

ifeq ($(LNK_SUFFIX),)
  LNK_SUFFIX=lnk
endif

ifeq ($(LIB_SUFFIX),)
  LIB_SUFFIX=lib
endif

ifeq ($(DEP_SUFFIX),)
  DEP_SUFFIX=dep
endif

ifeq ($(LST_SUFFIX_LIST),)
  LST_SUFFIX_LIST = lst
endif
ifeq ($(ERR_SUFFIX_LIST),)
  ERR_SUFFIX_LIST = err
endif
