########################################################################################################################
# File Name  : Makefile_AutoSar_library.mk
# Description: This Sub-Makefile provides library creation targets for MICROSAR deliveries.
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
# Claudia Buhl                  vircbl        Vector Informatik GmbH
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
#  Makefile_AutoSar_library.mk
#  ===========================
#
#  This file provides library creation targets.
#  It is included by Global.Makefile.target.\$(VERSION).mk if the value of `USE_AUTOSAR_MAKE` is 1 (delivery type is MICROSAR).
#
#
#  Targets:
#  --------
#  - libs, library
#  - copylibs
#  - printlibs
#  - printlibfiles
#  - cleanlibs
#  - clean
#
#  @ingroup mod_Makefile
#


.PHONY: library libs
library libs:: $(foreach _M,$(LIBRARIES_TO_BUILD),$(LIB_PATH)/lib$(_M).$(LIB_SUFFIX))

.SECONDARY:: $(LIB_PATH)/.dirstamp

###########################################################
#
#  Generate <MODULE>_FILES_U
#
#  Example:
#    Can_FILES_U = Can\Can.c
#
$(foreach _L,$(LIBRARIES_TO_BUILD),$(eval $(_L)_FILES_U=$(call wildcard_dir_u,$(BSW_DIR),$($(_L)_FILES))))

#
#  AR_RULE <mod>
#
#  specify AUTOSAR library rule
#
define AR_RULE
$(LIB_PATH)/lib$(1).$(LIB_SUFFIX): $(if $(wildcard $(addprefix $(ROOT_U)/$(GLOBAL_COMP_DIR)/,$($(1)_FILES_U))),$(addprefix $(OBJ_PATH)/$(ROOT_FROM_PRJROOT)/$(GLOBAL_COMP_DIR)/,$(patsubst %.c,%.o,$(patsubst %.asm,%.o,$($(1)_FILES_U))))) \
                                   $(if $(wildcard $(addprefix $(ROOT_U)/$(GLOBAL_DEMOCOMP_DIR)/,$($(1)_FILES_U))),$(addprefix $(OBJ_PATH)/$(ROOT_FROM_PRJROOT)/$(GLOBAL_DEMOCOMP_DIR)/,$(patsubst %.c,%.o,$(patsubst %.asm,%.o,$($(1)_FILES_U))))) \
                                   $(LIB_PATH)/.dirstamp
	@$(ECHO) "AR         lib$(1).$(LIB_SUFFIX)"
ifeq ($(V),1)
	@$(ECHO) "$(1) - sources: $(patsubst %.c,%.o,$(patsubst %.asm,%.o,$($(1)_FILES_U))) "
endif
	@$(RM) $@
	$(Q)$(AR_ENV) $(AR) $(ARFLAGS) $(LIB_PATH)/lib$(1).$(LIB_SUFFIX) $(addprefix $(OBJ_PATH)/$(ROOT_FROM_PRJROOT)/$(GLOBAL_COMP_DIR)/,$(patsubst %.c,%.o,$(patsubst %.asm,%.o,$($(1)_FILES_U)))) $(AR_REDIRECT_OUTPUT)
endef

OBJS_LIB += $(foreach _L,$(LIBRARIES_TO_BUILD),$(if $(call wildcard_dir,$(ROOT_U)/$(GLOBAL_COMP_DIR)/,$($(_L)_FILES_U)),$(addprefix $(OBJ_PATH)/$(ROOT_FROM_PRJROOT)/$(GLOBAL_COMP_DIR)/,$(patsubst %.c,%.o,$(patsubst %.asm,%.o,$($(_L)_FILES_U)))),))
OBJS_LIB += $(foreach _L,$(LIBRARIES_TO_BUILD),$(if $(call wildcard_dir,$(ROOT_U)/$(GLOBAL_DEMOCOMP_DIR)/,$($(_L)_FILES_U)),$(addprefix $(OBJ_PATH)/$(ROOT_FROM_PRJROOT)/$(GLOBAL_DEMOCOMP_DIR)/,$(patsubst %.c,%.o,$(patsubst %.asm,%.o,$($(_L)_FILES_U)))),))
BSW_LIBRARIES += $(foreach _M,$(LIBRARIES_TO_BUILD),$(LIB_PATH)/lib$(_M).$(LIB_SUFFIX))

#
#  Generate all rules for AUTOSAR Makefiles
#
$(foreach _L,$(LIBRARIES_TO_BUILD),$(eval $(call AR_RULE,$(_L))))


############################################
#
#  copylibs section
#
.PHONY: copylibs
copylibs:
	@$(ECHO) "Copy libraries to source directory of components:"
	$(Q)$(foreach LIB,$(LIBRARIES_TO_COPY),  $(if $(wildcard $(ROOT_U)/$(GLOBAL_COMP_DIR)/$(LIB)),$(ECHO) "  lib$(LIB)"; $(CP) $(LIB_PATH)/lib$(LIB).$(LIB_SUFFIX) -t $(ROOT_U)/$(GLOBAL_COMP_DIR)/$(LIB)$(BSW_SRC_DIR)/ ;,))
	$(Q)$(foreach LIB,$(LIBRARIES_TO_COPY),  $(if $(wildcard $(ROOT_U)/$(GLOBAL_DEMOCOMP_DIR)/$(LIB)),$(ECHO) "  lib$(LIB)"; $(CP) $(LIB_PATH)/lib$(LIB).$(LIB_SUFFIX) -t $(ROOT_U)/$(GLOBAL_DEMOCOMP_DIR)/$(LIB)$(BSW_SRC_DIR)/ ;,))


############################################
#
#  printlibs section
#
.PHONY: printlibs
printlibs:
	@$(ECHO) "Available libraries:"
	@$(foreach LIB,$(LIBRARIES_TO_BUILD), $(ECHO) "  $(LIB)";)

printlibfiles:
	@$(ECHO) "Available libraries:"
	@$(foreach LIB,$(LIBRARIES_TO_BUILD), $(ECHO);$(ECHO) "  lib$(LIB)"; $(foreach F,$($(LIB)_FILES),$(ECHO) "    $(F)";))


cleanlibs::
	@echo "CLEAN      libraries"
	$(Q)$(RM) $(foreach MODULE,$(LIBRARIES_TO_BUILD),$(LIB_PATH)/lib$(MODULE).$(LIB_SUFFIX))

clean:: cleanlibs