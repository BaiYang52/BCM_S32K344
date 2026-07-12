############################################################################### 
# File Name  : Eep_30_XXi2c01_rules.mak 
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
# 1.00.00   2007-06-13  virou   Initial creation of makefiles
# 1.01.00   2017-05-30  virgmi  Added Eep_30_XXi2c01_Proc.c
# 1.02.00   2019-01-22  virmid  Added support of component-based SIP structure
#------------------------------------------------------------------------------
# TemplateVersion = 1.02
###############################################################################

# Component Files
CC_FILES_TO_BUILD       += Eep_30_XXi2c01$(BSW_SRC_DIR)\Eep_30_XXi2c01.c \
                           Eep_30_XXi2c01$(BSW_SRC_DIR)\Eep_30_XXi2c01_Proc.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\Eep_30_XXi2c01_PBcfg.c \
                           $(GENDATA_DIR)\Eep_30_XXi2c01_Lcfg.c

# Library Settings
LIBRARIES_TO_BUILD      += Eep_30_XXi2c01
Eep_30_XXi2c01_FILES    += Eep_30_XXi2c01$(BSW_SRC_DIR)\Eep_30_XXi2c01.c \
                           Eep_30_XXi2c01$(BSW_SRC_DIR)\Eep_30_XXi2c01_Proc.c

