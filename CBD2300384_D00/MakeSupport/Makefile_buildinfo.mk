########################################################################################################################
# File Name  : Makefile_buildinfo.mk
# Description: This Sub-Makefile contains targets to create build information.
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
#  Makefile_buildinfo.mk
#  ====================
#
#  This file handles build information. It is included by Global.Makefile.target.\$(VERSION).mk.
#
#  The build information contains
#   - information about the tools used for build 
#   - build options, like compiler and linker options
#   - sources that are part of the build
#
#  Targets:
#  --------
#  - buildinfo
#  - buildinfo_clean
#  - distclean
#
#  @ingroup mod_Makefile
#


## @file
#
#  VERSION Information from build tools in build information :
#  --------------------------------------------------------------
#
#  Version information is generated from build tools from parsed command line.
#  Note: Copied from Makefile_delivery.mk
#
#  ~~~~~~~~~~~~~~~
#  $(CC)  $(CVERSION)
#  $(AS)  $(ASVERSION)
#  $(AR)  $(ARVERSION)
#  $(LD)  $(LDRERSION)
#  ~~~~~~~~~~~~~~~
#
#  The version information is extracted from command line with a line extract.<br>
#  Makefile should provide offset and last line number in environment variables:
#
#  ~~~~~~~~~~~~~~~
#  {BUILD_TOOL}_OFF=
#  {BUILD_TOOL}_LC=
#  ~~~~~~~~~~~~~~~
#
#  Example:
#  ~~~~~~~~~~~~~~~
#  CC_OFF=1
#  CC_LC=5
#  ~~~~~~~~~~~~~~~
#
#
#
#  @ingroup mod_Makefile
#

_CC_LC  = $(strip $(if $(CC_LC),$(CC_LC),5))
_AS_LC  = $(strip $(if $(AS_LC),$(AS_LC),5))
_LD_LC  = $(strip $(if $(LD_LC),$(LD_LC),5))
_AR_LC  = $(strip $(if $(AR_LC),$(AR_LC),5))

_CC_OFF  = $(strip $(if $(CC_OFF),$(CC_OFF),1))
_AS_OFF  = $(strip $(if $(AS_OFF),$(AS_OFF),1))
_LD_OFF  = $(strip $(if $(LD_OFF),$(LD_OFF),1))
_AR_OFF  = $(strip $(if $(AR_OFF),$(AR_OFF),1))

CC_VERSION_STRING     = $(strip $(shell $(CC_ENV)  $(CC)  $(CVERSION) 2>&1   | sed '$(_CC_OFF),$(_CC_LC)!d'   | tr -d "\`'"))
AS_VERSION_STRING     = $(strip $(shell $(AS_ENV)  $(AS)  $(ASVERSION) 2>&1  | sed '$(_AS_OFF),$(_AS_LC)!d'   | tr -d "\`'"))
LD_VERSION_STRING     = $(strip $(shell $(LD_ENV)  $(LD)  $(LDVERSION) 2>&1  | sed '$(_LD_OFF),$(_LD_LC)!d'   | tr -d "\`'"))
AR_VERSION_STRING     = $(strip $(shell $(AR_ENV)  $(AR)  $(ARVERSION) 2>&1  | sed '$(_AR_OFF),$(_AR_LC)!d'   | tr -d "\`'"))


############################################
#
#  buildinfo section
#
.PHONY: buildinfo
buildinfo: buildinfo.xml


buildinfo.xml: $(wildcard Makefile.config Makefile.config.generated Makefile.project.part.defines Makefile.static) Makefile
	@$(ECHO) "Generate $@"
	$(Q)$(ECHO) "<?xml version='1.0' encoding='UTF-8'?>"                                                        > $@
	$(Q)$(ECHO) "<buildinformation>"                                                                           >> $@
	$(Q)$(ECHO) "<controller name='$(PLATFORM)' derivative='$(DERIVATIVE)' />"                                 >> $@
	$(Q)$(ECHO) " <devtoolinfo>"                                                                               >> $@
ifneq ($(CC),)
	$(Q)$(ECHO) "    <tool type='Compiler' manufacturer='$(COMPILER_MANUFACTURER)'>"                           >> $@
	$(Q)$(ECHO) "      <entry type='Version'>$(CC_VERSION_STRING)</entry>"                                     >> $@
ifeq ("$(CFLAGS_SELECTOR)","CUSTOMER")
	$(Q)$(ECHO) "      <entry type='RequestedCustomerOptions'>$(CFLAGS_CUSTOMER_OPTIONS)</entry>"              >> $@
else
	$(Q)$(ECHO) "      <entry type='VectorDefaultOptions'>$(CFLAGS_VECTOR_OPTIONS)</entry>"                    >> $@
endif  # CFLAGS_SELECTOR
	$(Q)$(ECHO) "      <entry type='VectorBuildEnvironmentOptions'>$(CPPFLAGS_VECTOR_MAKESUPPORT) $(patsubst -o%,-o,$(subst -o ,-o,$(CFLAGS_VECTOR_MAKESUPPORT)))</entry>"  >> $@
	$(Q)$(ECHO) "      <entry type='AllOptions'>$(patsubst -o%,-o,$(subst -o ,-o,$(CFLAGS)))</entry>"          >> $@
	$(Q)$(ECHOESC) $(foreach FILE,$(SRCS),                                                                     \
                $(if $(CFLAGS_COMPONENT_$(call file2mod,$(FILE))),                                             \
                  "      <entry type='AdditionalOptions $(basename $(notdir $(FILE))).$(OBJ_SUFFIX)'>          \
                     $(CFLAGS_COMPONENT_$(call file2mod,$(FILE)))</entry>\n"                                   \
                  ,))                                                                                        >> $@;
	$(Q)$(ECHOESC) $(foreach FILE,$(SRCS),                                                                     \
                $(if $(CFLAGS_$(basename $(notdir $(FILE)))),                                                  \
                  "      <entry type='AdditionalOptions $(basename $(notdir $(FILE))).$(OBJ_SUFFIX)'>          \
                     $(CFLAGS_$(basename $(notdir $(FILE))))</entry>\n",))                                   >> $@;
	$(Q)$(ECHOESC) $(foreach FILE,$(SRCS),                                                                     \
                $(if $(FILTER_COMPONENT_$(call file2mod,$(FILE))),                                             \
                  "      <entry type='FilteredOptions $(basename $(notdir $(FILE))).$(OBJ_SUFFIX)'>            \
                     $(FILTER_COMPONENT_$(call file2mod,$(FILE)))</entry>\n",))                              >> $@;
	$(Q)$(ECHOESC) $(foreach FILE,$(SRCS),                                                                     \
                $(if $(FILTER_$(basename $(notdir $(FILE)))),                                                  \
                  "      <entry type='FilteredOptions $(basename $(notdir $(FILE))).$(OBJ_SUFFIX)'>            \
                     $(FILTER_$(basename $(notdir $(FILE))))</entry>\n",))                                   >> $@;
	$(Q)$(ECHO) "    </tool>"                                                                                  >> $@
endif  # CC
ifneq ($(AS),)
	$(Q)$(ECHO) "    <tool type='Assembler' manufacturer='$(COMPILER_MANUFACTURER)'>"                          >> $@
	$(Q)$(ECHO) "      <entry type='Version'>$(AS_VERSION_STRING)</entry>"                                     >> $@
ifeq ("$(ASFLAGS_SELECTOR)","CUSTOMER")
	$(Q)$(ECHO) "      <entry type='RequestedCustomerOptions'>$(ASFLAGS_CUSTOMER_OPTIONS)</entry>"             >> $@
else
	$(Q)$(ECHO) "      <entry type='VectorDefaultOptions'>$(ASFLAGS_VECTOR_OPTIONS)</entry>"                   >> $@
endif
	$(Q)$(ECHO) "      <entry type='VectorBuildEnvironmentOptions'>$(patsubst -o%,-o,$(subst -o ,-o,$(ASFLAGS_VECTOR_MAKESUPPORT)))</entry>"  >> $@
	$(Q)$(ECHO) "      <entry type='AllOptions'>$(patsubst -o%,-o,$(subst -o ,-o,$(ASFLAGS)))</entry>"         >> $@
	$(Q)$(ECHO) "    </tool>"                                                                                  >> $@
endif  # AS
ifneq ($(LD),)
	$(Q)$(ECHO) "    <tool type='Linker' manufacturer='$(COMPILER_MANUFACTURER)'>"                             >> $@
	$(Q)$(ECHO) "      <entry type='Version'>$(LD_VERSION_STRING)</entry>"                                     >> $@
ifeq ("$(LDFLAGS_SELECTOR)","CUSTOMER")
	$(Q)$(ECHO) "      <entry type='RequestedCustomerOptions'>$(LDFLAGS_CUSTOMER_OPTIONS)</entry>"             >> $@
else
	$(Q)$(ECHO) "      <entry type='VectorDefaultOptions'>$(LDFLAGS_VECTOR_OPTIONS)</entry>"                   >> $@
endif
	$(Q)$(ECHO) "      <entry type='VectorBuildEnvironmentOptions'>$(LDFLAGS_VECTOR_MAKESUPPORT)</entry>"      >> $@
	$(Q)$(ECHO) "      <entry type='AllOptions'>$(LDFLAGS)</entry>"                                            >> $@	
	$(Q)$(ECHO) "    </tool>"                                                                                  >> $@
endif
	$(Q)$(ECHO) " </devtoolinfo>"                                                                              >> $@
	$(Q)$(ECHO) " <sourceinfo>"                                                                                >> $@
	$(Q)$(ECHO) "  <entry type='BuildRoot'>$(APPL_DIR_FROM_PRJROOT)/$(firstword $(MAKEFILE_LIST))"             >> $@
	$(Q)$(ECHO) "  </entry>"                                                                                   >> $@	
	$(Q)$(ECHO) "  <entry type='Include'>$(GCCINC_LIST_U)"                                                     >> $@
	$(Q)$(ECHO) "  </entry>"                                                                                   >> $@
	$(Q)$(ECHO) "  <entry type='Sources'>$(SRCS)"                                                              >> $@
	$(Q)$(ECHO) "  </entry>"                                                                                   >> $@	
	$(Q)$(ECHO) "  <entry type='Objects'>$(ADDITIONAL_OBJECTS_U)"                                              >> $@
	$(Q)$(ECHO) "  </entry>"                                                                                   >> $@
	$(Q)$(ECHO) "  <entry type='Libraries'>$(ADDITIONAL_LIBRARIES_U)"                                          >> $@
	$(Q)$(ECHO) "  </entry>"                                                                                   >> $@
	$(Q)$(ECHO) " </sourceinfo>"  	                                                                           >> $@
	$(Q)$(ECHO) "</buildinformation>"                                                                          >> $@


buildinfo_clean:
	@$(ECHO) "CLEAN      buildinfo"
	$(Q)$(RM) buildinfo.xml

clean:: buildinfo_clean

distclean::
	@$(ECHO) "DISTCLEAN  buildinfo"
	$(Q)$(RM) buildinfo.xml


help::
	@echo " *"
	@echo " *   Build Information - Targets:"
	@echo " *   ----------------------------"
	@echo " *    buildinfo                        Generate build information as XML file"
	@echo " *    buildinfo_clean                  Delete build information file"	
	@echo " *"
	@echo " ***********************************************************************************"
