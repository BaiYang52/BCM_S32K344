############################################################################### 
# File Name  : E2E_check.mak 
# Description: Configuration check makefile 
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
# Support PROFILE 1
#
# Check if set
###############################################################
ifeq ($(E2E_USE_PROFILE_01),)
  $(error Error: E2E_check.mak: Please set E2E_USE_PROFILE_01 in E2E_cfg.mak)
endif

###############################################################
# Support PROFILE 2
#
# Check if set
###############################################################
ifeq ($(E2E_USE_PROFILE_02),)
  $(error Error: E2E_check.mak: Please set E2E_USE_PROFILE_02 in E2E_cfg.mak)
endif

###############################################################
# Support PROFILE 4
#
# Check if set
###############################################################
ifeq ($(E2E_USE_PROFILE_04),)
  $(error Error: E2E_check.mak: Please set E2E_USE_PROFILE_04 in E2E_cfg.mak)
endif

###############################################################
# Support PROFILE 5
#
# Check if set
###############################################################
ifeq ($(E2E_USE_PROFILE_05),)
  $(error Error: E2E_check.mak: Please set E2E_USE_PROFILE_05 in E2E_cfg.mak)
endif

###############################################################
# Support PROFILE 6
#
# Check if set
###############################################################
ifeq ($(E2E_USE_PROFILE_06),)
  $(error Error: E2E_check.mak: Please set E2E_USE_PROFILE_06 in E2E_cfg.mak)
endif

###############################################################
# Support PROFILE 7
#
# Check if set
###############################################################
ifeq ($(E2E_USE_PROFILE_07),)
  $(error Error: E2E_check.mak: Please set E2E_USE_PROFILE_07 in E2E_cfg.mak)
endif

###############################################################
# Support PROFILE 8
#
# Check if set
###############################################################
ifeq ($(E2E_USE_PROFILE_08),)
  $(error Error: E2E_check.mak: Please set E2E_USE_PROFILE_08 in E2E_cfg.mak)
endif

###############################################################
# Support PROFILE 11
#
# Check if set
###############################################################
ifeq ($(E2E_USE_PROFILE_11),)
  $(error Error: E2E_check.mak: Please set E2E_USE_PROFILE_11 in E2E_cfg.mak)
endif

###############################################################
# Support PROFILE 22
#
# Check if set
###############################################################
ifeq ($(E2E_USE_PROFILE_22),)
  $(error Error: E2E_check.mak: Please set E2E_USE_PROFILE_22 in E2E_cfg.mak)
endif

###############################################################
# Support PROFILE 44
#
# Check if set
###############################################################
ifeq ($(E2E_USE_PROFILE_44),)
  $(error Error: E2E_check.mak: Please set E2E_USE_PROFILE_44 in E2E_cfg.mak)
endif

###############################################################
# Support PROFILE JLR
#
# Check if set
###############################################################
ifeq ($(E2E_USE_PROFILE_JLR),)
  $(error Error: E2E_check.mak: Please set E2E_USE_PROFILE_JLR in E2E_cfg.mak)
endif


###############################################################
# REGISTRY
#

#e.g.: PREPARE_CONFIGURATION_INTERFACE     += E2E_MODULES
PREPARE_CONFIGURATION_INTERFACE     +=

CHECK_VARS_WHICH_ARE_REQUIRED       +=

CHECK_VARS_WHICH_ARE_OPTIONAL       +=

CHECK_VARS_WHICH_ARE_OBSOLETE       +=

CHECK_VARS_WITH_ONE_CC_FILE         +=
CHECK_VARS_WITH_MORE_CC_FILES       +=

CHECK_VARS_WITH_ONE_CPP_FILE        +=
CHECK_VARS_WITH_MORE_CPP_FILES      +=

CHECK_VARS_WITH_ONE_ASM_FILE        +=
CHECK_VARS_WITH_MORE_ASM_FILES      +=

CHECK_VARS_WITH_ONE_LIB_FILE        +=
CHECK_VARS_WITH_MORE_LIB_FILES      +=

CHECK_VARS_WITH_ONE_OBJ_FILE        +=
CHECK_VARS_WITH_MORE_OBJ_FILES      +=

#e.g: CHECK_VARS_WITH_ONE_DIRECTORY       +=     $(DIRECTORIES_TO_CREATE)
#       $(DIRECTORIES_TO_CREATE) = C:\demo\drv          (this var is defined in _rules.mak)
CHECK_VARS_WITH_ONE_DIRECTORY       +=
CHECK_VARS_WITH_MORE_DIRECTORIES    +=

CHECK_VARS_WITH_ONE_FILE            +=
CHECK_VARS_WITH_MORE_FILES          +=

CHECK_VARS_WITH_MAX_LENGTH_ONE      +=

###############################################################
# SPECIFIC
#
###############################################################


###############################################################
# REQUIRED
#
# No other resources are required for a Configuration Check
# Makefile
#
###############################################################


###############################################################
# PROVIDE
#
# A configuration Makefile does not provide resources for other
# basic software bundles or the base-make package.
#
###############################################################

