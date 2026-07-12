############################################################################### 
# File Name  : CanTrcv_30_Tja1043_rules.mak 
# Description: Rules makefile 
#------------------------------------------------------------------------------
# COPYRIGHT
#------------------------------------------------------------------------------
# Copyright (c) 2019 by Vector Informatik GmbH.  All rights reserved.
#------------------------------------------------------------------------------
# REVISION HISTORY
#------------------------------------------------------------------------------
# Version   Date        Author  Description
#------------------------------------------------------------------------------
# 1.00.00   2007-06-13  Aba     Initial Version of Template (1.0)
# 1.01.00   2007-08-14  Ces     Adapted to AUTOSAR CanTrcv
# 1.02.00   2008-03-25  Fn      Adapted to AUTOSAR 3.0
# 1.03.00   2008-04-30  fbg     Changes for ASR library support
# 1.04.00   2009-04-17  Tvi     Changed Paths / Renamed files
# 1.05.00   2009-11-11  Tvi     Renamed files
# 1.06.00   2010-05-02  Tvi     Renamed files
# 1.07.00   2019-02-06  vircbl  Added support of component-based SIP structure
#------------------------------------------------------------------------------
# TemplateVersion = 1.02
###############################################################################


###############################################################
# REGISTRY
#

#e.g.: LIBRARIES_TO_BUILD +=    $(LIB_OUPUT_PATH)\vendorx_canlib1.$(LIB_FILE_SUFFIX)
LIBRARIES_TO_BUILD      += CanTrcv_30_Tja1043
CanTrcv_30_Tja1043_FILES    = CanTrcv_30_Tja1043$(BSW_SRC_DIR)\CanTrcv_30_Tja1043.c

# e.g.: CC_FILES_TO_BUILD += drv\CanTrcv_30_Tja1043dio.c
CC_FILES_TO_BUILD       += CanTrcv_30_Tja1043$(BSW_SRC_DIR)\CanTrcv_30_Tja1043.c
CPP_FILES_TO_BUILD      +=
ASM_FILES_TO_BUILD      +=

#LIBRARIES_LINK_ONLY     += (not yet supported)
#OBJECTS_LINK_ONLY       += (not yet supported)

#-------------------------------------------------------------------------------------------------
#only define new dirs, OBJ, LIB, LOG were created automaticly 
#-------------------------------------------------------------------------------------------------
DIRECTORIES_TO_CREATE   +=

#DEPEND_GCC_OPTS        += (not yet supported)

# e.g.:  GENERATED_SOURCE_FILES += $(GENDATA_DIR)\drv_par.c
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\CanTrcv_30_Tja1043_Cfg.c

#e.g.: COMMON_SOURCE_FILES     += $(GENDATA_DIR)\v_par.c
COMMON_SOURCE_FILES     += 

#-------------------------------------------------------------------------------------------------
# <project>.dep & <projekt>.lnk & <project>.bin and.....
# all in err\ & obj\ & lst\ & lib\ & log\ will be deleted by clean-rule automaticly
# so in this clean-rule it is only necessary to define additional files which
# were not delete automaticly.
# e.g.: $(<PATH>)\can_test.c
#-------------------------------------------------------------------------------------------------
MAKE_CLEAN_RULES         +=
#MAKE_GENERATE_RULES     +=
#MAKE_COMPILER_RULES     +=
#MAKE_DEBUG_RULES        +=
#MAKE_CONFIG_RULES       +=
#MAKE_ADD_RULES          +=


###############################################################
# REQUIRED   (defined in BaseMake (global.Makefile.target.make...))
#
# SSC_ROOT		(required)
# PROJECT_ROOT	(required)
#
# LIB_OUTPUT_PATH	(optional)
# OBJ_OUTPUT_PATH	(optional)
#
# OBJ_FILE_SUFFIX	
# LIB_FILE_SUFFIX
#
###############################################################


###############################################################
# PROVIDE   this Section can be used to define own additional rules
#
# In vendorx_can_cfg.mak:
# Please configure the project file:
#CAN_CONFIG_FILE = $(PROJECT_ROOT)\source\network\can\my_can_config.cfg

#In vendorx_can_config :
#generate_can_config:
#$(SSC_ROOT)\core\com\can\tools\canconfiggen.exe -o $(CAN_CONFIG_FILE)


###############################################################
# SPECIFIC
#
# There are no rules defined for the Specific part of the 
# Rules-Makefile. Each author is free to create temporary 
# variables or to use other resources of GNU-MAKE
#
###############################################################


