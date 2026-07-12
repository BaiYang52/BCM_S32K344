############################################################################### 
# File Name  : WdgM_rules.mak 
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
# 1.00.00   2016-02-18  chl     Initial Version of migrated WdgM_rules.mak
# 1.01.00   2018-04-27  chl     Additional generated make file added
#                            New template used
# 1.02.00   2019-02-06  vircbl  Added support of component-based SIP structure
#------------------------------------------------------------------------------
# TemplateVersion = 1.02
###############################################################################

# Component Files
CC_FILES_TO_BUILD       += WdgM$(BSW_SRC_DIR)\WdgM.c WdgM$(BSW_SRC_DIR)\WdgM_Checkpoint.c
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\WdgM_Cfg.c

# Include additional generated make file. File(s) will be added to GENERATED_SOURCE_FILES
include $(GENDATA_DIR)\mak\WdgM_rules_generated.mak

# Library Settings
LIBRARIES_TO_BUILD      += WdgM
WdgM_FILES              += WdgM$(BSW_SRC_DIR)\WdgM.c WdgM$(BSW_SRC_DIR)\WdgM_Checkpoint.c

