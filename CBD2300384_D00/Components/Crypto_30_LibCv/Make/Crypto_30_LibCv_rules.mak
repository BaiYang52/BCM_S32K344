############################################################################### 
# File Name  : Crypto_30_LibCv_rules.mak 
# Description: Rules makefile 
#------------------------------------------------------------------------------
# COPYRIGHT
#------------------------------------------------------------------------------
# Copyright (c) 2020 by Vector Informatik GmbH.  All rights reserved.
#------------------------------------------------------------------------------
# REVISION HISTORY
#------------------------------------------------------------------------------
# Version   Date        Author  Description
#------------------------------------------------------------------------------
# 1.00.00   2016-11-24  vismss  Creation for ASR 4.3
# 1.01.00   2019-02-06  vircbl  Added support of component-based SIP structure
# 1.02.00   2020-02-07  vistof	Add Crypto_30_LibCv_KeyDerive.c and Crypto_30_LibCv_KeyEchange.c
# 1.03.00   2020-04-27  visenc	Add Crypto_30_LibCv_KeyGenerate.c
#------------------------------------------------------------------------------
# TemplateVersion = 1.03
###############################################################################


###############################################################
# REGISTRY
#

#e.g.: LIBRARIES_TO_BUILD      +=    $(LIB_OUPUT_PATH)\vendorx_canlib1.$(LIB_FILE_SUFFIX)
LIBRARIES_TO_BUILD      += Crypto_30_LibCv
Crypto_30_LibCv_FILES    = Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv.c Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_Aead.c  Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_Cipher.c Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_Hash.c Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_KeyDerive.c Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_KeyExchange.c Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_KeyManagement.c Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_Mac.c Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_Random.c  Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_Signature.c  Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_KeyGenerate.c

# e.g.: CC_FILES_TO_BUILD       += drv\can_drv.c
CC_FILES_TO_BUILD       += Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv.c Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_Aead.c  Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_Cipher.c Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_Hash.c Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_KeyDerive.c Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_KeyExchange.c Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_KeyManagement.c Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_Mac.c Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_Random.c  Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_Signature.c  Crypto_30_LibCv$(BSW_SRC_DIR)\Crypto_30_LibCv_KeyGenerate.c
CPP_FILES_TO_BUILD      +=
ASM_FILES_TO_BUILD      +=

#LIBRARIES_LINK_ONLY     += (not yet supported)
#OBJECTS_LINK_ONLY       += (not yet supported)

#-------------------------------------------------------------------------------------------------
#only define new dirs, OBJ, LIB, LOG were created automaticly 
#-------------------------------------------------------------------------------------------------
DIRECTORIES_TO_CREATE   +=

#DEPEND_GCC_OPTS         += (not yet supported)

# e.g.:  GENERATED_SOURCE_FILES += $(GENDATA_DIR)\drv_par.c
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\Crypto_30_LibCv_Cfg.c

#e.g.: COMMON_SOURCE_FILES     += $(GENDATA_DIR)\v_par.c
COMMON_SOURCE_FILES     += 

#-------------------------------------------------------------------------------------------------
# <project>.dep & <projekt>.lnk & <project>.bin and.....
# all in err\ & obj\ & lst\ & lib\ & log\ will be deleted by clean-rule automaticly
# so in this clean-rule it is only necessary to define additional files which
# were not delete automaticly.
# e.g.: $(<PATH>)\can_test.c
#-------------------------------------------------------------------------------------------------
MAKE_CLEAN_RULES        +=
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


