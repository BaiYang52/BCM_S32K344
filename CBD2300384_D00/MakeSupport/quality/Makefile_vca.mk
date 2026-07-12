########################################################################################################################
# File Name  : Makefile_vca.mk
# Description: This Sub-Makefile provides targets for VCA usage.
#
#              This file MUST NOT BE CHANGED on project integration
#
# Project    : Vector PES Build Environment
# Module     : GlobalMake 4.1
#
#-----------------------------------------------------------------------------------------------------------------------
# COPYRIGHT
#-----------------------------------------------------------------------------------------------------------------------
# Copyright (c) 2020 by Vector Informatik GmbH.                                                  All rights reserved.
#
#-----------------------------------------------------------------------------------------------------------------------
# AUTHOR IDENTITY
#-----------------------------------------------------------------------------------------------------------------------
# Name                          Initials      Company
# ----------------------------  ------------  --------------------------------------------------------------------------
# Timo Vanoni                   vistvi        Vector Informatik GmbH
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
#  Makefile_vca.mk
#  ================
#
#  This Sub-Makefile provides targets for VCA usage. It is included by Global.Makefile.target.\$(VERSION).mk.
#
#
#  Targets:
#  --------
#  - vca
#  - vca_execute_verifier
#  - vca_plugin_check
#  - vca_clean
#  - distclean
#  - help
#
#  Expected Variables:
#  -------------------
#  - `VERSION`               : Delivered by Makefile or MakeSupport 4
#  - `MAKEFILE_LIST`         : Delivered by Makefile or MakeSupport 4
#  - `PROJECT_NAME`          : Delivered by Makefile or MakeSupport 4
#  - `ROOT_U`                : Delivered by Makefile or MakeSupport 4
#
#  @ingroup mod_Makefile
#


#
# Check if Global.Makefile.target.$(VERSION).mk is already included
#
ifeq ($(filter %/Global.Makefile.target.$(VERSION).mk,$(MAKEFILE_LIST)),)
  $(error Global.Makefile.target.$(VERSION).mk must be included before Makefile_vca.mk.)
endif


VCA_OUTPUT_DIR ?= ../../../_doc/30_TestReports
ifeq ($(wildcard $(VCA_OUTPUT_DIR)/.),)
  VCA_OUTPUT_DIR = .
endif

VCA_INCLUDES=$(addprefix -I ,$(call realpath_rel,.,$(wildcard $(GCCINC_LIST_U))))
VCA_DEFINES=$(subst -D,-D ,$(filter -D%,$(CPPFLAGS))) \
             -D OS_STATIC_CODE_ANALYSIS \
             -D Vector_VCA_Tests \
             -D osdNoAsm

VCA_PLUGIN_DIRS=$(if $(wildcard $(ROOT_U)/Components),$(foreach P,$(wildcard $(ROOT_U)/Components/*/VCA),-p $(P)),) -p $(VCA_DIR)/plugins

.PHONY: vca_plugin_check
vca_plugin_check:
ifneq ($(wildcard $(VCA_DIR)/plugins),)
	@$(ECHO) "VCA       Plugin-Check"
	$(VCA_DIR)/CodeAnalyzer.exe $(VCA_PLUGIN_DIRS) --generate-check-file=$(VCA_DIR)/plugins
endif

vca_execute_verifier: $(PROJECT_NAME)_VCAReport.html

$(PROJECT_NAME)_VCAReport.html:
	@$(ECHO) "VCA       $@"
	$(Q)$(VCA_DIR)/CodeAnalyzer.exe $(VCA_INCLUDES) $(VCA_DEFINES) -r $(VCA_OUTPUT_DIR)/$@ $(VCA_PLUGIN_DIRS) ; \
	if [ $$? -gt 1 ]; then   \
	  exit 1;                \
	fi

.PHONY: vca
vca: $(PROJECT_NAME)_VCAReport.html

vca_clean:
	@$(ECHO) "CLEAN      vca"
	$(Q)$(RM) $(VCA_OUTPUT_DIR)/$(PROJECT_NAME)_VCAReport.*

help::
	@$(ECHO) " *"
	@$(ECHO) " *   VCA - Targets:"
	@$(ECHO) " *   ---------------"
	@$(ECHO) " *    vca                              Execute VCA"
	@$(ECHO) " *    vca_execute_verifier             Execute VCA verifier"
	@$(ECHO) " *    vca_plugin_check                 Do Plugin Check for VCA"
	@$(ECHO) " *    vca_clean                        Delete VCA report"
	@$(ECHO) " *"
	@$(ECHO) " ***********************************************************************************"
