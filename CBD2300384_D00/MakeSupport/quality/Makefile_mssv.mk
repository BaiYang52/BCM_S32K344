########################################################################################################################
# File Name  : Makefile_mssv.mk
# Description: This Sub-Makefile provides targets for MSSV usage.
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
#  Makefile_mssv.mk
#  ================
#
#  This Sub-Makefile provides targets for MSSV usage. It is included by Global.Makefile.target.\$(VERSION).mk.
#
#
#  Targets:
#  --------
#  - mssv
#  - mssv_convert_report
#  - mssv_execute_verifier
#  - mssv_plugin_check
#  - mssv_clean
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
  $(error Error: Global.Makefile.target.$(VERSION).mk must be included before Makefile.mssv.mk.)
endif


MSSV_OUTPUT_DIR ?= ../../../_doc/30_TestReports
ifeq ($(wildcard $(MSSV_OUTPUT_DIR)/.),)
  MSSV_OUTPUT_DIR = .
endif

MSSV_INCLUDES=$(addprefix -i ,$(call realpath_rel,.,$(wildcard $(GCCINC_LIST_U))))
MSSV_DEFINES=$(subst -D,-D ,$(filter -D%,$(CPPFLAGS))) \
             -D OS_STATIC_CODE_ANALYSIS \
             -D Vector_MSSV_Tests \
             -D osdNoAsm \
             -D __GNUC__

MSSV_LICENSE_FILE=$(or $(wildcard $(ROOT_U)/Components/Os/ComponentLicense_Os.lic),  \
                       $(ROOT_U)/SipLicense.lic)

MSSV_PLUGIN_DIRS=$(or $(if $(wildcard $(ROOT_U)/Components),$(foreach P,$(wildcard $(ROOT_U)/Components/*/MSSV),-p $(P))),-p $(MSSV_DIR)/plugins)

.PHONY: mssv_plugin_check
mssv_plugin_check:
ifneq ($(wildcard $(MSSV_DIR)/plugins),)
  ifneq ($(wildcard $(MSSV_DIR)/plugins/PluginCheckFileGenerator.exe),)
	@$(ECHO) "MSSV       Plugin-Check"
	$(Q)$(MSSV_DIR)/plugins/PluginCheckFileGenerator.exe -i $(MSSV_LICENSE_FILE) -o $(MSSV_DIR)/plugins/ \
	                                                     $(MSSV_PLUGIN_DIRS) -c $(MSSV_DIR)/plugins/mssv.config.xml
  endif
endif

mssv_execute_verifier: $(PROJECT_NAME)_MSSVReport.html

$(PROJECT_NAME)_MSSVReport.html: $(wildcard Makefile.config Makefile.config.generated Makefile.project.part.defines Makefile.static) Makefile
	@$(ECHO) "MSSV       $@"
	$(Q)$(MSSV_DIR)/MSSV.exe $(MSSV_INCLUDES) $(MSSV_DEFINES) -l $(basename $@).log $(MSSV_PLUGIN_DIRS) \
	                         $(if $(wildcard $(ROOT_U)/Components),-p $(MSSV_DIR)/plugins) -r $(MSSV_OUTPUT_DIR)/$@ ; \
	if [ $$? -gt 1 ]; then   \
	  exit 1;                \
	fi

mssv mssv_convert_report: $(PROJECT_NAME)_MSSVReport.xml

$(PROJECT_NAME)_MSSVReport.xml: $(PROJECT_NAME)_MSSVReport.html
	@$(ECHO) "MSSV       $@"
	$(Q)./MSSVReportToXml.exe -i $(MSSV_OUTPUT_DIR)/$< -o $(MSSV_OUTPUT_DIR)/$@

mssv_clean:
	@$(ECHO) "CLEAN      mssv"
	$(Q)$(RM) $(MSSV_OUTPUT_DIR)/$(PROJECT_NAME)_MSSVReport.* $(PROJECT_NAME)_MSSVReport.*

distclean::
	@$(ECHO) "DISTCLEAN  mssv"
	$(Q)$(RM) $(PROJECT_NAME)_MSSVReport.log

help::
	@$(ECHO) " *"
	@$(ECHO) " *   MSSV - Targets:"
	@$(ECHO) " *   ---------------"
	@$(ECHO) " *    mssv                             Execute MSSV"
	@$(ECHO) " *    mssv_convert_report              Convert MSSV report from HTML to XML"
	@$(ECHO) " *    mssv_execute_verifier            Execute MSSV verifier"
	@$(ECHO) " *    mssv_plugin_check                Do Plugin Check for MSSV"
	@$(ECHO) " *    mssv_clean                       Delete MSSV report"
	@$(ECHO) " *"
	@$(ECHO) " ***********************************************************************************"
