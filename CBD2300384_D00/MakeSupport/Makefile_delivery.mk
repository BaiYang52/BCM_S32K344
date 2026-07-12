########################################################################################################################
# File Name  : Makefile_delivery.mk
# Description: This Sub-Makefile contains targets to create delivery information.
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
#  Makefile_delivery.mk
#  ====================
#
#  This file handles delivery information. It is included by Global.Makefile.target.\$(VERSION).mk.
#
#  Delivery information is created by a generated script
#  <b>makesupport.xml</b> and a xsl script called <b>script.xslt</b> from
#  MakeSupport.
#
#
#  Targets:
#  --------
#  - dd, deliverydescription
#  - dd_clean
#  - distclean
#
#


#-----------------------------------------------------------------------------
# Delivery Description target dir is $(ROOT)\_Doc for old deliveries (FBL),
# $(ROOT)\Doc for new deliveries and $(LOG_PATH) if both do not exist.
#-----------------------------------------------------------------------------
DESC_PATH=$(or $(wildcard $(ROOT_U)/Doc/DeliveryInformation),  \
               $(wildcard $(ROOT_U)/_Doc),                     \
               $(LOG_PATH),                                    \
               log)


ifeq ($(PLATFORM),VTT_ONLY)
$(warning Warning: MakeSupport is in VTTonly mode! Only generation of Delivery Description is possible.)

VTT_USECASE = 1

################################################################################
#
#  Give the possibility to extract cygwin command to MakeSupport/cmd directory
#  This value is valid for mingw and cygwin. So work with OS instead of uname
#
ifeq ($(OS),Windows_NT)
  SHELL=sh
endif

MAKESUPPORT_DIR_U:=$(patsubst %/,%,$(subst \,/,$(dir $(lastword $(MAKEFILE_LIST)))))

# Include all Sub-makefiles necessary for VTTonly use case
include $(MAKESUPPORT_DIR_U)/Makefile_functions.mk
include $(MAKESUPPORT_DIR_U)/Makefile_tools.mk

ROOT_U:=$(patsubst %/,%,$(call os_path,$(ROOT)))
DESC_PATH=$(or $(wildcard $(ROOT_U)/Doc/DeliveryInformation),  \
               $(wildcard $(ROOT_U)/_Doc),                     \
               $(LOG_PATH),                                    \
               log)

# Location of file version.info
GENTOOL_DIR_U := $(call os_path,$(GENTOOL_DIR))
VERSION_INFO_FILE ?= $(GENTOOL_DIR_U)/version.info

#
#  Default directory creation
#  Let make manage the existence of directory creation instead
#  of calling a shell instance
#
%/.dirstamp:
	$(Q)$(MKDIR) $(@D)
	$(Q)$(TOUCH) $@

# make V case insensitive
V?=$(v)

ifneq ($(V),1)
  Q=@
endif

endif

FROM_VINFO_CBD        =$(or $(shell $(XML) sel -t -v /CANbeddedVersionInfo/Properties/CBD $(VERSION_INFO_FILE) 2>/dev/null),UNKNOWN_CBD)
RELEASESHEET_FILENAME = DeliveryDescription_$(FROM_VINFO_CBD).html
RELSHEET_XSL         := $(MAKESUPPORT_DIR_U)/cmd/xslt/ReleaseSheet.xslt

############################################
#
#  xslt section
#
.PHONY: dd deliverydescription
dd deliverydescription: $(DESC_PATH)/$(RELEASESHEET_FILENAME)

# Following rule marks script.xslt as an intermediate file.
# Intermediate files in make are deleted automatically
# and the chain of rule jumps over that file.
# This avoids copying script.xslt if it is not necessary.
.INTERMEDIATE: script.xslt

$(DESC_PATH)/$(RELEASESHEET_FILENAME): $(VERSION_INFO_FILE) script.xslt $(DESC_PATH)/.dirstamp $(if $(VTT_USECASE),,makesupport.xml)
	@$(ECHO) "Generate  $@"
	$(Q)$(XSLT) -o $@ script.xslt $(VERSION_INFO_FILE)
	$(Q)$(RM) script.xslt

script.xslt: $(RELSHEET_XSL)
	$(Q)$(CP) $< $@

dd_clean:
	@$(ECHO) "CLEAN      deliverydescription"
	@$(RM) script.xslt $(DESC_PATH)/$(RELEASESHEET_FILENAME) $(if $(VTT_USECASE),,makesupport.xml)

distclean::
	@$(ECHO) "DISTCLEAN  deliverydescription"
	$(Q)$(RM) script.xslt $(if $(VTT_USECASE),,makesupport.xml)
	$(Q)$(RM) $(DESC_PATH)/.dirstamp


#-----------------------------------------------------------------------------
# xml file which contains tool version and settings (processed by xslt processor)
#-----------------------------------------------------------------------------

## @file
#
#  VERSION Information from build tools in delivery description :
#  --------------------------------------------------------------
#
#  Version information is generated from build tools from parsed command line.
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

makesupport.xml: $(wildcard Makefile.config Makefile.config.generated Makefile.project.part.defines Makefile.static) Makefile
	@$(ECHO) "Generate $@"
	$(Q)$(ECHO) "<?xml version='1.0' encoding='UTF-8'?>"                                                                                         > $@
	$(Q)$(ECHO) "<makesupport>"                                                                                                                 >> $@
	$(Q)$(ECHO) "<controller name='$(PLATFORM)' derivative='$(DERIVATIVE)' />"                                                                  >> $@
	$(Q)$(ECHO) " <devtoolinfo>"                                                                                                                >> $@
ifneq ($(CC),)
	$(Q)$(ECHO) "    <tool type='Compiler' manufacturer='$(COMPILER_MANUFACTURER)'>"                                                            >> $@
	$(Q)$(ECHO) "      <entry type='Version'>$(CC_VERSION_STRING)</entry>"                                                                      >> $@
ifeq ("$(CFLAGS_SELECTOR)","CUSTOMER")
	$(Q)$(ECHO) "      <entry type='RequestedCustomerOptions'>$(subst >,&gt;,$(subst <,&lt;,$(CFLAGS_CUSTOMER_OPTIONS)))</entry>"               >> $@
else
	$(Q)$(ECHO) "      <entry type='VectorDefaultOptions'>$(subst >,&gt;,$(subst <,&lt;,$(CFLAGS_VECTOR_OPTIONS)))</entry>"                     >> $@
endif  # CFLAGS_SELECTOR
	$(Q)$(ECHO) "      <entry type='VectorBuildEnvironmentOptions'>$(CPPFLAGS_VECTOR_MAKESUPPORT) $(patsubst -o%,-o,$(subst -o ,-o,$(CFLAGS_VECTOR_MAKESUPPORT)))</entry>"    >> $@
	$(Q)$(ECHOESC) $(foreach FILE,$(SRCS),                               \
                $(if $(CFLAGS_COMPONENT_$(call file2mod,$(FILE))),                                                                              \
                  "      <entry type='AdditionalOptions $(basename $(notdir $(FILE))).$(OBJ_SUFFIX)'>                                           \
                     $(subst >,&gt;,$(subst <,&lt;,$(CFLAGS_COMPONENT_$(call file2mod,$(FILE)))))</entry>\n"                                    \
                  ,)                                                                                                                            \
               )                                                                                                                                >> $@;
	$(Q)$(ECHOESC) $(foreach FILE,$(SRCS),                                                                                                      \
                $(if $(CFLAGS_$(basename $(notdir $(FILE)))),                                                                                   \
                  "      <entry type='AdditionalOptions $(basename $(notdir $(FILE))).$(OBJ_SUFFIX)'>                                           \
                     $(subst >,&gt;,$(subst <,&lt;,$(CFLAGS_$(basename $(notdir $(FILE))))))</entry>\n"                                         \
                  ,)                                                                                                                            \
               )                                                                                                                                >> $@;
	$(Q)$(ECHOESC) $(foreach FILE,$(SRCS),                                                                                                      \
                $(if $(FILTER_COMPONENT_$(call file2mod,$(FILE))),                                                                              \
                  "      <entry type='FilteredOptions $(basename $(notdir $(FILE))).$(OBJ_SUFFIX)'>                                             \
                     $(subst >,&gt;,$(subst <,&lt;,$(FILTER_COMPONENT_$(call file2mod,$(FILE)))))</entry>\n"                                    \
                  ,)                                                                                                                            \
               )                                                                                                                                >> $@;
	$(Q)$(ECHOESC) $(foreach FILE,$(SRCS),                                                                                                      \
                $(if $(FILTER_$(basename $(notdir $(FILE)))),                                                                                   \
                  "      <entry type='FilteredOptions $(basename $(notdir $(FILE))).$(OBJ_SUFFIX)'>                                             \
                     $(subst >,&gt;,$(subst <,&lt;,$(FILTER_$(basename $(notdir $(FILE))))))</entry>\n"                                         \
                  ,)                                                                                                                            \
               )                                                                                                                                >> $@;
ifneq ($(CFLAGS_VECTOR_HSM),)
	$(Q)$(ECHO) "      <entry type='VectorHSMOptions'>$(CFLAGS_VECTOR_HSM)</entry>"                                                             >> $@
endif # CFLAGS_VECTOR_HSM
	$(Q)$(ECHO) "    </tool>"                                                                                                                   >> $@
endif  # CC
ifneq ($(AS),)
	$(Q)$(ECHO) "    <tool type='Assembler' manufacturer='$(COMPILER_MANUFACTURER)'>"                                                           >> $@
	$(Q)$(ECHO) "      <entry type='Version'>$(AS_VERSION_STRING)</entry>"                                                                      >> $@
ifeq ("$(ASFLAGS_SELECTOR)","CUSTOMER")
	$(Q)$(ECHO) "      <entry type='RequestedCustomerOptions'>$(subst >,&gt;,$(subst <,&lt;,$(ASFLAGS_CUSTOMER_OPTIONS)))</entry>"              >> $@
else
	$(Q)$(ECHO) "      <entry type='VectorDefaultOptions'>$(subst >,&gt;,$(subst <,&lt;,$(ASFLAGS_VECTOR_OPTIONS)))</entry>"                    >> $@
endif
	$(Q)$(ECHO) "      <entry type='VectorBuildEnvironmentOptions'>$(patsubst -o%,-o,$(subst -o ,-o,$(ASFLAGS_VECTOR_MAKESUPPORT)))</entry>"    >> $@
	$(Q)$(ECHO) "    </tool>"                                                                                                                   >> $@
endif  # AS
ifneq ($(LD),)
	$(Q)$(ECHO) "    <tool type='Linker' manufacturer='$(COMPILER_MANUFACTURER)'>"                                                              >> $@
	$(Q)$(ECHO) "      <entry type='Version'>$(LD_VERSION_STRING)</entry>"                                                                      >> $@
ifeq ("$(LDFLAGS_SELECTOR)","CUSTOMER")
	$(Q)$(ECHO) "      <entry type='RequestedCustomerOptions'>$(subst >,&gt;,$(subst <,&lt;,$(LDFLAGS_CUSTOMER_OPTIONS)))</entry>"              >> $@
else
	$(Q)$(ECHO) "      <entry type='VectorDefaultOptions'>$(subst >,&gt;,$(subst <,&lt;,$(LDFLAGS_VECTOR_OPTIONS)))</entry>"                    >> $@
endif
	$(Q)$(ECHO) "      <entry type='VectorBuildEnvironmentOptions'>$(LDFLAGS_VECTOR_MAKESUPPORT)</entry>"                                       >> $@
	$(Q)$(ECHO) "    </tool>"                                                                                                                   >> $@
endif
ifneq ($(EXT_T1),)
	$(Q)$(ECHO) "    <tool type='ExternalTool1'>"                                                                                               >> $@
	$(Q)$(ECHO) "      <entry type='Version'>$(EXT_T1_VERSION_STRING)</entry>"                                                                  >> $@
	$(Q)$(ECHO) "      <entry type='Flags'>$(EXT_T1_FLAGS)</entry>"                                                                             >> $@
	$(Q)$(ECHO) "    </tool>"                                                                                                                   >> $@
endif  # EXT_T1
ifneq ($(EXT_T2),)
	$(Q)$(ECHO) "    <tool type='ExternalTool2'>"                                                                                               >> $@
	$(Q)$(ECHO) "      <entry type='Version'>$(EXT_T2_VERSION_STRING)</entry>"                                                                  >> $@
	$(Q)$(ECHO) "      <entry type='Flags'>$(EXT_T2_FLAGS)</entry>"                                                                             >> $@
	$(Q)$(ECHO) "    </tool>"                                                                                                                   >> $@
endif   # EXT_T2
	$(Q)$(ECHO) " </devtoolinfo>"                                                                                                               >> $@
	$(Q)$(ECHO) "</makesupport>"                                                                                                                >> $@

#
#  helper target to determine *_LC and *_OFF values
#
dd_dump_version:
	$(Q)$(ECHO) "====================================================="
	$(Q)$(ECHO) CC_OFF=$(CC_OFF)  CC_LC=$(CC_LC) CVERSION=$(CVERSION)
	$(Q)$(ECHO) "----------"
	$(Q)-$(CC_ENV) $(CC) $(CVERSION)
	$(Q)$(ECHO) "----------"
	$(Q)$(ECHO) "CC_VERSION_STRING=$(CC_VERSION_STRING)"
	$(Q)$(ECHO) "====================================================="
	$(Q)$(ECHO) AS_OFF=$(AS_OFF)  AS_LC=$(AS_LC) ASVERSION=$(ASVERSION)
	$(Q)$(ECHO) "----------"
	$(Q)-$(AS_ENV) $(AS) $(ASVERSION)
	$(Q)$(ECHO) "----------"
	$(Q)$(ECHO) "AS_VERSION_STRING=$(AS_VERSION_STRING)"
	$(Q)$(ECHO) "====================================================="
	$(Q)$(ECHO) AR_OFF=$(AR_OFF)  AR_LC=$(AR_LC) ARVERSION=$(ARVERSION)
	$(Q)$(ECHO) "----------"
	$(Q)-$(AR_ENV) $(AR) $(ARVERSION)
	$(Q)$(ECHO) "----------"
	$(Q)$(ECHO) "AR_VERSION_STRING=$(AR_VERSION_STRING)"
	$(Q)$(ECHO) "====================================================="
	$(Q)$(ECHO) LD_OFF=$(LD_OFF)  LD_LC=$(LD_LC) LDVERSION=$(LDVERSION)
	$(Q)$(ECHO) "----------"
	$(Q)-$(LD_ENV) $(LD) $(LDVERSION)
	$(Q)$(ECHO) "----------"
	$(Q)$(ECHO) "LD_VERSION_STRING=$(LD_VERSION_STRING)"
	$(Q)$(ECHO) "====================================================="

dd_version:
	$(Q)$(ECHO) "CC_VERSION_STRING=$(CC_VERSION_STRING)"
	$(Q)$(ECHO) "AS_VERSION_STRING=$(AS_VERSION_STRING)"
	$(Q)$(ECHO) "AR_VERSION_STRING=$(AR_VERSION_STRING)"
	$(Q)$(ECHO) "LD_VERSION_STRING=$(LD_VERSION_STRING)"


help::
	@echo " *"
	@echo " *   Delivery Description - Targets:"
	@echo " *   -------------------------------"
	@echo " *    dd,deliverydescription           Generate HTML delivery description"
	@echo " *    dd_dump_version                  Print compiler, linker, assembler, archiver version information"
	@echo " *                                     Helper to set *_LC and *_OFF variables"
	@echo " *    dd_version                       Print xxVERSION. Helper to set *_LC and *_OFF variables"
	@echo " *    dd_clean                         Delete delivery description"	
	@echo " *"
	@echo " ***********************************************************************************"
