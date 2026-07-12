############################################################################### 
# File Name  : Os_rules.mak 
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
# 2.00.00   2017-04-06  Bse     Initial Version
# 2.01.00   2017-08-29  Biv     TI Compiler specific file added
# 2.02.00   2017-09-08  Sso     Arm Compiler specific file added
# 2.05.00   2019-02-06  vircbl  Added support of component-based SIP structure
# 2.05.00   2019-02-11  Rework  to match the template
# 3.00.00   2020-09-28  Mun     Support to new TLM
# 3.01.00   2020-09-28  Mun     Update after ARMv8-M support
#------------------------------------------------------------------------------
# TemplateVersion = 1.02
###############################################################################

# Include of OS Core makefile

-include $(ROOT)\$(GLOBAL_COMP_DIR)\Os\$(if $(BSW_MAKE_DIR),$(BSW_MAKE_DIR),mak)\Os_Core.mak

# In case of new TLM

NEW_TLM_PATH = ..\internal\external
-include $(ROOT)\$(NEW_TLM_PATH)\$(GLOBAL_COMP_DIR)\Os\$(if $(BSW_MAKE_DIR),$(BSW_MAKE_DIR),mak)\Os_Core.mak


# Component Files

CC_FILES_TO_BUILD          += Os$(BSW_SRC_DIR)\Os_Hal_Core.c                \
                              Os$(BSW_SRC_DIR)\Os_Hal_Context.c             \
                              Os$(BSW_SRC_DIR)\Os_Hal_Entry.c               \
                              Os$(BSW_SRC_DIR)\Os_Hal_Compiler_Arm.c        \
                              Os$(BSW_SRC_DIR)\Os_Hal_Compiler_Iar.c

ASM_FILES_TO_BUILD         += Os$(BSW_SRC_DIR)\Os_Hal_ContextAsm.asm        \
                              Os$(BSW_SRC_DIR)\Os_Hal_Compiler_TiInt.asm

GENERATED_SOURCE_FILES     += $(GENDATA_DIR)\Os_Hal_Context_Lcfg.c          \
                              $(GENDATA_DIR)\Os_Hal_Core_Lcfg.c             \
                              $(GENDATA_DIR)\Os_Hal_Entry_Lcfg.c            \
                              $(GENDATA_DIR)\Os_Hal_Interrupt_Lcfg.c        \
                              $(GENDATA_DIR)\Os_Hal_Kernel_Lcfg.c           \
                              $(GENDATA_DIR)\Os_Hal_MemoryProtection_Lcfg.c \
                              $(GENDATA_DIR)\Os_Hal_Timer_Lcfg.c 
                            
ASM_GENERATED_SOURCE_FILES += $(GENDATA_DIR)\Os_Hal_EntryAsm_Lcfg.asm 

# Library Settings
# Library not supported
