########################################################################################################################
# File Name  : Makefile_BSW_vars_ComponentBased.mk
# Description: This makefile contains variables for setting directory names in a SIP with component-based structure.
#
#              This file MUST NOT BE CHANGED on project integration
#
# Project    : Vector PES Build Environment
# Module     : GlobalMake 4.1
#
#-----------------------------------------------------------------------------------------------------------------------
# COPYRIGHT
#-----------------------------------------------------------------------------------------------------------------------
# Copyright (c) 2021 by Vector Informatik GmbH.                                                  All rights reserved. 
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
# Refer to the file Global.Makefile.target.4.mk.
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
#  Makefile_BSW_vars_ComponentBased.mk
#  ===================================
#
#  This Sub-Makefile contains variables for setting directory names in a SIP.
#  It is included by Global.Makefile.target.4.mk if modules are located in folder 'Components' (component-based SIP structure).
#
#  @ingroup mod_Makefile
#


#
#  Set variables for component-based SIP structure
#
BSW_MAKE_DIR?=Make
GLOBAL_COMP_DIR?=Components
GLOBAL_DEMOCOMP_DIR?=DemoComponents
BSW_DIR:=$(ROOT_U)/$(GLOBAL_COMP_DIR)
DEMOBSW_DIR:=$(ROOT_U)/$(GLOBAL_DEMOCOMP_DIR)
#   Note: BSW_SRC_DIR is empty or begins with a / and does not end with a /
BSW_SRC_DIR:=/Implementation

MSSV_DIR:=$(ROOT_U)/Misc/MSSV/Application
VCA_DIR:=$(ROOT_U)/Misc/VCA/Application

#  function file2mod(filepath)
#
#  @return module of file
#
# Note:
# * $(dir ...) returns a trailing slash according to GNU Make documentation
# * $(patsubst pattern,replacement,text) with wildcards (%) only matches
#   when the part after the "%" in pattern exactly matches the end of text.
#   text must not contain additional characters at the end
# * Therefor add a "/" to BSW_SRC_DIR
file2mod=$(notdir $(patsubst %$(BSW_SRC_DIR)/,%,$(dir $(1))))
