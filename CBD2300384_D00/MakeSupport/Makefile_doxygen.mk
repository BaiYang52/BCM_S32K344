########################################################################################################################
# File Name  : Makefile_doxygen.mk
# Description: Sub-Makefile for doxygen handling in context of MakeSupport 4.
#
#              This file MUST NOT BE CHANGED on project integration
#
# Project    : Vector PES Build Environment
# Module     : GlobalMake 4.1
#
#-----------------------------------------------------------------------------------------------------------------------
# COPYRIGHT                                                                                                           
#-----------------------------------------------------------------------------------------------------------------------
# Copyright (c) 2019 by Vector Informatik GmbH.                                                  All rights reserved. 
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
# 04.00.00  2018-09-26  vircvo  Initial Release
# 04.00.02  2018-10-15  vircbl  Modified header, added BETA disclaimer
# 04.00.05  2018-11-20  vircvo  Added targets 'doxygen_makesupport' and 'help', moved make2doxygen.awk to folder util
# 04.01.20  2019-06-03  vircbl  Updated file header and doxygen documentation;
#                               added additional Sub-Makefiles to DOXYGEN_MAKEFILES
# 04.01.21  2019-06-03  vircbl  Updated target 'help'; moved Readme.txt and Release_notes.txt to folder 'doc'
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
#  Makefile_doxygen.mk
#  ===================
#
#  This Sub-Makefile contains target and handling to create doxygen documentation. It is included by Global.Makefile.target.\$(VERSION).mk.
#
#  Targets:
#  --------
#  - doxygen
#  - clean
#  - distclean
#
#  Expected Variables:
#  -------------------
#  - `PROJECT_NAME`         : Delivered by Makefile or MakeSupport 4
#
#  Optional Variables:
#  -------------------
#  - `MAKESUPPORT_DOC_PATH` : Output path for doxygen documentation
#  - `DOXYGEN_EXCL_FILES`   : Used to exclude files from doxygen
#  - `DOXYGEN_EXCL_DIRS`    : Used to exclude directories from doxygen
#
#  Variables supported by MakeSupport 4:
#  -------------------------------------
#  - `MAKESUPPORT_DIR_U`
#  - `MAKEFILE_DELIVERY`
#  - `ERR_PATH`
#
#  @ingroup mod_Makefile
#


############################################
#
#  doxygen section
#
#  s - command details:
#  \l  : set next character to lower case
#
DOXYGEN_NO_WIN_PATH=s!(^| )([A-Za-z]):!\1/cygdrive/\l\2!g
DOXYGEN_SED_NO_TARGET=s![^:]+:!!g
DOXYGEN_SED_NO_SOURCE=s/.+\.c//g
DOXYGEN_SED_NO_BACK_SLASH=s!\\!!g
DOXYGEN_SED_INCFILE_LIST=$(DOXYGEN_NO_WIN_PATH);$(DOXYGEN_SED_NO_TARGET);$(DOXYGEN_SED_NO_SOURCE);$(DOXYGEN_SED_NO_BACK_SLASH)

#
#  Use '/dev/null' because $(wildcard can be empty if no dependency files were generated so far
#
INCFILE_LIST=$(sort $(shell $(CAT) /dev/null $(wildcard $(OBJS_C:.o=.d)) | $(SED) -E '$(DOXYGEN_SED_INCFILE_LIST)'))

GEN_DOXYGEN_MAKEFILES=$(addsuffix .c,$(subst $(MAKESUPPORT_DIR_U),$(MAKESUPPORT_DOC_PATH),$(DOXYGEN_MAKEFILES)))

MAKESUPPORT_DOC_PATH = $(or $(wildcard $(PRJROOT_U)/internal/Doc/zBrs_MakeSupport), $(MAKESUPPORT_DIR_U)/doc)
DOXYGEN_FILE=doxygen.dox
DOXYGEN_TEMPL=$(MAKESUPPORT_DOC_PATH)/doxygen_templ.dox
doxygen: DOXYGEN_INPUT_RAW=$(SRCS) $(INCFILE_LIST)                            \
                                   $(GEN_DOXYGEN_MAKEFILES)                   \
                                   $(MAKESUPPORT_DIR_U)/doc/Readme.txt        \
                                   $(MAKESUPPORT_DIR_U)/doc/Release_notes.txt

doxygen_makesupport: DOXYGEN_INPUT_RAW= $(GEN_DOXYGEN_MAKEFILES)                   \
                                        $(MAKESUPPORT_DIR_U)/doc/Readme.txt        \
                                        $(MAKESUPPORT_DIR_U)/doc/Release_notes.txt

DOXYGEN_EXCL_DIRS  += $(shell cygpath $(COMPILER_BASE) $(COMPILER_INC) $(COMPILER_BIN) $(COMPILER_LIB))

DOXYGEN_INPUT=$(filter-out $(DOXYGEN_EXCL_FILES) $(addsuffix /%,$(DOXYGEN_EXCL_DIRS)),$(sort $(DOXYGEN_INPUT_RAW)))

THIS_MAKEFILE:=$(subst \,/,$(lastword $(MAKEFILE_LIST)))

DOXYGEN_MAKEFILES=$(MAKESUPPORT_DIR_U)/Global.Makefile.target.4.mk \
                  $(MAKEFILE_DELIVERY) \
                  $(THIS_MAKEFILE) \
                  $(MAKESUPPORT_DIR_U)/Makefile_default_values.mk \
                  $(MAKESUPPORT_DIR_U)/Makefile_functions.mk \
                  $(MAKESUPPORT_DIR_U)/Makefile_tools.mk \
                  $(MAKEFILE_SIP_STRUCTURE) \
                  $(MAKEFILE_SOURCE_WRAP) \
                  $(MAKEFILE_LIBRARY) \
                  $(MAKESUPPORT_DIR_U)/Makefile_buildinfo.mk \
                  $(MAKESUPPORT_DIR_U)/Makefile_resource.mk \
                  $(MAKESUPPORT_DIR_U)/Makefile_visual_studio.mk \
                  $(MAKESUPPORT_DIR_U)/quality/Makefile_mssv.mk \
                  $(MAKESUPPORT_DIR_U)/quality/Makefile_vca.mk \
                  $(MAKESUPPORT_DIR_U)/quality/Makefile_ECO.mk
                  

DOXYGEN_OUTPUT_PATH=$(MAKESUPPORT_DOC_PATH)/html
DOXYGEN_INDEX=$(DOXYGEN_OUTPUT_PATH)/index.html

QUIET_V_1=NO
QUIET_V_0=YES
DOXYGEN_VARS=\
             "FULL_PATH_NAMES        = NO\n"                 \
             "PROJECT_NAME           = $(PROJECT_NAME)\n"    \
             "OPTIMIZE_OUTPUT_FOR_C  = NO\n"                 \
             "TAB_SIZE               = 2\n"                  \
             "QUIET                  = $(if $(QUIET_V_$(V)),$(QUIET_V_$(V)),YES)\n"    \
             "ALIASES                = \"param_i=\\par Parameter Indexed:\\\n\" \\ \n" \
             "                          \"context=\\par Call Context:\\\n\"\n"        \
             "EXTRACT_ALL            = YES\n"               \
             "EXTRACT_PRIVATE        = YES\n"               \
             "EXTRACT_STATIC         = YES\n"               \
             "EXTRACT_LOCAL_CLASSES  = YES\n"               \
             "EXTRACT_LOCAL_METHODS  = YES\n"               \
             "EXTRACT_ANON_NSPACES   = YES\n"               \
             "DISTRIBUTE_GROUP_DOC   = YES\n"               \
             "CASE_SENSE_NAMES       = NO\n"                \
             "SORT_MEMBER_DOCS       = NO\n"                \
             "WARN_NO_PARAMDOC       = YES\n"               \
             "WARN_LOGFILE           = $(ERR_PATH)/doxygen.err\n" \
             "INPUT                  = $(foreach F,$(DOXYGEN_INPUT),  \\ \n $(F))"  \
             "\n"                                           \
             "FILE_PATTERNS          = *.h \\ \n"           \
             "                         *.c\n"               \
             "SOURCE_BROWSER         = YES\n"               \
             "INLINE_SOURCES         = YES\n"               \
             "REFERENCED_BY_RELATION = YES\n"               \
             "REFERENCES_RELATION    = YES\n"               \
             "HTML_OUTPUT            = $(DOXYGEN_OUTPUT_PATH)\n"  \
             "ENUM_VALUES_PER_LINE   = 1\n"                 \
             "GENERATE_TREEVIEW      = YES\n"               \
             "GENERATE_LATEX         = NO\n"                \
             "MACRO_EXPANSION        = YES\n"               \
             "INCLUDE_FILE_PATTERNS  = *.h\n"               \
             "SKIP_FUNCTION_MACROS   = NO\n"                \
             "HAVE_DOT               = NO\n"                \
             "UML_LOOK               = YES\n"               \
             "TEMPLATE_RELATIONS     = YES\n"               \
             "CALL_GRAPH             = YES\n"               \
             "CALLER_GRAPH           = YES\n"               \
             "MAX_DOT_GRAPH_DEPTH    = 1000\n"              \
             "DOT_TRANSPARENT        = YES\n"               \
             "SEARCHENGINE           = NO\n"

.INTERMEDIATE: $(GEN_DOXYGEN_MAKEFILES) $(DOXYGEN_FILE) $(DOXYGEN_TEMPL)

.PHONY: doxygen
doxygen doxygen_makesupport: $(DOXYGEN_INDEX)

$(DOXYGEN_INDEX): $(DOXYGEN_FILE) $(DOXYGEN_INPUT) \
                                   $(DOXYGEN_OUTPUT_PATH)/.dirstamp \
                                   $(ERR_PATH)/.dirstamp $(GEN_DOXYGEN_MAKEFILES)
	@$(ECHO) "DOXYGEN   $(DOXYGEN_OUTPUT_PATH)"
	$(Q)doxygen $<

$(DOXYGEN_FILE): $(DOXYGEN_TEMPL)
	@$(ECHO) "Generate  $@"
	$(Q)$(ECHO) -e $(DOXYGEN_VARS) | $(SED) 's/^[[:blank:]]*//g' | cat $< - | sed '-es/\\ /\\/g' > $@

$(DOXYGEN_TEMPL): $(dir $(DOXYGEN_TEMPL)).dirstamp
	@$(ECHO) "Generate  $@"
	$(Q)doxygen -s -g $@ > /dev/null

#
#  Extract documentation from Makefiles
#
$(GEN_DOXYGEN_MAKEFILES) : $(MAKESUPPORT_DOC_PATH)/%.c: $(MAKESUPPORT_DIR_U)/% $(addsuffix .dirstamp,$(sort $(dir $(GEN_DOXYGEN_MAKEFILES))))
	@$(ECHO) "Generate  $@"
	$(Q)$(AWK) -f $(MAKESUPPORT_DIR_U)/util/make2doxygen.awk $< > $@

doxygen_clean::
	@$(ECHO) "CLEAN      doxygen"
	$(Q)$(RM) $(DOXYGEN_FILE) $(DOXYGEN_FILE)1 $(DOXYGEN_FILE)0 $(DOXYGEN_TEMPL) $(GEN_DOXYGEN_MAKEFILES)
	$(Q)$(RM) $(DOXYGEN_OUTPUT_PATH)/*.html $(DOXYGEN_OUTPUT_PATH)/*.css $(DOXYGEN_OUTPUT_PATH)/*.js $(DOXYGEN_OUTPUT_PATH)/*.png
	$(Q)$(RMDIR) $(DOXYGEN_OUTPUT_PATH)

ifeq ($(VV),1)
  $(info doxygen - stat)
  $(info --------------)
  $(info INCFILE_LIST=$(words $(INCFILE_LIST)))
  $(info DOXYGEN_INPUT=$(words $(DOXYGEN_INPUT)))
  $(info Excluded=$(shell $(ECHO) $$(($(words $(DOXYGEN_INPUT_RAW)) - $(words $(DOXYGEN_INPUT))))))
  $(info DOXYGEN_INDEX=$(DOXYGEN_INDEX))
  $(info )
  $(info )
endif

help::
	@echo " *"
	@echo " *   Doxygen - Targets:"
	@echo " *   ------------------"
	@echo " *    doxygen                          Generate doxygen documentation"
	@echo " *    doxygen_makesupport              Generate MakeSupport documentation"
	@echo " *    doxygen_clean                    Delete doxygen documentation"
	@echo " *"
	@echo " ***********************************************************************************"
