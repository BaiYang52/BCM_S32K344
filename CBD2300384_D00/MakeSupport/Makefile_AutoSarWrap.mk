########################################################################################################################
# File Name  : Makefile_AutoSarWrap.mk
# Description: This Sub-Makefile encapsulates the AUTOSAR Makefiles to a common set of variables.
#              Another encapsulation is Makefile_NoAutoSarWrap.mk
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
#  Makefile_AutoSarWrap.mk
#  =======================
#
#  This Sub-Makefile encapsulates the AUTOSAR Makefiles to a common set of variables.
#  It is included by Global.Makefile.target.\$(VERSION).mk if the value of `USE_AUTOSAR_MAKE` is 1 (delivery type is MSR).
#
#  @ingroup mod_Makefile
#


################################################################################
#
#  function mod2lib(module)
#
#  Get lib name from module name
#
mod2lib=$(strip $(if $(call wildcard_dir,$(ROOT_U)/$(GLOBAL_COMP_DIR)/,$(subst $(GLOBAL_COMP_DIR)/,,$(call os_path,$($(1)_PATH)))),$(subst $(GLOBAL_COMP_DIR)/,,$(call os_path,$($(1)_PATH))),$(subst $(GLOBAL_DEMOCOMP_DIR)/,,$(call os_path,$($(1)_PATH)))))

ifeq ($(VV),1)
  $(info )
  $(info include AUTOSAR - Makefiles)
  $(info '  $(foreach M,$(MODULE_USED_LST),$(call mod2lib,$(M)) )')
endif

include  $(foreach _M,$(MODULE_USED_LST),$(BSW_DIR)/$(call mod2lib,$(_M))/$(BSW_MAKE_DIR)/$(call mod2lib,$(_M))_defs.mak)
include  $(foreach _M,$(MODULE_USED_LST),$(BSW_DIR)/$(call mod2lib,$(_M))/$(BSW_MAKE_DIR)/$(call mod2lib,$(_M))_rules.mak)
-include $(foreach _M,$(MODULE_USED_LST),$(BSW_DIR)/$(call mod2lib,$(_M))/$(BSW_MAKE_DIR)/$(call mod2lib,$(_M))_check.mak)

include  $(foreach _M,$(MODULE_USED_LST),$(DEMOBSW_DIR)/$(call mod2lib,$(_M))/$(BSW_MAKE_DIR)/$(call mod2lib,$(_M))_defs.mak)
include  $(foreach _M,$(MODULE_USED_LST),$(DEMOBSW_DIR)/$(call mod2lib,$(_M))/$(BSW_MAKE_DIR)/$(call mod2lib,$(_M))_rules.mak)
-include $(foreach _M,$(MODULE_USED_LST),$(DEMOBSW_DIR)/$(call mod2lib,$(_M))/$(BSW_MAKE_DIR)/$(call mod2lib,$(_M))_check.mak)

CC_FILES_TO_BUILD_U=$(call os_path,$(CC_FILES_TO_BUILD))
CPP_FILES_TO_BUILD_U=$(call os_path,$(CPP_FILES_TO_BUILD))

MODULE_SOURCE_LST_RAW_U  = $(addprefix $(GLOBAL_COMP_DIR)/,$(CC_FILES_TO_BUILD_U) $(CPP_FILES_TO_BUILD_U))
MODULE_SOURCE_LST_RAW_U += $(addprefix $(GLOBAL_DEMOCOMP_DIR)/,$(CC_FILES_TO_BUILD_U) $(CPP_FILES_TO_BUILD_U))

# $(MODULE_SOURCE_LST_RAW_U) with expanded wildcards (*), reduce '..' from paths, sort list and remove duplicates
MODULE_SOURCE_LST_U=$(sort $(call realpath_rel,.,$(call wildcard_dir,$(ROOT_U),$(MODULE_SOURCE_LST_RAW_U))))

##########################################################
#
#  Generate <module>_SOURCES_U
#
#  This produces the variables analog to settings in Makefile.project.part.defines not using AUTOSAR Makefiles.
#
$(foreach _M,$(MODULE_USED_LST),$(eval $(_M)_SOURCES_U=$(call wildcard_dir,$(ROOT_U),$(addprefix $(GLOBAL_COMP_DIR)/,$(call os_path,$($(call mod2lib,$(_M))_FILES))))))

##########################################################
#
#  MODULE_ASM_LST_U
#
MODULE_ASM_LST_RAW_U  = $(if $(call wildcard_dir,$(ROOT_U)/$(GLOBAL_COMP_DIR)/,$(call os_path,$(ASM_FILES_TO_BUILD))),$(addprefix $(GLOBAL_COMP_DIR)/,$(call os_path,$(ASM_FILES_TO_BUILD))),)
MODULE_ASM_LST_RAW_U += $(if $(call wildcard_dir,$(ROOT_U)/$(GLOBAL_DEMOCOMP_DIR)/,$(call os_path,$(ASM_FILES_TO_BUILD))),$(addprefix $(GLOBAL_DEMOCOMP_DIR)/,$(call os_path,$(ASM_FILES_TO_BUILD))),)

# $(MODULE_ASM_LST_RAW_U) with expanded wildcards (*), reduce '..' from paths, sort list and remove duplicates
MODULE_ASM_LST_U=$(sort $(call realpath_rel,.,$(call wildcard_dir,$(ROOT_U),$(MODULE_ASM_LST_RAW_U))))

##########################################################
#
#  MODULE_INCLUDE_SUBDIRS_U
#   Contains all directories of modules
#   Note: BSW_SRC_DIR is empty or begins with a /
#
MODULE_INCLUDE_SUBDIRS_U=$(sort $(foreach _M,$(MODULE_USED_LST), \
                               $(patsubst %,$(ROOT_U)/%$(BSW_SRC_DIR),$(call os_path,$($(_M)_PATH)))) \
                               $(call realpath_rel,.,$(addprefix $(BSW_DIR)/,$(CC_INCLUDE_PATH_U))))      

##########################################################
#
#  Handle DIRECTORIES_TO_CREATE
#
DIRECTORIES_TO_CREATE_U:=$(addprefix $(BSW_DIR),$(call os_path,$(DIRECTORIES_TO_CREATE)))
DIR_TO_CREATE_DIR_STAMPS_U:=$(addsuffix /.dirstamp,$(DIRECTORIES_TO_CREATE_U))
.SECONDARY:: $(DIR_TO_CREATE_DIR_STAMPS_U)

##########################################################
#
#  COMMON_INC
#   Note: BSW_SRC_DIR is empty or begins with a /
#
COMMON_DIR?=$(GLOBAL_COMP_DIR)/_Common$(BSW_SRC_DIR)
COMMON_INC=$(foreach dir, $(COMMON_DIR), $(ROOT_U)/$(dir))

