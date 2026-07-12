###############################################################################
# File Name  : Arti_rules.mak
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
# 1.00.00   2022-07-15  virleh  Initial Version
#------------------------------------------------------------------------------
# TemplateVersion = 1.02
###############################################################################

# Component Files
CC_FILES_TO_BUILD       += Arti$(BSW_SRC_DIR)\Arti.c
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\Arti_Lcfg.c

# Library Settings
LIBRARIES_TO_BUILD      +=
ARTI_FILES              +=
