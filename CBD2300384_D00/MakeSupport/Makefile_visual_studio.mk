########################################################################################################################
# File Name  : Makefile_visual_studio.mk
# Description: This Sub-Makefile provides targets for generation of Visual Studio projects in context of MakeSupport 4.
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
# Version   Date        Author  Description                                                                           
# --------  ----------  ------  ----------------------------------------------------------------------------------------
# 04.01.07  2019-03-19  vircvo  Initial Release
# 04.01.11  2019-04-02  vircbl  Updated target 'help', added target 'distclean', changed target 'clean' to 'visual_clean'
# 04.01.20  2019-06-03  vircbl  Updated file header and added doxygen documentation
# 04.01.21  2019-06-03  vircbl  Updated target 'help'
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
#  Makefile_visual_studio.mk
#  =========================
#
#  This Sub-Makefile contains targets to create a Visual Studio project. It is included by Global.Makefile.target.\$(VERSION).mk.
#
#
#  Targets:
#  --------
#  - visual
#  - visual_clean
#
#  @ingroup mod_Makefile
#


.PHONY: visual10 visual12 visual
visual10 visual12 visual: VS_$(PROJECT_NAME).vcxproj VS_$(PROJECT_NAME).vcxproj.filters

VS_MAKE_FILES=Makefile                                                       \
              $(MAKEFILE_CONFIG)                                             \
              Makefile.project.part.defines                                  \
              Makefile.$(PLATFORM).$(COMPILER_MANUFACTURER).$(EMULATOR).make \
              m.bat                                                          \
              b.bat

VC_SRC_LIST=$(SRCS)

VS_$(PROJECT_NAME).vcxproj: Makefile                                       \
                            $(MAKESUPPORT_DIR_U)/util/vcxproj_10_HERE.sh   \
                            $(MAKESUPPORT_DIR_U)/Makefile_visual_studio.mk
	@$(ECHO) "GEN        $@"
	$(Q)VC_INC_LIST="$(call list2sep,;,$(GCCINC_LIST_U))" \
      VC_DEF_LIST="$(call list2sep,;,$(subst -D,,$(CPPFLAGS_VECTOR_MAKESUPPORT)))" \
      VC_SRC_LIST="$(VC_SRC_LIST)"                      \
      LST_PATH="$(LST_PATH)"                \
      APPL_DIR="$(dir $(APPL_DIR))"         \
      MAKE_FILES="$(VS_MAKE_FILES)"         \
      $(MAKESUPPORT_DIR_U)/util/vcxproj_10_HERE.sh > $@

VS_$(PROJECT_NAME).vcxproj.filters: Makefile                                              \
                                    $(MAKESUPPORT_DIR_U)/util/vcxproj_filters_10_HERE.sh   \
                                    $(MAKESUPPORT_DIR_U)/Makefile_visual_studio.mk
	@$(ECHO) "GEN        $@"
	$(Q)VC_INC_LIST="$(call list2sep,;,$(GCCINC_LIST_U))" \
      VC_DEF_LIST="$(call list2sep,;,$(subst -D,,$(CPPFLAGS_VECTOR_MAKESUPPORT)))" \
      VC_SRC_LIST="$(VC_SRC_LIST)"               \
      LST_PATH="$(LST_PATH)"                     \
      APPL_DIR="$(dir $(APPL_DIR))"              \
      MAKE_FILES="$(VS_MAKE_FILES)"              \
      LIBRARIES_TO_BUILD="$(LIBRARIES_TO_BUILD)" \
      GLOBAL_COMP_DIR="$(GLOBAL_COMP_DIR)"       \
      GLOBAL_DEMOCOMP_DIR="$(GLOBAL_DEMOCOMP_DIR)" \
      GENDATA_DIR="$(GENDATA_DIR)"               \
      $(MAKESUPPORT_DIR_U)/util/vcxproj_filters_10_HERE.sh > $@

visual_clean:
	@$(ECHO) "CLEAN      visual"
	$(Q)$(RM) VS_$(PROJECT_NAME).vcxproj
	$(Q)$(RM) VS_$(PROJECT_NAME).vcxproj.filters

distclean:: visual_clean

help::
	@echo " *"
	@echo " *   Visual Studio - Targets:"
	@echo " *   ------------------------"
	@echo " *    visual                           Generate vcxproj file for Visual Studio"
	@echo " *    visual_clean                     Delete vcxproj file for Visual Studio"
	@echo " *"
	@echo " ***********************************************************************************"
