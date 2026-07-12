############################################################################### 
# File Name  : E2E_cfg.mak 
# Description: Configuration makefile 
#------------------------------------------------------------------------------
# COPYRIGHT
#------------------------------------------------------------------------------
# Copyright (c) 2021 by Vector Informatik GmbH.  All rights reserved.
#------------------------------------------------------------------------------
# REVISION HISTORY
#------------------------------------------------------------------------------
# Refer to the E2E_rules.mak file.
############################################################################### 

###############################################################
# Use PROFILE 1
#
# Possible Values:
# 0 - Not used
# 1 - Used
###############################################################
E2E_USE_PROFILE_01 = 0

###############################################################
# Use PROFILE 2
#
# Possible Values:
# 0 - Not used
# 1 - Used
###############################################################
E2E_USE_PROFILE_02 = 0

###############################################################
# Use PROFILE 4
#
# Possible Values:
# 0 - Not used
# 1 - Used
###############################################################
E2E_USE_PROFILE_04 = 0

###############################################################
# Use PROFILE 5
#
# Possible Values:
# 0 - Not used
# 1 - Used
###############################################################
E2E_USE_PROFILE_05 = 0

###############################################################
# Use PROFILE 6
#
# Possible Values:
# 0 - Not used
# 1 - Used
###############################################################
E2E_USE_PROFILE_06 = 0

###############################################################
# Use PROFILE 7
#
# Possible Values:
# 0 - Not used
# 1 - Used
###############################################################
E2E_USE_PROFILE_07 = 0

###############################################################
# Use PROFILE 8
#
# Possible Values:
# 0 - Not used
# 1 - Used
###############################################################
E2E_USE_PROFILE_08 = 0

###############################################################
# Use PROFILE 11
#
# Possible Values:
# 0 - Not used
# 1 - Used
###############################################################
E2E_USE_PROFILE_11 = 0

###############################################################
# Use PROFILE 22
#
# Possible Values:
# 0 - Not used
# 1 - Used
###############################################################
E2E_USE_PROFILE_22 = 0

###############################################################
# Use PROFILE 44
#
# Possible Values:
# 0 - Not used
# 1 - Used
###############################################################
E2E_USE_PROFILE_44 = 0

###############################################################
# Use PROFILJLR
#
# Possible Values:
# 0 - Not used
# 1 - Used
###############################################################
E2E_USE_PROFILE_JLR = 0

##############################################################
# E2E_CONFIG_PATH: (not used for Vector)
# The Variable E2E_CONFIG_PATH is optional. It allows the
# configuration of the directory, where you can also find the
# generated source files. Generally they should be in the
# directory $(PROJECT_ROOT)\source\networks.
# But if the application should run on several hardware
# targets, then different E2E configurations of different
# directories will be necessary. Therefore the variable
# E2E_CONFIG_PATH should be used.
#
# For example:
# E2E_CONFIG_PATH = $(PROJECT_ROOT)\source\network\v850
# E2E_CONFIG_PATH = $(PROJECT_ROOT)\source\network\st10
# E2E_CONFIG_PATH = $(PROJECT_ROOT)\source\network\v850
#

#E2E_CONFIG_PATH =


##############################################################
# E2E_DRIVER_MODE:  (not used for Vector)
# Not relevant for module E2E, leave it as it is.
#
# For example:
# E2E_DRIVER_MODE = singlechannel/multichannel
#

#E2E_DRIVER_MODE = singlechannel


###############################################################
# REQUIRED
#
# No Other resources are required to write a Configuration
# Makefile.
#
###############################################################


###############################################################
# PROVIDE
#
# A Configuration Makefile does not provide resources for other
# basic software bundles or the base_make package
#
###############################################################


###############################################################
# REGISTRY
#
# Like the Provide-Section this block is empty.
#
###############################################################


###############################################################
# SPECIFIC
#
# All variables in the _cfg makefile are "Specific" variables,
# because they should only be used in the *_check.mak,
# *_defs.mak and *_rules.mak Makefiles of the same basic
# software bundle.
###############################################################
