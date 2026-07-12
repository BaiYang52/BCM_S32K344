########################################################################################################################
# File Name  : Global.Makefile.target.4.mk
# Description: This Sub-Makefile implements the common features of each project. It is included by the project local file
#              "Makefile", which contains the project specific settings (platform, compiler).
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
# Georg Lutz                    visglz        Vector Informatik GmbH
# Christian Semmelmann          virsec        Vector Informatik GmbH
#-----------------------------------------------------------------------------------------------------------------------
# REVISION HISTORY                                                                                                    
#-----------------------------------------------------------------------------------------------------------------------
# Version   Date        Author  Description                                                                           
# --------  ----------  ------  ----------------------------------------------------------------------------------------
# 04.00.00  2018-09-26  vircvo  Initial Release
# 04.00.01  2018-10-10  vircvo  Several changes/fixes (ASM Preprocessing, DEPENDFLAGS, ERR_OUTPUT, dd_dump_version)
# 04.00.02  2018-10-15  vircbl  Modified header, added BETA disclaimer
# 04.00.03  2018-10-16  vircvo  Added support of component-based SIP structure
#           2018-10-17  vircvo  Added dependency Makefile.project.part.defines for linking; Added C++ support
#           2018-10-18  vircvo  Added target preprocess and optional include of Makefile.config
#           2018-10-23  vircvo  Added support of ECO and TFW; added target p-%; Added C++ file support to target depend
#           2018-10-25  vircvo  Added MSSV; Added rebuild to NO_DEP_TARGETS
# 04.00.04  2018-11-07  vircvo  Changed semantic of 'extsubst'
#           2018-11-08  vircvo  Improved redirection syntax, make depend files from platform compiler instead of clang/cpp
#           2018-11-09  vircbl  Small fixes
# 04.00.05  2018-11-22  vircvo  Several modifications (improvement of V=1, MAKESUPPORT_POST_LINK_CMD, etc.)
#           2018-12-13  vircvo  Fixed assembler suffixes and handling of USER_LINKER_COMMAND_FILE; Added documentation
# 04.00.06  2018-12-19  vircvo  Added pseudo targets to avoid remake of Makefile; added automatic .SUFFIX
# 04.00.07  2019-01-08  vircvo  Moved folder 'lib' into folder 'cygpath_root';
#                               added feature file=<myfile-without-path>.c to target 'preprocess'
#           2019-01-09  vircvo  Added clean for section preprocess
# 04.00.08  2019-01-10  vircvo  Optimization: Avoid unneeded convertion
#           2019-01-14  vircvo  Fixed ASMCPP, cleaned-up unneeded variables
# 04.01.00  2019-01-17  vircvo  Added feature to filter options component-wise (FILTER_COMPONENT_<module>);
#                               modified features CFLAGS_MyFile and CFLAGS_COMPONENT_MyFile: MyFile is now case-sensitive
#                       vircbl  Bugfix in m.bat and in target 'clean'; Fixed target 'rebuild'
# 04.01.01  2019-01-22  vircvo  Bugfix in target 'preprocess'
# 04.01.02  2019-01-29  vircvo  Fixed EXTRA_DEPENDFLAGS if COMPILER_SUPPORT_DEPEND=1; added wildcard support for variable APP_SOURCE_LST
#           2019-01-30  vircvo  Fixed call of intrinsic function 'wildcard'
# 04.01.03  2019-01-31  vircvo  Target 'preprocess': Added option -C to print comments
#                       vircbl  Bugfix in m.bat (environment variable TMP was overwritten erroneously)
# 04.01.04  2019-02-05  vircvo  Extended default asm suffixes (added 850 and arm)
# 04.01.05  2019-02-12  vircbl  Incremented SUB_MODULE_VERSION
# 04.01.06  2019-02-13  vircbl  Added feature "Generation of Visual Studio project"
# 04.01.07  2019-03-19  vircbl  Added MSSV support as default
# 04.01.08  2019-03-20  vircbl  m.bat: Added make option -Otarget by default
# 04.01.09  2019-03-26  vircbl  - No changes
# 04.01.10  2019-04-02  vircbl  Added support of library usage for FBL and CBD
# 04.01.11  2019-04-05  vircbl  - No changes
# 04.01.12  2019-04-12  vircbl  Added 'CPPFLAGS_CORE' to assembler calls
# 04.01.13  2019-04-12  vircbl  Fix in assembler calls: Just add include paths
# 04.01.14  2019-04-16  vircbl  Integrated NAnt support compatible with MakeSupport 4
# 04.01.15  2019-03-24  vircbl  - No changes
# 04.01.16  2019-04-25  vircbl  Bugfixes for Non-AUTOSAR use case
# 04.01.20  2019-06-03  vircbl  Updated file header and doxygen documentation
# 04.01.21  2019-06-03  vircbl  Added overview of Makefile structure (see MakeSupport doc folder); updated target 'help'
# 04.01.22  2019-06-05  vircbl  Just include Makefile_library.mk if 'LIBRARY_USED' is 1;
#                               fixed issue in case 'APP_SOURCE_LST' is empty
# 04.01.23  2019-06-25  vircbl  Fixed condition for include of Makefile_library.mk
# 04.01.25  2019-09-24  vircbl  Since C++ is optional for classic, setting CXXFLAGS_SELECTOR is optional, too
# 04.01.27  2020-01-07  vircbl  Add: Support of target 'resource'
# 04.01.28  2020-03-10  vircbl  Fixes: Target 'distclean'; calls of FblPostBuild.bat and UpdateVFlashPack.bat;
#                                      AR_RULE: ASM files are now considered for library creation
#                               Add: Targets 'cleanlibs', 'doxygen_clean' and 'res_clean'
# 04.01.29  2020-03-30  vircbl  Add: #DI-682 Flag LINK_BSW_LIBRARIES to link BSW components as libraries instead of as object files
#                               Add: Target 'copylibs' (available if USE_AUTOSAR_MAKE is 1)
# 04.01.30  2020-03-31  vircbl  Fix: #ECO #DI-926 Completed ECO argument list, deleted duplicated depend options,
#                                    added missing linker arguments
#                               Fix: #ECO Removed preprocess flag to preserve comments from all compiler config files
# 04.01.31  2020-04-14  vircbl  Fix: #DI-1000 Fixed path issues for component-based SIP structure
#                               Mod: Renamed 'LINK_BSW_LIBRARIES' to 'LINK_LIBRARIES'
#                       virrro  Mod: Updated download location of NAntSupport
# 04.01.32  2020-04-16  vircbl  Fix: Fixed issue in check of deprecated variable 'LINK_BSW_LIBRARIES'
# 04.01.33  2020-05-04  vircbl  Add: #DI-999 Added target 'copyartifacts'
#                               Fix: Extended list 'NO_DEP_TARGETS'; Corrected path for NAntSupport download;
#                               Fix: Added support of MSSV usage for component-based SIP structure
# 04.01.34  2020-06-29  vircbl  Fix: #DI-619 Fixed issue with additional/filtered options in DD and output path of DD
# 04.01.35  2020-07-23  vircbl  Fix: #DI-1171 Added support of MSSV for component-based SIP structure;
#                               Mod: Added prefix to warning and error messages
# 04.01.36  2020-08-25  vircbl  Fix: #DI-1380 MSSV: Added names of license files
# 04.01.37  2020-08-31  vircbl  Fix: MSSV: Deleted automatic dependency of MSSV report to mssv_plugin_check;
#                                          PluginCheckFileGenerator now considers all plugins
#                                    ECO: Enclosed header files with quotes for argument '-HeaderFiles'
#                                    Resource: Added support of spaces in path for resource file
# 04.01.38  2020-09-24  vircbl  Fix: MSSV: Added __GNUC__ to MSSV defines
#                                    Consider options added/filtered by CLAGS|FILTER_COMPONENT_* in delivery description
# 04.01.39  2020-10-02  vircbl  Add: #DI-1091 Added integration of VCA
# 04.01.40  2020-10-08  vircbl  Add: #DI-1091 Added VCA-specific files to target 'distclean'
#                               Fix: Fixed issue regarding wrong variable usage for MSSV and VCA 
# 04.01.41  2020-12-16  vircbl  Add: #DI-692 Provide generation of build information
#                               Fix: Fixed issues for targets 'distclean' and 'copylibs' 
# 04.01.42  2021-02-18  vircbl  Fix: Fixed issue for 'dd' in VTTonly use case
#                               Fix: Added target 'cleanlibs' to target 'clean'
#                               Mod: Display MakeSupport help when 'm.bat -h' is called
# 04.01.43  2021-06-23  vircbl  Mod: #DI-1682: Updated NAnt version
#                               Del: #DI-1718: Removed unnecessary target 'resource'
# 04.01.44  2021-07-23  vircbl  Add: #DI-1578: Support folder DemoComponent
# 04.01.45  2021-09-09  visglz  Add: DI-1565: Add vHSM-specific compiler options to DeliveryDescription
# 04.01.46  2021-10-01  visglz  Add: DI-2250: Support long argument lists for linkage
#                       virsec  Add: DI-2117, DI-1379: Update ECO configuration
# 04.01.47  2021-10-22  visglz  Fix: TAR-2542: Re-Release of 04.01.46 with changed ALM usage type, no source changes
# 04.01.48  2022-02-21  visglz  Fix: DI-2481: (Fix) MakeSupport4 FILTER_COMPONENT setting does not work
# 04.01.49  2022-04-11  visglz  Fix: DI-2643: (Fix) MakeSupport4 Creation of response file fails with long list of objects
# 04.01.50  2022-04-27  visglz  Fix: DI-2671: MakeSupport4: Fix UsageType for ECO files
# 04.01.51  2022-06-28  vircbl  Fix: TAR-8835: (Fix) MakeSupport4: No Errorcode if MAKESUPPORT_DIR is not set
# 04.01.52  2022-08-15  vircbl  Fix: TAR-10134: (Fix) MakeSupport4: Adapt condition for SIP structure evaluation
# 04.01.53  2022-09-15  vircbl  Fix: TAR-11450: (Fix) MakeSupport4: Just add ASM files to variable that exist
#                                               (Fix) MakeSupport4: Make sure that source list does not contain duplicates
# 04.01.54  2022-11-15  vircbl  Fix: CIB-2404: (Fix) Fix for TAR-14124: Fix archiver command
# 04.01.55  2022-11-16  vircbl  Fix: CIB-2404: (Fix) Fix for TAR-14124: Remove target library from linkage command
# 04.01.56  2022-12-08  vircbl  Fix: CIB-2440: Fix for TAR-4871: Prevent the dependencies from being updated twice during a rebuild
# 04.01.57  2023-01-31  vircbl  Fix: CIB-2421: Fix for TAR-14996: Allow '<' and '>' in build options for DeliveryDescription
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


## @defgroup mod_Makefile MakeSupport Version 4
#  @brief MakeSupport Version 4 Documentation
#
#  This module collects documentation of MakeSupport.
#  Please read the documentation in Makefile collected in this module.
#

## @file
#
#  @a Global.Makefile.target.4.mk
#  ==============================
#
#  This Sub-Makefile contains the main targets needed for build process. It is included by main Makefile.
#
#
#  Targets:
#  ========
#  - all
#  - elf, link
#  - compile
#  - assemble
#  - depend
#  - preprocess
#  - preprocess file=<MyFile>.<ext>
#  - <MyFile>.o
#  - help
#  - clean
#  - distclean
#  - rebuild
#  - copyartifacts
#  - info
#  - show_objs
#  - check
#  - brsvinfo
#  - printmodules
#  - printmodulefiles
#  - For detailed information, please have a look at output of <b>help</b> target
#
#
#  Expected variables:
#  ===================
#
#  AUTOSAR variables:
#  ------------------
#  - `MODULE_LIST_PROJECT`   : List of AUTOSAR modules.
#                            Normally defined in Makefile.project.part.defines
#  - `<MODULE>_USED`       : 0|1 to disable|enable module from `MODULE_LIST_PROJECT`
#  - `USE_AUTOSAR_MAKE`      : 1 to use AUTOSAR Makefiles
#  - `VERSION`               : Compared with `MODULE_VERSION`
#  - `SUB_VERSION `          : Compared with `SUB_MODULE_VERSION` and
#                            `SUB_MODULE_VERSION_COMPATIBILITY_LIST`.
#
#  Flags optionally used by Makefile:
#  ----------------------------------------
#  - `LINK_LIBRARIES=1`              : Default=0. Link all sources as object files
#                                      If set to 1, link components as libraries instead of as object files in order
#                                      to reduce the length of the linker's argument list
#  - `REUSE_CPP_DEPEND=1`            : Optimization for reuse preprocessed dependency process
#  - `EXTRA_DEPENDFLAGS`             : Flags added to depend pre-proceccor. These flags are typically
#                                    defines added by embedded compiler.
#  - `DEPENDFLAGS`                   : Compiler specific dependency flags. clang/cpp uses -MMD or -MM.
#                                      Default [empty] uses CPP depend flags.
#                                      To use another dependency preprocessor set
#                                      `DEPENDFLAGS` and `DEPEND` environment variable.
#  - `COMPILER_SUPPORTS_DEPEND=1`    : Default=0. cpp/clang is used.
#                                      If set to 1, the embedded compiler must be
#                                      instructed with its own dependency flags.
#
#                                      Example for Tasking/TriCore: 
#                                      `CPPFLAGS_VECTOR_MAKESUPPORT+=--dep-file=$(basename $@).d) --make-target=$@ -Em`
#    `AR_COMMAND_WITHOUT_SPACE=1`    : Default=0 or unset. Rule for archive command without space between flags and target name is used. 
#                                      If set to 1, the rule for archive command without space between flags and target name is used.
#                                      Cross-compiler specific option; so it will be set in Makefile.static.
#  - `CPPFLAGS_VECTOR_MAKESUPPORT`   : Used to add build defines (corresponds to variable `DEP_DEFINES` in MakeSupport 3)
#  - `MAKESUPPORT_POST_COMPILE_CMD`  : Command after compiler. Can be used to move temp files or
#                                      fix dependency double quotes.
#
#                                      Calling convention:
#                                      `$(call MAKESUPPORT_POST_COMPILE_CMD,$@,$<)`
#
#                                      \b Examples:
#                                    
#                                      -#  Dummy Verbose:
#
#                                          `MAKESUPPORT_POST_COMPILE_CMD=echo "object_file=$(1) source_file=$(2)"`
#
#                                      -#  Move temp files from current directory to object directory:
#
#                                          `MAKESUPPORT_POST_COMPILE_CMD=mv $(notdir $(basename $(2))).i $(dir $(1)); mv $(notdir $(basename $(2))).s $(dir $(1)) || true`
#
#                                      -#  Fix " in dependency file:
#
#                                          ```MAKESUPPORT_POST_COMPILE_CMD=tr -d  '\"' < $(basename $@).dq > $(basename $@).d; rm $(basename $@).dq```
#
#  - `MAKESUPPORT_POST_LINK_CMD`     : Here you can add an alternative post processing after link like hex convertion.
#                                      Call syntax: 	`$(call MAKESUPPORT_POST_LINK_CMD,$@,$<)`
#                                      For more details see variable `MAKESUPPORT_POST_COMPILE_CMD`
#
#  - `USER_LINKER_COMMAND_FILE`      : Linker script. Default: `$``(PROJECT_NAME).``$``(BINARY_SUFFIX)`
#
#  - `PREPROCESS_LINKER_COMMAND_FILE`: Preprocess `$(USER_LINKER_COMMAND_FILE)` with `CPPFLAGS_VECTOR_MAKESUPPORT` (defined in main Makefile)
#
#  - `LD_DEPENDENCIES`               : Additional dependency. Might be used for Linker script files.
#
#  - `TARGET`                        : Binary executable. Default: `$``(PROJECT_NAME).``$``(BINARY_SUFFIX)`
#
#  - `ELFCONVERTER_FLAGS`            : Flags using `$``(ELFCONVERTER)`
#
#  ID:
#  ---
#  - `PLATFORM`
#  - `COMPILER_MANUFACTURER`
#  - `EMULATOR`
#
#  Suffix:
#  -------
#  - `BINARY_SUFFIX`
#  - `OBJ_SUFFIX`
#  - `ASM_SUFFIX` extra suffix other than '.s', '.asm', '.850' or  '.arm'. Could be a list.
#
#  Path:
#  -----
#  - `LOG_PATH`
#  - `OBJ_PATH`
#  - `ERR_PATH`
#  - `LST_PATH`
#  - `LIB_PATH`
#  - `MAKESUPPORT_DIR` (must be a relative path!)
#
#
#  Control variables:
#  ------------------
#  - `COMPILER_SUPPORTS_DEPEND=1`
#  - `DO_DEPEND=0`
#  - `REUSE_CPP_DEPEND=1`
#
#  Guide port Makefile:
#  ====================
#
#  Windows '\' in cygwin:
#  ----------------------
#  '\' doesn't exists in cygwin path specification. Current Makefile calls 
#  toolchain which can contain '\' in their paths. cygwin and cygwin bash
#  handle '\' as an escape sign which will do arrive the toolchain.
#  Solutions:
#  a) use '/' instead of '\' which windows understands as well
#  b) use bash not expandable quotes <'>. Compare to <"> <'> will not be
#     modified inside. No escape and no variable expansion.
#
#  Why 'cygpath' is used:
#  ----------------------
#  -# cygpath is used in build, because CC, CXX and sometimes DEPEND use
#     windows toolchain which knows only windows path spec.
#  -# windows path spec contains ':' which is incompatible to cygwin Makefile
#
#  Restriction:
#    Blanks in a path are NOT allowed, because cygwin make handles this like separate
#    files.
#
#
#  Changes from MakeSupport 3:
#  ---------------------------
#
#  - MakeSupport 4 is a completely new created Makefile based on cygwin instead
#    of MinGW. '\' is not used anymore. It uses a gnu-make version which is
#    younger than MakeSupport 3 which leads to some useful new features.
#  - MakeSupport 4 creates dependencies automatically;
#    target <b>depend</b> is still available.
#  - Overwritng rules is normally not needed anymore, because `FILTER_<File-without-extension>` and
#    `CFLAGS_<File-without-extension>` are implemented by default. These features can now be used
#    component-wise: `FILTER_COMPONENT_<Module>` and `CFLAGS_COMPONENT_<Module>`
#
#    Notes:
#    - <file-without-extension> and <Module> are case-sensitive!
#    - Currently, just static sources are considered!
#
#    If overwriting is needed place it after include of Global.Makefile.target.\$(VERSION).mk
#    or use static pattern rule. @sa https://www.gnu.org/software/make/manual/html_node/Static-Usage.html#Static-Usage
#  - A couple of targets are changed from ':' to '::'. This helps to get more
#    modular Makefiles. Every Sub-Makefile can have its own clean-target
#    without overwriting:
#    - clean
#    - distclean
#    - help
#
#  @ingroup mod_Makefile
#
################################################################################

.SUFFIX:

################################################################################
#
#  Version section
#
#  This section prevents from using incompatible MakeSupport versions
#
#  Makefile.static in delivery has to set the following variables:
#  VERSION         - checked with MODULE_VERSION
#  SUB_VERSION     - checked with SUB_MODULE_VERSION and
#                    SUB_MODULE_VERSION_COMPATIBILITY_LIST
#
#
#
MODULE_VERSION = 4
SUB_MODULE_VERSION = 1
SUB_MODULE_VERSION_COMPATIBILITY_LIST = 1

ifneq ($(VERSION),$(MODULE_VERSION))
  $(error Error: Wrong Version: '$(VERSION)'. Expected : '$(MODULE_VERSION)'.)
endif

ifneq ($(SUB_VERSION),$(SUB_MODULE_VERSION))
  ifeq ($(findstring $(SUB_VERSION),$(SUB_MODULE_VERSION_COMPATIBILITY_LIST)),)
    $(error Error: Wrong SUB_VERSION=$(SUB_VERSION). Expected SUB_VERSION='$(SUB_MODULE_VERSION)'.)
  else
    $(warning Warning: Wrong SUB_VERSION=$(SUB_VERSION). But is backward-compatible to '$(SUB_MODULE_VERSION)'.)
  endif
endif

################################################################################
#
#  Check section
#
ifneq ($(filter-out visual,$(MAKECMDGOALS)),)
  ifeq ($(wildcard $(COMPILER_BASE)/.),)
    $(error Error: Directory COMPILER_BASE='$(COMPILER_BASE)' doesn't exist.)
  else ifeq ($(VV),1)
    $(info )
    $(info COMPILER_BASE='$(COMPILER_BASE)')
  endif
endif

ifeq ($(MAKESUPPORT_DIR),)
  $(error Error: MAKESUPPORT_DIR not set.)
endif

MISSING_CONF_VARS = $(filter __MISSING_VAR__%,                            \
                      $(foreach MUSTVAR,$(REQUIRED_MK_CONF_VARS),         \
                        $(subst ,__MISSING_VAR__$(MUSTVAR),$($(MUSTVAR))) \
                      )                                                   \
                     )

ifneq ($(MISSING_CONF_VARS),)
  $(error Error: Variable(s) $(subst __MISSING_VAR__,,$(MISSING_CONF_VARS)) are missing or not configured correctly in Makefile.config.)
endif



.SUFFIXES:

ifeq ($(VV),1)
  $(info )
  $(info Control variables)
  $(info -----------------)
  $(info '  USER_LINKER_COMMAND_FILE=$(USER_LINKER_COMMAND_FILE)')
  $(info '  TARGET=$(TARGET)')
  $(info '  COMPILER_SUPPORTS_DEPEND=$(COMPILER_SUPPORTS_DEPEND)')
  $(info '  DO_DEPEND=$(DO_DEPEND)')
  $(info '  REUSE_CPP_DEPEND=$(REUSE_CPP_DEPEND)')
endif

################################################################################
#
#  Give the possibility to extract cygwin command to MakeSupport/cygwin_root/cmd directory
#  This value is valid for mingw and cygwin. So work with OS instead of uname
#
ifeq ($(OS),Windows_NT)
  SHELL=sh
endif

.PHONY: all
all: elf

#
#  Make default target independent on Makefile include order
#
.DEFAULT_GOAL := default

################################################################################
#
#  Common variables not dependent on include
#

APPL_DIR:=$(dir $(firstword $(MAKEFILE_LIST)))
uname:=$(shell uname)
MAKESUPPORT_DIR_U:=$(patsubst %/,%,$(subst \,/,$(dir $(lastword $(MAKEFILE_LIST)))))

empty :=
space := $(empty) $(empty)

ifeq ($(VV),1)
  $(info )
  $(info MAKESUPPORT_DIR_U=$(MAKESUPPORT_DIR_U))
endif

include $(MAKESUPPORT_DIR_U)/Makefile_functions.mk


################################################################################
#
#  clean and all MUST be serialized even when called with -j
#  .NOTPARALLEL would serialize complete build
#
rebuild: clean
	$(Q)$(MAKE) --no-print-directory default

################################################################################
#
#  include section 1
#
include $(MAKESUPPORT_DIR_U)/Makefile_tools.mk

ifeq ($(findstring Makefile.config,$(notdir $(MAKEFILE_LIST))),)
  ifeq ($(VV),1)
    $(info )
    $(info include Makefile.config)
  endif
  -include $(APPL_DIR)/Makefile.config
endif
ifeq ($(findstring Makefile.project.part.defines,$(notdir $(MAKEFILE_LIST))),)
  ifeq ($(VV),1)
    $(info )
    $(info include Makefile.project.part.defines)
  endif
  include $(APPL_DIR)/Makefile.project.part.defines
endif

################################################################################
#
#  function get_enabled list-of-var
#
#  VAR1=0
#  VAR2=1
#  VAR3=0
#  VAR4=1
#  VAR5=1
#
#  LIST=VAR1 VAR2 VAR3 VAR4 VAR5
#
#  $(call get_enabled,$(LIST))
#
#  -> VAR2 VAR4 VAR5
#
get_enabled=$(filter-out 0,$(foreach E,$(1),$(subst 1,$(E),$($(E)_USED))))

################################################################################
#
#  Common variables
#
#  Use real path with relative to options -m to reduce path length
#  and make canonical which is needed by compile rule
#
#  ROOT_U            : root of directory where the used components reside
#  PRJROOT_U         : root of project (common folder of components and application)
#  MAKESUPPORT_DIR_U : place where MakeSupport resides
#

ROOT_U:=$(patsubst %/,%,$(call os_path,$(ROOT)))
PRJROOT_U=$(call os_path,$(PRJROOT))
ifeq ($(PRJROOT_U),)
  PRJROOT_U:=$(call realpath_rel,.,$(ROOT_U)/..)
endif

ROOT_FROM_PRJROOT:=$(call realpath_rel,$(PRJROOT_U),$(ROOT_U))
APPL_DIR_FROM_PRJROOT:=$(call realpath_rel,$(PRJROOT_U),$(APPL_DIR))

GENTOOL_DIR          ?=$(ROOT_U)/Generators/Components

MAKEFILE_DELIVERY=$(MAKESUPPORT_DIR_U)/Makefile_delivery.mk

ifeq ($(VV),1)
$(info )
$(info Common variables)
$(info ----------------)
$(info '  ROOT=$(ROOT)')
$(info '  ROOT_U=$(ROOT_U)')
$(info '  PRJROOT_U=$(PRJROOT_U)')
$(info ----------------------------------------)
endif

#
#  Variables required by AUTOSAR makefiles
#
OBJ_OUTPUT_PATH?=$(OBJ_PATH)
OBJ_FILE_SUFFIX?=$(OBJ_SUFFIX)
LIB_OUTPUT_PATH?=$(LIB_PATH)
OBJ_FILE_SUFFIX?=$(OBJ_SUFFIX)

ifeq ($(VV),1)
$(info )
$(info AUTOSAR variables)
$(info ----------------)
$(info '  OBJ_OUTPUT_PATH=$(OBJ_OUTPUT_PATH)')
$(info '  OBJ_FILE_SUFFIX=$(OBJ_FILE_SUFFIX)')
$(info '  LIB_OUTPUT_PATH=$(LIB_OUTPUT_PATH)')
$(info '  OBJ_FILE_SUFFIX=$(OBJ_FILE_SUFFIX)')
$(info ----------------------------------------)
endif

# Include makefile containing necessary variables for SIP structure settings
ifeq ($(wildcard $(ROOT_U)/BSW/*/*.h),)
  ifeq ($(VV),1)
    $(info )
    $(info include Makefile_BSW_vars_ComponentBased.mk ROOT=$(ROOT_U)/BSWMD)
  endif
  include $(MAKESUPPORT_DIR_U)/Makefile_BSW_vars_ComponentBased.mk
  MAKEFILE_SIP_STRUCTURE = $(MAKESUPPORT_DIR_U)/Makefile_BSW_vars_ComponentBased.mk 
else
  ifeq ($(VV),1)
    $(info )
    $(info include Makefile_BSW_vars_Legacy.mk)
  endif
  include $(MAKESUPPORT_DIR_U)/Makefile_BSW_vars_Legacy.mk
  MAKEFILE_SIP_STRUCTURE = $(MAKESUPPORT_DIR_U)/Makefile_BSW_vars_Legacy.mk
endif

################################################################################
#
#  function mod2sources(module)
#
#  Get files for a module
#
mod2sources=$(call os_path,$($(1)_SOURCES_U))

################################################################################
#
#  include section 2
#
ifneq ($(GLOBAL_MAKEFILE_GUARD),)
  $(error Error: '$(basename $(lastword $(MAKEFILE_LIST)))' already included.)
endif
GLOBAL_MAKEFILE_GUARD:=1

MODULE_USED_LST := $(call get_enabled,$(MODULE_LIST_PROJECT))

ifeq ($(USE_AUTOSAR_MAKE),1)
  ifeq ($(VV),1)
    $(info )
    $(info include Makefile_AutoSarWrap)
  endif
  include $(MAKESUPPORT_DIR_U)/Makefile_AutoSarWrap.mk
  MAKEFILE_SOURCE_WRAP = $(MAKESUPPORT_DIR_U)/Makefile_AutoSarWrap.mk
else
  ifeq ($(VV),1)
    $(info )
    $(info include Makefile_NoAutoSarWrap)
  endif
  include $(MAKESUPPORT_DIR_U)/Makefile_NoAutoSarWrap.mk
  MAKEFILE_SOURCE_WRAP = $(MAKESUPPORT_DIR_U)/Makefile_NoAutoSarWrap.mk
  
endif

ifeq ($(findstring Makefile.static,$(notdir $(MAKEFILE_LIST))),)
  ifeq ($(VV),1)
    $(info )
    $(info include Makefile.static)
  endif
  -include $(APPL_DIR)/Makefile.static
endif

ifeq ($(findstring Makefile.derivative.settings,$(notdir $(MAKEFILE_LIST))),)
  ifeq ($(VV),1)
    $(info )
    $(info include Makefile.derivative.settings)
  endif
  -include Makefile.derivative.settings
endif

ifeq ($(findstring Makefile.$(PLATFORM).$(COMPILER_MANUFACTURER).$(EMULATOR).make,$(notdir $(MAKEFILE_LIST))),)
  ifneq ($(wildcard $(APPL_DIR)/Makefile.$(PLATFORM).$(COMPILER_MANUFACTURER).$(EMULATOR).make),)
    ifeq ($(VV),1)
      $(info )
      $(info include $(APPL_DIR)/Makefile.$(PLATFORM).$(COMPILER_MANUFACTURER).$(EMULATOR).make)
    endif
    include $(APPL_DIR)/Makefile.$(PLATFORM).$(COMPILER_MANUFACTURER).$(EMULATOR).make
  endif
endif

ifeq ($(VV),1)
    $(info )
  $(info '  LD_DEPENDENCIES=$(LD_DEPENDENCIES)')
endif

include $(MAKESUPPORT_DIR_U)/Makefile_default_values.mk

################################################################################
#
#  No Makefile remaking
#
%.mk: ;

%.mak: ;

Makefile.derivative.settings : ; @true
Makefile.project.part.defines: ; @true
Makefile.Platform.config     : ; @true
Makefile.config              : ; @true
Makefile.config.generated    : ; @true
Makefile.static              : ; @true
Makefile                     : ; @true

#
#  Default values
#
DO_DEPEND?=1

ifeq ($(COMPILER_INC),)
  $(error Error: 'COMPILER_INC' has to be specified via environment variable.)
endif
ifeq ($(COMPILER_BIN),)
  $(error Error: 'COMPILER_BIN' has to be specified via environment variable.)
endif

ifeq ($(CC),)
  $(error Error: 'CC' has to be specified via environment variable.)
endif
ifeq ($(AR),)
  $(error Error: 'AR' has to be specified via environment variable.)
endif
ifeq ($(DEPEND),)
  $(error Error: 'DEPEND' has to be specified via environment variable.)
endif

################################################################################
#
#  If COMPILER_SUPPORTS_DEPEND is not set, try to find out automatically by Makefile
#
#  If 'gcc' is part of CC, it is mentioned that it is a cross compiler
#  based on gcc with platform specific backend.
#
ifeq ($(COMPILER_SUPPORTS_DEPEND),)
  COMPILER_SUPPORTS_DEPEND=$(if $(filter %gcc,$(CC))$(filter gcc%,$(CC))$(filter %gcc,$(CC)),1,0)
  ifeq ($(V)+$(COMPILER_SUPPORTS_DEPEND),1+1)
    $(info Set COMPILER_SUPPORTS_DEPEND=1 automatically)
  endif
  ifeq ($(COMPILER_SUPPORTS_DEPEND),1)
    CPPFLAGS_INTERMEDIATE+=-MMD -MP -MT '$@' -MF $(basename $@).d
  endif
endif

#############################################################################
#
#  DEPENDFLAGS
#
#  In case of usage of different build step for dependencies, don't
#  waste CPPFLAGS with additional dependency flags.
#
#  This section sets the default DEPENDFLAGS for cpp or clang dependency
#  preprocessor.
#
#  -nostdinc  : to avoid include from host-system-includes like /usr/include
#
ifeq ($(DEPENDFLAGS),)
  DEPENDFLAGS=-MM -MP -MT '$@' -nostdinc $(CPPFLAGS_CORE) -o $(basename $@).d
endif

ifeq ($(REUSE_DEPENDFLAGS)+$(REUSE_CPP_DEPEND),+1)
  REUSE_DEPENDFLAGS=-MMD -MP -MT '$@' -nostdinc $(CPPFLAGS_CORE) -MF $(basename $@).d -E
endif

ifeq ($(ASMPP_DEPENDFLAGS),)
  ASMPP_DEPENDFLAGS=-MMD -MP -MT '$@' -nostdinc $(CPPFLAGS_CORE) -MF $(basename $@).d
endif

ifeq ($(DEPEND),clang)
  DEPENDFLAGS+=-Wno-nonportable-include-path
endif

ifneq ($(COMPILER_SUPPORTS_DEPEND),1)
  DEPENDFLAGS+=$(EXTRA_DEPENDFLAGS)
endif

#############################################################################
#
#  Compiler info
#
ifeq ($(VV),1)
  $(info CC=$(CC))
  $(info LD=$(LD))
  $(info AR=$(AR))
  $(info AS=$(AS))
endif

################################################################################
#
#  Convinient UNIX variables from AUTOSAR generation
#

APP_SOURCE_LST_U              = $(if $(APP_SOURCE_LST), $(shell cygpath $(call os_path,$(APP_SOURCE_LST))))
APP_SOURCE_LST_REL_U          = $(sort $(wildcard $(filter-out /%,$(APP_SOURCE_LST_U))))
APP_SOURCE_LST_ABS_U          = $(filter     /%,$(APP_SOURCE_LST_U))
ASM_SOURCES_U                := $(sort $(wildcard $(call os_path,$(ASM_SOURCES))))
GENERATED_SOURCE_FILES_U     := $(sort $(wildcard $(call os_path,$(GENERATED_SOURCE_FILES))))
ASM_GENERATED_SOURCE_FILES_U := $(sort $(wildcard $(sort $(call os_path,$(ASM_GENERATED_SOURCE_FILES)))))
TARGET                       ?= $(PROJECT_NAME).$(BINARY_SUFFIX)
CC_INCLUDE_PATH_U            := $(call os_path,$(CC_INCLUDE_PATH))
GENTOOL_DIR_U                := $(call os_path,$(GENTOOL_DIR))
ADDITIONAL_OBJECTS_U         := $(call os_path,$(ADDITIONAL_OBJECTS))
ADDITIONAL_LIBRARIES_U       := $(call os_path,$(ADDITIONAL_LIBRARIES))
ELFCONVERTER_BIN_U           := $(call os_path,$(ELFCONVERTER_BIN))
DATA_SOURCE_LST_U            := $(call os_path,$(DATA_SOURCE_LST))

################################################################################
#
#  Define section
#

ifeq ($(MKVERBOSE),1)
  V=1
endif

# make V case insensitive
V?=$(v)

ifneq ($(V),1)
  Q=@
endif

APP_SOURCE_ALL_LST_REL_U=$(sort $(APP_SOURCE_LST_REL_U) $(GENERATED_SOURCE_FILES_U) $(MODULE_DATA_LST_U) $(DATA_SOURCE_LST_U))

SRCS+=$(addprefix $(ROOT_U)/,$(MODULE_SOURCE_LST_U))
SRCS+=$(APP_SOURCE_ALL_LST_REL_U)
SRCS+=$(APP_SOURCE_LST_ABS_U)

APPL_DIR_FROM_ROOT:=$(call realpath_rel,../../..,$(APPL_DIR))

#
#  CC - part
#
OBJS_C_NOT_REAL+=    $(addprefix $(OBJ_PATH)/$(ROOT_FROM_PRJROOT)/,$(call extsubst,cpp c,$(OBJ_SUFFIX),$(MODULE_SOURCE_LST_U)))
OBJS_C_NOT_REAL+=$(addprefix $(OBJ_PATH)/$(APPL_DIR_FROM_PRJROOT)/,$(call extsubst,cpp c,$(OBJ_SUFFIX),$(APP_SOURCE_ALL_LST_REL_U)))
OBJS_C_NOT_REAL+=                       $(addprefix $(OBJ_PATH)/,$(call extsubst,cpp c,$(OBJ_SUFFIX),$(APP_SOURCE_LST_ABS_U)))

# reduce '..' from path
OBJS_C=$(call realpath_rel,.,$(sort $(OBJS_C_NOT_REAL)))

#
#  AS - part
#
ASM_SUFFIXES+=$(sort $(ASM_SUFFIX) s asm 850 arm)

SRCS+=$(sort $(MODULE_ASM_LST_U) $(ASM_GENERATED_SOURCE_FILES_U) $(ASM_SOURCES_U))

OBJS_ASM_NOT_REAL+=    $(addprefix $(OBJ_PATH)/$(ROOT_FROM_PRJROOT)/,$(call extsubst,$(ASM_SUFFIXES),$(OBJ_SUFFIX),$(MODULE_ASM_LST_U)))
OBJS_ASM_NOT_REAL+=$(addprefix $(OBJ_PATH)/$(APPL_DIR_FROM_PRJROOT)/,$(call extsubst,$(ASM_SUFFIXES),$(OBJ_SUFFIX),$(ASM_GENERATED_SOURCE_FILES_U) $(ASM_SOURCES_U)))

# reduce '..' from path
OBJS_ASM=$(call realpath_rel,.,$(sort $(OBJS_ASM_NOT_REAL)))

OBJS=$(OBJS_ASM) $(OBJS_C)

# Location of file version.info (necessary for Delivery Description and Resource Information)
VERSION_INFO_FILE ?= $(GENTOOL_DIR_U)/version.info

###########################################
#
#  Redirection of compile and link
#
#  Options for ERR_OUTPUT
#  - PIPE
#  - FLAGS
#  - CONSOLE
#  - TEST (just for internal use!)
#
ifeq ($(ERR_OUTPUT),PIPE)
  ifeq ($(V),1)
    #
    #  PIPESTATUS is used to retrieve return code oc compiler instead of tee
    #
    REDIRECT_OUTPUT    = |& tee $(basename $@).$(ERR_SUFFIX)                 ; test $${PIPESTATUS[0]} -eq 0
    CC_REDIRECT_OUTPUT = |& tee $(call obj2outfile,$(ERR_PATH),$(ERR_SUFFIX)); test $${PIPESTATUS[0]} -eq 0
    LD_REDIRECT_OUTPUT = |& tee $(@:.$(BINARY_SUFFIX)=.$(ERR_SUFFIX))        ; test $${PIPESTATUS[0]} -eq 0
    AR_REDIRECT_OUTPUT = |& tee $(LIB_PATH)/lib$(1).$(ERR_SUFFIX)            ; test $${PIPESTATUS[0]} -eq 0
  else
    REDIRECT_OUTPUT    = &> $(basename $@).$(ERR_SUFFIX)
    CC_REDIRECT_OUTPUT = &> $(call obj2outfile,$(ERR_PATH),$(ERR_SUFFIX))
    LD_REDIRECT_OUTPUT = &> $(@:.$(BINARY_SUFFIX)=.$(ERR_SUFFIX))
    AR_REDIRECT_OUTPUT = &> $(LIB_PATH)/lib$(1).$(ERR_SUFFIX)
  endif
else ifeq ($(ERR_OUTPUT),FLAGS)
  CC_REDIRECT_OUTPUT = 
  LD_REDIRECT_OUTPUT = 
  AR_REDIRECT_OUTPUT = 
else ifeq ($(ERR_OUTPUT),CONSOLE)
  CC_REDIRECT_OUTPUT = 
  LD_REDIRECT_OUTPUT = 
  AR_REDIRECT_OUTPUT = 
else ifeq ($(ERR_OUTPUT),TEST)
  CC_REDIRECT_OUTPUT = > $(TEST_FILE)
  LD_REDIRECT_OUTPUT = 
  AR_REDIRECT_OUTPUT = 
else
  $(error Error: Variable ERR_OUTPUT=$(ERR_OUTPUT) in Makefile.static is not set correctly. [PIPE,FLAGS,CONSOLE] are allowed.)
endif

############################################
#
#  AR section
#
ifeq ($(USE_AUTOSAR_MAKE),1)
  include $(MAKESUPPORT_DIR_U)/Makefile_AutoSar_library.mk
  MAKEFILE_LIBRARY = $(MAKESUPPORT_DIR_U)/Makefile_AutoSar_library.mk
endif

ifeq ($(LIBRARY_USED),1)
  ifneq ($(LIBRARY_SOURCES),)
    # This fills TARGET_LIBRARY for linking
    include $(MAKESUPPORT_DIR_U)/Makefile_library.mk
    MAKEFILE_LIBRARY = $(MAKESUPPORT_DIR_U)/Makefile_AutoSar_library.mk
  endif
endif

################################################################################
#
#  Linker section
#

.PHONY: elf link
link elf: $(TARGET)

###########################################
#
#  Preprocessed linker script section
#
#  - Use USER_LINKER_COMMAND_FILE if set or $(PROJECT_NAME).$(LNK_SUFFIX)
#  - Add in between file <basename>_Preprocessed.$(LNK_SUFFIX)
#
ifeq ($(USER_LINKER_COMMAND_FILE),)
  USER_LINKER_COMMAND_FILE = $(PROJECT_NAME).$(LNK_SUFFIX)
endif

LINKER_COMMAND_FILE ?= $(strip $(if $(findstring 1,$(PREPROCESS_LINKER_COMMAND_FILE))               \
                                ,$(basename $(USER_LINKER_COMMAND_FILE))_Pre.$(LNK_SUFFIX) \
                                ,$(USER_LINKER_COMMAND_FILE)))

ifeq ($(PREPROCESS_LINKER_COMMAND_FILE),1)
# Target for preprocessing linker file
# SED deletes empty lines in DOS and Windows lines.
# SED is prefered for grep -v because of -i [in place] option.
# This provides a better way to control cpp errors.
# - [[:cntrl:]] is for windows CR.
$(LINKER_COMMAND_FILE): $(USER_LINKER_COMMAND_FILE)
	@$(ECHO) "Run GNU GCC to pre-process linker script ..."
	$(Q)$(CPP) $(CPPFLAGS_VECTOR_MAKESUPPORT) -P -I$(GENDATA_DIR) $< -o $@
	$(Q)$(SED) -i '/^[[:blank:]]*[[:cntrl:]]*$$/d' $@
endif

LDFLAGS = $(LDFLAGS_VECTOR_MAKESUPPORT)
ifeq ($(LDFLAGS_SELECTOR),VECTOR)
  ifeq ($(V),1)
    $(info LD  using Vector-CFLAGS)
  endif
  LDFLAGS += $(LDFLAGS_VECTOR_OPTIONS)
else ifeq ($(LDFLAGS_SELECTOR),CUSTOMER)
  ifeq ($(V),1)
    $(info LD  using Customer-CFLAGS)
  endif
  LDFLAGS += $(LDFLAGS_CUSTOMER_OPTIONS)
else
  $(error Error: "LDFLAGS_SELECTOR" within Makefile not set. Set to "VECTOR" or "CUSTOMER".)
endif

ELFCONVERTER_FLAGS?=--patch_crc --out $(basename $@).hex

#
#  Linker rule
#
#  .SECONDEXPANSION:
#  OBJS might be expanded in other makefiles which occur afterwards.
#

# Support for response files
#
# This section defines the following variables:
#
# TARGET_RESPONSE_FILE: The path to the response file, empty if response files are not used.
# 
# LINKER_OBJS_ARG: Linker command line for object files, either directly the object file names
# or something like "@target.rsp" pointing to a file
#
ifneq ($(USE_RESPONSE_FILE_FOR_LINKAGE),1)

TARGET_RESPONSE_FILE :=
ifneq ($(LINK_LIBRARIES),1)
LINKER_OBJS_ARG = $(call uniq,$(OBJS))
else
LINKER_OBJS_ARG = $(call uniq,$(filter-out $(OBJS_LIB),$(OBJS))) $(BSW_LIBRARIES)
endif # LINK_LIBRARIES

else # USE_RESPONSE_FILE_FOR_LINKAGE

ifeq ($(LDFLAGS_RESPONSE_FILE),)
    $(error Error: USE_RESPONSE_FILE_FOR_LINKAGE set, but LDFLAGS_RESPONSE_FILE unset.)
endif # LDFLAGS_RESPONSE_FILE
TARGET_RESPONSE_FILE := $(TARGET).rsp
LINKER_OBJS_ARG = $(LDFLAGS_RESPONSE_FILE)$(TARGET_RESPONSE_FILE)
.SECONDEXPANSION:
$(TARGET_RESPONSE_FILE): $$(OBJS)
	@$(ECHO) "Generate response file $@"
ifneq ($(LINK_LIBRARIES),1)
	$(file >$@,$(call uniq,$(OBJS)))
else # LINK_LIBRARIES
	$(file >$@,$(call uniq,$(filter-out $(OBJS_LIB),$(OBJS))))
	$(file >>$@,$(BSW_LIBRARIES))
endif # LINK_LIBRARIES

endif # USE_RESPONSE_FILE_FOR_LINKAGE

.SECONDEXPANSION:
ifneq ($(LINK_LIBRARIES),1)
$(TARGET): $$(OBJS) $(LD_DEPENDENCIES) $(LINKER_COMMAND_FILE) \
           $(ADDITIONAL_OBJECTS_U) $(ADDITIONAL_LIBRARIES_U) \
           $(TARGET_LIBRARY) $(TARGET_RESPONSE_FILE) \
           $(wildcard Makefile.project.part.defines) \
           $(dir $(TARGET))/.dirstamp
	@$(ECHO) "LD         $@"
	$(Q)$(CC_ENV) $(LD) $(LDFLAGS) $(LINKER_OBJS_ARG)       \
	                               $(ADDITIONAL_OBJECTS_U)   \
	                               $(ADDITIONAL_LIBRARIES_U) \
	                               $(SYSLIBS)                \
	                               $(LD_REDIRECT_OUTPUT)
else
$(TARGET): library $$(filter-out $(OBJS_LIB),$(OBJS)) $(LD_DEPENDENCIES) $(LINKER_COMMAND_FILE) \
					 $(BSW_LIBRARIES) \
					 $(ADDITIONAL_OBJECTS_U) $(ADDITIONAL_LIBRARIES_U) \
           $(TARGET_LIBRARY) $(TARGET_RESPONSE_FILE) \
           $(wildcard Makefile.project.part.defines) \
           $(dir $(TARGET))/.dirstamp
	@$(ECHO) "LD         $@"
	$(Q)$(CC_ENV) $(LD) $(LDFLAGS) $(LINKER_OBJS_ARG) \
	                               $(ADDITIONAL_OBJECTS_U)   \
	                               $(ADDITIONAL_LIBRARIES_U) \
	                               $(SYSLIBS)                \
	                               $(LD_REDIRECT_OUTPUT)
endif
	$(Q)$(call MAKESUPPORT_POST_LINK_CMD,$@,$<)
ifneq ($(USE_ELFCONVERTER),)
	@$(ECHO) "CRC        $@"
	$(Q)$(ELFCONVERTER_BIN_U) $@ $(ELFCONVERTER_FLAGS)
endif
ifneq ($(wildcard ../A2L_AdressUpdate.bat),)
	@$(ECHO) "A2L        $@"
	$(Q)../A2L_AdressUpdate.bat --nopause
endif
ifneq ($(wildcard FblPostBuild.bat),)
	$(ECHO) "FBL        $@" 
	$(Q)./FblPostBuild.bat $(PROJECT_NAME).hex
endif
ifneq ($(wildcard UpdateVFlashPack.bat),)
	$(ECHO) "VFL        $@"
	$(Q)./UpdateVFlashPack.bat
endif

#
#  Dummy .dirstamp for current directory
#
.dirstamp:
	@touch $@

############################################
#
#  CC section
#

#
#  FBLCAN_INC
#
FBLCAN_INC   = $(if $(findstring 1,$(FBLCAN_USED) $(CANFBL_USED)),$(ROOT)\drvflash $(ROOT)\$(GLOBAL_COMP_DIR)\Flash)

#
#  GENDATA_DIR
#
GENDATA_DIR?=$(if $(wildcard data/.*),data,GenData)

#
#  CPPFLAGS
#

GCCINC_LIST  = \
               $(MODULE_INCLUDE_SUBDIRS_U)                \
               $(COMMON_INC)                              \
               $(FBLCAN_INC)                              \
               $(GENDATA_DIR)                             \
               Include                                    \
               $(COMPILER_INC)                            \
               $(ADDITIONAL_INCLUDES)

GCCINC_LIST_U=$(call uniq,$(call os_path,$(GCCINC_LIST)))

check:: check_include

check_include:
	@echo "Check if include paths do not exist:"
	@for dir in $(GCCINC_LIST_U); do \
    if [ ! -d $$dir ]; then \
      echo "$$dir does not exist."; \
    fi; \
  done

#
#  CPPFLAGS_CORE is used as a base of DEPENDFLAGS
#
CPPFLAGS_INC_LIST+=$(addprefix -I,$(GCCINC_LIST_U))

CPPFLAGS_CORE+=$(CPPFLAGS_INC_LIST)
CPPFLAGS_CORE+=$(CPPFLAGS_VECTOR_MAKESUPPORT)

#
#  CFLAGS
#
CFLAGS_INTERMEDIATE += $(call os_path,$(CFLAGS_VECTOR_MAKESUPPORT))
ifeq ($(CFLAGS_SELECTOR), VECTOR)
  ifeq ($(V),1)
    $(info CC  using Vector-CFLAGS)
  endif
  CFLAGS_INTERMEDIATE += $(call os_path,$(CFLAGS_VECTOR_OPTIONS))
else ifeq ($(CFLAGS_SELECTOR), CUSTOMER)
  ifeq ($(V),1)
    $(info CC  using Customer-CFLAGS)
  endif
  CFLAGS_INTERMEDIATE += $(call os_path,$(CFLAGS_CUSTOMER_OPTIONS))
else
  $(error Error: "CFLAGS_SELECTOR" in Makefile not set to "VECTOR" or "CUSTOMER".)
endif
CFLAGS_INTERMEDIATE += $(call os_path,$(CFLAGS_VECTOR_HSM))


#
#  CXXFLAGS
#
CXXFLAGS_INTERMEDIATE = $(call os_path,$(CXXFLAGS_VECTOR_MAKESUPPORT))
ifeq ($(CXXFLAGS_SELECTOR), VECTOR)
  ifeq ($(V),1)
    $(info CXX using Vector-CFLAGS)
  endif
  CXXFLAGS_INTERMEDIATE += $(call os_path,$(CXXFLAGS_VECTOR_OPTIONS))
else ifeq ($(CXXFLAGS_SELECTOR), CUSTOMER)
  ifeq ($(V),1)
    $(info CXX using Customer-CFLAGS)
  endif
  CXXFLAGS_INTERMEDIATE += $(call os_path,$(CXXFLAGS_CUSTOMER_OPTIONS))
# commented out since this is an optional feature for classic projects (MSR, CBD, and FBL)!
#else
#  $(warning Warning: "CXXFLAGS_SELECTOR" in Makefile not set to "VECTOR" or "CUSTOMER".)
endif

#
#  Add CFLAGS_COMPONENT_<module>-wise feature
#
CPPFLAGS_INTERMEDIATE+=$(CPPFLAGS_CORE)                          \
                       $(CPPFLAGS_$(basename $(*F)))             \
                       $(CPPFLAGS_COMPONENT_$(call file2mod,$<))

CPPFLAGS+=$(filter-out $(FILTER_$(basename $(*F))) $(FILTER_COMPONENT_$(call file2mod,$<)),$(CPPFLAGS_INTERMEDIATE))

CFLAGS_INTERMEDIATE+=$(CFLAGS_$(basename $(*F)))                            \
                     $(CFLAGS_COMPONENT_$(call file2mod,$<))
                    
CFLAGS+=$(filter-out $(FILTER_$(basename $(*F))) $(FILTER_COMPONENT_$(call file2mod,$<)),$(CFLAGS_INTERMEDIATE))

CXXFLAGS_INTERMEDIATE+=$(CFLAGS_$(basename $(*F)))                            \
                       $(CXXFLAGS_COMPONENT_$(call file2mod,$<))
                    
CXXFLAGS+=$(filter-out $(FILTER_$(basename $(*F))) $(FILTER_COMPONENT_$(call file2mod,$<)),$(CXXFLAGS_INTERMEDIATE))


compile: $(OBJS_C)

#
#  Create short targets to compile single files without knowledge of complete path
#
define SIMPLE_OBJ_RULE
$(1):$(2)
endef

$(foreach OBJ,$(OBJS),$(eval $(call SIMPLE_OBJ_RULE,$(notdir $(OBJ)),$(OBJ))))

OBJ_DIRSTAMPS:=$(addsuffix, .dirstamp,$(sort $(dir $(OBJS))))
.SECONDARY:: $(OBJ_DIRSTAMPS)
.PRECIOUS::  $(OBJ_DIRSTAMPS)

#
#  Relative 'C' compile rule
#
.SECONDEXPANSION:
$(OBJ_PATH)/%.$(OBJ_SUFFIX): $(PRJROOT_U)/%.c  $$(dir $(OBJ_PATH)/%).dirstamp                      \
                                               $$(if $(ERR_PATH),$$(dir $(ERR_PATH)/%).dirstamp)   \
                                               $$(if $(LST_PATH),$$(dir $(LST_PATH)/%).dirstamp)   \
                                               $(DIR_TO_CREATE_DIR_STAMPS_U) \
                                               $$(BRSVINFO_H)
ifeq ($(COMPILER_SUPPORTS_DEPEND)$(REUSE_CPP_DEPEND),01)
	@$(ECHO) "Depend     $(notdir $(@:.$(OBJ_SUFFIX)=.d))"
	$(Q)$(DEPEND) $(REUSE_DEPENDFLAGS) $< -o $(basename $@).c
	@$(ECHO) "CC         ${@F}"
	$(Q)$(CC_ENV) $(CC) $(CPPFLAGS) $(CFLAGS) $(basename $@).c $(CC_REDIRECT_OUTPUT)
else ifeq ($(COMPILER_SUPPORTS_DEPEND)$(DO_DEPEND),01)
	@$(ECHO) "Depend     $(notdir $(@:.$(OBJ_SUFFIX)=.d))"
	$(Q)$(DEPEND) $(DEPENDFLAGS) $<
	@$(ECHO) "CC         ${@F}"
	$(Q)$(CC_ENV) $(CC) $(CPPFLAGS) $(CFLAGS) $< $(CC_REDIRECT_OUTPUT)
else
	@$(ECHO) "CC         ${@F}"
	$(Q)$(CC_ENV) $(CC) $(CPPFLAGS) $(CFLAGS) $< $(CC_REDIRECT_OUTPUT)
endif
	$(Q)$(call MAKESUPPORT_POST_COMPILE_CMD,$@,$<)

#
#  abs 'C' compile rule
#
.SECONDEXPANSION:
$(OBJ_PATH)/%.$(OBJ_SUFFIX): /%.c  $$(dir $(OBJ_PATH)/%).dirstamp                    \
                                   $$(if $(ERR_PATH),$$(dir $(ERR_PATH)/%).dirstamp) \
                                   $$(if $(LST_PATH),$$(dir $(LST_PATH)/%).dirstamp) \
                                   $(DIR_TO_CREATE_DIR_STAMPS_U)
ifeq ($(COMPILER_SUPPORTS_DEPEND)$(REUSE_CPP_DEPEND),01)
	@$(ECHO) "Depend     $(notdir $(@:.$(OBJ_SUFFIX)=.d))"
	$(Q)$(DEPEND) $(REUSE_DEPENDFLAGS) $(shell cygpath -m $<) -o $(basename $@).c
	@$(ECHO) "CC         ${@F}"
	$(Q)$(CC_ENV) $(CC) $(CPPFLAGS) $(CFLAGS) $(basename $@).c $(CC_REDIRECT_OUTPUT)
else ifeq ($(COMPILER_SUPPORTS_DEPEND)$(DO_DEPEND),01)
	@$(ECHO) "Depend     $(notdir $(@:.$(OBJ_SUFFIX)=.d))"
	$(Q)$(DEPEND) $(DEPENDFLAGS) $(shell cygpath -m $<)
	@$(ECHO) "CC         ${@F}"
	$(Q)$(CC_ENV) $(CC) $(CPPFLAGS) $(CFLAGS) $(shell cygpath -m $<) $(CC_REDIRECT_OUTPUT)
else
	@$(ECHO) "CC         ${@F}"
	$(Q)$(CC_ENV) $(CC) $(CPPFLAGS) $(CFLAGS) $(shell cygpath -m $<) $(CC_REDIRECT_OUTPUT)
endif
	$(Q)$(call MAKESUPPORT_POST_COMPILE_CMD,$@,$<)

#
#  Relative 'C++' compile rule
#
.SECONDEXPANSION:
$(OBJ_PATH)/%.$(OBJ_SUFFIX): $(PRJROOT_U)/%.cpp  $$(dir $(OBJ_PATH)/%).dirstamp                    \
                                               $$(if $(ERR_PATH),$$(dir $(ERR_PATH)/%).dirstamp) \
                                               $$(if $(LST_PATH),$$(dir $(LST_PATH)/%).dirstamp) \
                                               $(DIR_TO_CREATE_DIR_STAMPS_U)                     \
                                               $$(BRSVINFO_H)
ifeq ($(COMPILER_SUPPORTS_DEPEND)$(REUSE_CPP_DEPEND),01)
	@$(ECHO) "Depend     $(notdir $(@:.$(OBJ_SUFFIX)=.d))"
	$(Q)$(DEPEND) $(REUSE_DEPENDFLAGS) $< -o $(basename $@).c
	@$(ECHO) "CXX        ${@F}"
	$(Q)$(CC_ENV) $(CXX) $(CPPFLAGS) $(CXXFLAGS) $(basename $@).c $(CC_REDIRECT_OUTPUT)
else ifeq ($(COMPILER_SUPPORTS_DEPEND)$(DO_DEPEND),01)
	@$(ECHO) "Depend     $(notdir $(@:.$(OBJ_SUFFIX)=.d))"
	$(Q)$(DEPEND) $(DEPENDFLAGS) $<
	@$(ECHO) "CXX        ${@F}"
	$(Q)$(CC_ENV) $(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(CC_REDIRECT_OUTPUT)
else
	@$(ECHO) "CXX        ${@F}"
	$(Q)$(CC_ENV) $(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(CC_REDIRECT_OUTPUT)
endif
	$(Q)$(call MAKESUPPORT_POST_COMPILE_CMD,$@,$<)

#
#  abs 'C++' compile rule
#
.SECONDEXPANSION:
$(OBJ_PATH)/%.$(OBJ_SUFFIX): /%.cpp  $$(dir $(OBJ_PATH)/%).dirstamp                    \
                                     $$(if $(ERR_PATH),$$(dir $(ERR_PATH)/%).dirstamp) \
                                     $$(if $(LST_PATH),$$(dir $(LST_PATH)/%).dirstamp) \
                                     $(DIR_TO_CREATE_DIR_STAMPS_U)
ifeq ($(COMPILER_SUPPORTS_DEPEND)$(REUSE_CPP_DEPEND),01)
	@$(ECHO) "Depend     $(notdir $(@:.$(OBJ_SUFFIX)=.d))"
	$(Q)$(DEPEND) $(REUSE_DEPENDFLAGS) $(shell cygpath -m $<) -o $(basename $@).c
	@$(ECHO) "CXX        ${@F}"
	$(Q)$(CC_ENV) $(CXX) $(CPPFLAGS) $(CXXFLAGS) $(basename $@).c $(CC_REDIRECT_OUTPUT)
else ifeq ($(COMPILER_SUPPORTS_DEPEND)$(DO_DEPEND),01)
	@$(ECHO) "Depend     $(notdir $(@:.$(OBJ_SUFFIX)=.d))"
	$(Q)$(DEPEND) $(DEPENDFLAGS) $(shell cygpath -m $<)
	@$(ECHO) "CXX        ${@F}"
	$(Q)$(CC_ENV) $(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(CC_REDIRECT_OUTPUT)
else
	@$(ECHO) "CXX        ${@F}"
	$(Q)$(CC_ENV) $(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(CC_REDIRECT_OUTPUT)
endif
	$(Q)$(call MAKESUPPORT_POST_COMPILE_CMD,$@,$<)

#
#  Default directory creation
#  Let make manage the existence of directory creation instead
#  of calling a shell instance
#
%/.dirstamp:
	$(Q)$(MKDIR) $(@D)
	$(Q)$(TOUCH) $@

############################################
#
#  depend section
#
.PHONY: depend
depend: DEPEND_TARGET_CALLED = 1
depend: $(OBJS_C:.$(OBJ_SUFFIX)=.d)

#
#  Dependency files with relative file names
#
.SECONDEXPANSION:
$(OBJ_PATH)/%.d: $(PRJROOT_U)/%.c $$(BRSVINFO_H) $$(dir $(OBJ_PATH)/%).dirstamp
	$(if $(DEPEND_TARGET_CALLED),@$(ECHO) "Depend    $(notdir $(@))",)
	$(if $(DEPEND_TARGET_CALLED),$(Q)$(DEPEND) $(DEPENDFLAGS) $(EXTRA_DEPENDFLAGS) $<,)

#
#  Dependency files with relative file names
#
.SECONDEXPANSION:
$(OBJ_PATH)/%.d: $(PRJROOT_U)/%.cpp $$(BRSVINFO_H) $$(dir $(OBJ_PATH)/%).dirstamp
	$(if $(DEPEND_TARGET_CALLED),@$(ECHO) "Depend    $(notdir $(@))",)
	$(if $(DEPEND_TARGET_CALLED),$(Q)$(DEPEND) $(DEPENDFLAGS) $(EXTRA_DEPENDFLAGS) $<,)

#
#  Dependency files with absolute file names
#
.SECONDEXPANSION:
$(OBJ_PATH)/%.d: /%.c $$(dir $(OBJ_PATH)/%).dirstamp
	$(if $(DEPEND_TARGET_CALLED),@$(ECHO) "Depend    $(notdir $(@))",)
	$(if $(DEPEND_TARGET_CALLED),$(Q)$(DEPEND) $(DEPENDFLAGS) $<,)

#
#  Dependency files with absolute file names
#
.SECONDEXPANSION:
$(OBJ_PATH)/%.d: /%.cpp $$(dir $(OBJ_PATH)/%).dirstamp
	$(if $(DEPEND_TARGET_CALLED),@$(ECHO) "Depend    $(notdir $(@))",)
	$(if $(DEPEND_TARGET_CALLED),$(Q)$(DEPEND) $(DEPENDFLAGS) $<,)

#
#  Optimization:
#  Wildcard reduced the implicit try to create missing dependency file
#
#  Don't stop clean project in case of error in dependency file.
#  Make it possible to clean wrong dependency file.
#
NO_DEP_TARGETS=check \
               clean \
               depend \
               distclean \
               doxygen \
               info \
               help \
               print-% p-% \
               rebuild \
               show_objs \
               copylibs printlibs printlibfiles cleanlibs \
               printmodules printmodulefiles \
               deliverydescription dd \
               buildinfo \
               mssv mssv_plugin_check mssv_convert_report mssv_execute_verifier mssv_clean \
               vca vca_execute_verifier vca_plugin_check vca_clean \
               visual10 visual12 visual \
               nant_setup NAnt-files/Gen/%.xml build.xml nant_rm nant_rm_all \
               multi cs
  
#
#  Ignore dependency on special targets which do not need dependency
#
#  This supports erasing of corrupted dependency files.
#
ifneq ($(filter-out $(NO_DEP_TARGETS),$(or $(MAKECMDGOALS),default)),)
  -include $(wildcard $(OBJS:.$(OBJ_SUFFIX)=.d))
endif

show_objs:
	@$(ECHO) $(foreach OBJ,$(OBJS),$(notdir $(OBJ))) | tr " " "\n"

############################################
#
#  preprocess section
#
PREPROCESSFLAGS     += -P -C
PREPROCESS_NOT_REAL += $(addprefix $(LOG_PATH)/$(ROOT_FROM_PRJROOT)/,$(call wildcard_dir,$(ROOT),$(MODULE_SOURCE_LST_U)))
PREPROCESS_NOT_REAL += $(addprefix $(LOG_PATH)/$(APPL_DIR_FROM_PRJROOT)/,$(call wildcard_dir,.,$(APP_SOURCE_ALL_LST_REL_U)))

PREPROCESS_FILES=$(call realpath_rel,.,$(sort $(PREPROCESS_NOT_REAL)))

ifeq ($(file),)
preprocess: $(PREPROCESS_FILES)
else
# TODO: Check if file exists -> ifneq($(wildcard $(filter %/$(file),$(PREPROCESS_FILES))),)
preprocess: $(filter %/$(file),$(PREPROCESS_FILES))
endif

PRE_DIRSTAMPS:=$(addsuffix .dirstamp,$(sort $(dir $(PREPROCESS_FILES))))

.SECONDARY:: $(PRE_DIRSTAMPS)
.PRECIOUS::  $(PRE_DIRSTAMPS)

.SECONDEXPANSION:
$(LOG_PATH)/%.c: $(PRJROOT_U)/%.c $$(dir $(LOG_PATH)/%).dirstamp
	@$(ECHO) "CPP        $(@F)"
	$(Q)$(CPP) $(CPPFLAGS_CORE) $(EXTRA_DEPENDFLAGS) $(PREPROCESSFLAGS) $< | awk -f $(MAKESUPPORT_DIR_U)/util/doscpp2dos.awk > $@

.SECONDEXPANSION:
$(LOG_PATH)/%.c: $(PRJROOT_U)/%.cpp $$(dir $(LOG_PATH)/%).dirstamp
	@$(ECHO) "CPP        $(@F)"
	$(Q)$(CPP) $(CPPFLAGS_CORE) $(EXTRA_DEPENDFLAGS) $(PREPROCESSFLAGS) $< | awk -f $(MAKESUPPORT_DIR_U)/util/doscpp2dos.awk > $@

clean::
	@$(ECHO) "CLEAN      preprocess"
	$(Q)$(RM) $(PREPROCESS_FILES)

############################################
#
#  AS section
#
ASFLAGS += $(ASFLAGS_VECTOR_MAKESUPPORT)
ifeq ($(ASFLAGS_SELECTOR), VECTOR)
  ifeq ($(V),1)
    $(info AS  using Vector-CFLAGS)
  endif
  ASFLAGS += $(ASFLAGS_VECTOR_OPTIONS)
else ifeq ($(ASFLAGS_SELECTOR), CUSTOMER)
  ifeq ($(V),1)
    $(info AS  using Customer-CFLAGS)
  endif
  ASFLAGS += $(ASFLAGS_CUSTOMER_OPTIONS)
else
  $(error Error: "ASFLAGS_SELECTOR" within Makefile not set. Set to "VECTOR" or "CUSTOMER".)
endif

assemble: $(OBJS_ASM)

ifneq ($(ASFLAGS_VECTOR_PREPROCESS),)
  AS_VECTOR_PREPROCESS=1
endif

#
#  Rules
#
.SECONDEXPANSION:
$(OBJ_PATH)/%.$(OBJ_SUFFIX): $(PRJROOT_U)/%.asm \
                             $$(dir $(OBJ_PATH)/%).dirstamp \
                             $$(if $(ERR_PATH),$$(dir $(ERR_PATH)/%).dirstamp) \
                             $$(if $(LST_PATH),$$(dir $(LST_PATH)/%).dirstamp)
ifeq ($(AS_VECTOR_PREPROCESS),1)
	@$(ECHO) "ASMPP      $(notdir $@)"
	$(Q)$(AS_ENV) $(ASMPP) $(ASMPP_DEPENDFLAGS) $(ASFLAGS_VECTOR_PREPROCESS) $(CPPFLAGS_VECTOR_MAKESUPPORT) $< -o $(basename $@)$(suffix $<) $(CC_REDIRECT_OUTPUT)
	@$(ECHO) "AS        ${@F}"
	$(Q)$(AS_ENV) $(AS) $(CPPFLAGS_INC_LIST) $(ASFLAGS) $(basename $@)$(suffix $<) $(CC_REDIRECT_OUTPUT)
else
	@$(ECHO) "AS         ${@F}"
	$(Q)$(AS_ENV) $(AS) $(CPPFLAGS_INC_LIST) $(ASFLAGS) $< $(CC_REDIRECT_OUTPUT)
endif

.SECONDEXPANSION:
$(OBJ_PATH)/%.$(OBJ_SUFFIX): $(PRJROOT_U)/%.850 \
                             $$(dir $(OBJ_PATH)/%).dirstamp \
                             $$(if $(ERR_PATH),$$(dir $(ERR_PATH)/%).dirstamp) \
                             $$(if $(LST_PATH),$$(dir $(LST_PATH)/%).dirstamp)
ifeq ($(AS_VECTOR_PREPROCESS),1)
	@$(ECHO) "ASMPP      $(notdir $@)"
	$(Q)$(AS_ENV) $(ASMPP) $(ASMPP_DEPENDFLAGS) $(ASFLAGS_VECTOR_PREPROCESS) $(CPPFLAGS_VECTOR_MAKESUPPORT) $< -o $(basename $@)$(suffix $<) $(CC_REDIRECT_OUTPUT)
	@$(ECHO) "AS        ${@F}"
	$(Q)$(AS_ENV) $(AS) $(CPPFLAGS_INC_LIST) $(ASFLAGS) $(basename $@)$(suffix $<) $(CC_REDIRECT_OUTPUT)
else
	@$(ECHO) "AS         ${@F}"
	$(Q)$(AS) $(CPPFLAGS_INC_LIST) $(ASFLAGS) $< $(CC_REDIRECT_OUTPUT)
endif

.SECONDEXPANSION:
$(OBJ_PATH)/%.$(OBJ_SUFFIX): $(PRJROOT_U)/%.s \
                             $$(dir $(OBJ_PATH)/%).dirstamp \
                             $$(if $(ERR_PATH),$$(dir $(ERR_PATH)/%).dirstamp) \
                             $$(if $(LST_PATH),$$(dir $(LST_PATH)/%).dirstamp)
ifeq ($(AS_VECTOR_PREPROCESS),1)
	@$(ECHO) "ASMPP      $(notdir $@)"
	$(Q)$(AS_ENV) $(ASMPP) $(ASMPP_DEPENDFLAGS) $(ASFLAGS_VECTOR_PREPROCESS) $(CPPFLAGS_VECTOR_MAKESUPPORT) $< -o $(basename $@)$(suffix $<) $(CC_REDIRECT_OUTPUT)
	@$(ECHO) "AS        ${@F}"
	$(Q)$(AS_ENV) $(AS) $(CPPFLAGS_INC_LIST) $(ASFLAGS) $(basename $@)$(suffix $<) $(CC_REDIRECT_OUTPUT)
else
	@$(ECHO) "AS         ${@F}"
	$(Q)$(AS) $(CPPFLAGS_INC_LIST) $(ASFLAGS) $< $(CC_REDIRECT_OUTPUT)
endif

.SECONDEXPANSION:
$(OBJ_PATH)/%.$(OBJ_SUFFIX): $(PRJROOT_U)/%.arm \
                             $$(dir $(OBJ_PATH)/%).dirstamp \
                             $$(if $(ERR_PATH),$$(dir $(ERR_PATH)/%).dirstamp) \
                             $$(if $(LST_PATH),$$(dir $(LST_PATH)/%).dirstamp)
ifeq ($(AS_VECTOR_PREPROCESS),1)
	@$(ECHO) "ASMPP      $(notdir $@)"
	$(Q)$(AS_ENV) $(ASMPP) $(ASMPP_DEPENDFLAGS) $(ASFLAGS_VECTOR_PREPROCESS) $(CPPFLAGS_VECTOR_MAKESUPPORT) $< -o $(basename $@)$(suffix $<) $(CC_REDIRECT_OUTPUT)
	@$(ECHO) "AS        ${@F}"
	$(Q)$(AS_ENV) $(AS) $(CPPFLAGS_INC_LIST) $(ASFLAGS) $(basename $@)$(suffix $<) $(CC_REDIRECT_OUTPUT)
else
	@$(ECHO) "AS         ${@F}"
	$(Q)$(AS) $(CPPFLAGS_INC_LIST) $(ASFLAGS) $< $(CC_REDIRECT_OUTPUT)
endif


############################################
#
#  clean section
#
#  Use double-colons to make it possible for
#  Makefile to add clean and distclean targets
#  instead of overwrite.
#
clean::
	@$(ECHO) "CLEAN      build files"
	$(Q)$(RM) $(OBJS:.$(OBJ_SUFFIX)=.*)
	$(Q)$(RM) $(patsubst $(OBJ_PATH)/%.$(OBJ_SUFFIX),$(LST_PATH)/%.$(LST_SUFFIX),$(OBJS))
	$(Q)$(RM) $(patsubst $(OBJ_PATH)/%.$(OBJ_SUFFIX),$(ERR_PATH)/%.$(ERR_SUFFIX),$(OBJS))
	$(Q)$(RM) $(PROJECT_NAME).$(BINARY_SUFFIX) $(PROJECT_NAME).$(LNK_SUFFIX)
	$(Q)$(RM) $(PROJECT_NAME).map $(PROJECT_NAME).mapxml $(PROJECT_NAME).mdf
	$(Q)$(RM) $(basename $(USER_LINKER_COMMAND_FILE))_Pre.$(LNK_SUFFIX)
	$(Q)$(RM) $(TARGET_RESPONSE_FILE)

distclean::
	@$(ECHO) "DISTCLEAN  build files"
	$(Q)$(RM) $(OBJS:.$(OBJ_SUFFIX)=.*)
	$(Q)$(RM) $(patsubst $(OBJ_PATH)/%.$(OBJ_SUFFIX),$(LST_PATH)/%.$(LST_SUFFIX),$(OBJS))
	$(Q)$(RM) $(patsubst $(OBJ_PATH)/%.$(OBJ_SUFFIX),$(ERR_PATH)/%.$(ERR_SUFFIX),$(OBJS))
	$(Q)$(RMDIR) $(OBJ_PATH) $(ERR_PATH) $(LST_PATH) $(LOG_PATH)
	$(Q)$(RM) $(dir $(TARGET))/.dirstamp


############################################
#
#  BRSVINFO_USED section
#
ifeq ($(BRSVINFO_USED),1)

BRSVINFO_H=$(GENDATA_DIR)/BrsVInfo.h

#
#  Helper
#
brsvinfo: $(BRSVINFO_H)

$(BRSVINFO_H):
	@$(ECHO) "Generate  $@"
	$(Q)VERSION="$(VERSION)"                     \
    SUB_VERSION="$(SUB_VERSION)"                     \
    CC_VERSION_STRING="$(CC_VERSION_STRING)"         \
    CFLAGS="$(CFLAGS)"                               \
    AS_VERSION_STRING="$(AS_VERSION_STRING)"         \
    ASFLAGS="$(ASFLAGS)"                             \
    LD_VERSION_STRING="$(LD_VERSION_STRING)"         \
    LDFLAGS="$(LDFLAGS)"                             \
    AR_VERSION_STRING="$(AR_VERSION_STRING)"         \
    ARFLAGS="$(ARFLAGS)"                             \
    EXT_T1="$(EXT_T1)"                               \
    EXT_T1_VERSION_STRING="$(EXT_T1_VERSION_STRING)" \
    EXT_T1_FLAGS="$(EXT_T1_FLAGS)"                   \
    EXT_T2="$(EXT_T2)"                               \
    EXT_T2_VERSION_STRING="$(EXT_T2_VERSION_STRING)" \
    EXT_T2_FLAGS="$(EXT_T2_FLAGS)"                   \
    COMPILER_MANUFACTURER="$(COMPILER_MANUFACTURER)" \
    DERIVATIVE="$(DERIVATIVE)"                       \
    $(MAKESUPPORT_DIR_U)/template/BrsVInfo.h.sh $@

clean::
	@$(ECHO) "CLEAN      BrsVInfo.h"
	$(Q)$(RM) $(BRSVINFO_H)
	
distclean::
	@$(ECHO) "DISTCLEAN  BrsVInfo.h"
	$(Q)$(RM) $(BRSVINFO_H)
	
endif


############################################
#
#  printmodules section
#
printmodules:
	@$(ECHO) "Available modules/libraries:"
	@$(foreach M,$(MODULE_USED_LST), $(PRINTF) "%30s : %s\n" $(M) $(call mod2lib,$(M));)

printmodulefiles:
	@$(ECHO) "Available modules:"
	@$(foreach M,$(MODULE_USED_LST), $(PRINTF) "\n  %30s : lib%s\n" $(M) $(call mod2lib,$(M)); $(foreach F,$(call mod2sources,$(M)),$(PRINTF) "%50s\n" $(F);))


############################################
#
#  copyartifacts section
#
#-----------------------------------------------------------------------------
# Copy build artifacts to specified directory
# e.g. copyartifacts dir=../Artifacts
#-----------------------------------------------------------------------------
copyartifacts:
	@$(ECHO) "Copy all build artifacts $(PROJECT_NAME).* to folder '$(dir)'"
	$(Q)$(MKDIR) $(call os_path,$(dir))
	$(Q)$(CP) $(PROJECT_NAME).* $(call os_path,$(dir))
	
############################################
#
#  info section
#
info:
	@$(ECHO) "***********************************************************"
	@$(ECHO) " *  Build Info:"
	@$(ECHO) " *"
	@$(ECHO) " *                          Version: $(VERSION)"
	@$(ECHO) " *                      Sub-Version: $(SUB_VERSION)"
	@$(ECHO) " *"
	@$(ECHO) " *"
	@$(ECHO) " *"
	@$(ECHO) " *  Number of source files to build:  "$(words $(OBJS))
	@$(ECHO) " *                       C sources :  "$(words $(OBJS_C))
	@$(ECHO) " *                     ASM sources :  "$(words $(OBJS_ASM))
	@$(ECHO) " *  Number of modules (used/total) :  "$(words $(MODULE_USED_LST))"/"$(words $(MODULE_LIST_PROJECT))
	@$(ECHO) " *"
	@$(ECHO) " *  ROOT                  : $(ROOT_U)"
	@$(ECHO) " *  PRJROOT               : $(PRJROOT_U)"
	@$(ECHO) " *  ROOT_FROM_PRJROOT     : $(ROOT_FROM_PRJROOT)"
	@$(ECHO) " *  APPL_DIR_FROM_PRJROOT : $(APPL_DIR_FROM_PRJROOT)"
	@$(ECHO) " *"
ifeq ($(V),1)
	@$(ECHO) " *  C/C++ files:"
	@$(foreach _F,$(addprefix $(ROOT_U)/,$(MODULE_SOURCE_LST_U)) $(APP_SOURCE_ALL_LST_REL_U) $(APP_SOURCE_LST_ABS_U),$(ECHO) " *    $(_F)";)
	@$(ECHO) " *"
	@$(ECHO) " *  C/C++ object files:"
	@$(foreach _F,$(OBJS_C),$(ECHO) " *    $(_F)";)
	@$(ECHO) " *"
	@$(ECHO) " *  ASM files:"
	@$(foreach _F,$(addprefix $(ROOT_U)/,$(sort $(MODULE_ASM_LST_U) $(ASM_GENERATED_SOURCE_FILES_U) $(ASM_SOURCES_U))),$(ECHO) " *    $(_F)";)
	@$(ECHO) " *"
	@$(ECHO) " *  ASM object files:"
	@$(foreach _F,$(OBJS_ASM),$(ECHO) " *    $(_F)";)
	@$(ECHO) " *"
endif
	@$(ECHO) " *"
	@$(ECHO) "***********************************************************"

############################################
#
#  DEBUG section
#
#-----------------------------------------------------------------------------
# Print variables: Display any value of a Makefile variable
# e.g. print-COMPILER_BASE
#-----------------------------------------------------------------------------
print-% :
	@$(ECHO) '$*="$($*)"'

#
#  print varaible without identifier. Used for further processing like grep, tr, ...
#
p-% :
	@$(ECHO) '$($*)'

############################################
#
#  help section
#
help::
	@echo " ***********************************************************************************"
	@echo " *"
	@echo " *  MakeSupport 4"
	@echo " *  ============="
	@echo " *"
	@echo " *  Targets:"
	@echo " *    help                             Show this screen"
	@echo " *    all (default)                    Build target libs, elf"
	@echo " *    libs,library                     Build all libraries"
	@echo " *    cleanlibs                        Clean libraries"		
	@echo " *    clean                            Clean generated files"
	@echo " *    distclean                        Clean project for delivery"
	@echo " *    compile                          Compile all files"
	@echo " *    assemble                         Assemble all files"
	@echo " *    elf,link                         Build binary target"
	@echo " *"
	@echo " *    preprocess                       Preprocess all files"
	@echo " *    preprocess file=<MyFile>.<ext>   Preprocess a single file (can be a C or ASM file)"
	@echo " *"
	@echo " *    <MyFile>.o                       Compile/assemble specific file without object path"
	@echo " *"
	@echo " *    brsvinfo                         Generate file BrsVInfo.h if BRSVINFO_USED is set to 1 in Makefile"
	@echo " *"
	@echo " *    copyartifacts dir=<relPath>      Copy build artifacts to specified directory (relative to Makefile)"	
	@echo " *"	
	@echo " *    check                            Check several conditions, e.g. include paths"
	@echo " *    info                             Display some information about files to build"
	@echo " *    show_objs                        Show object files"
	@echo " *"
ifeq ($(USE_AUTOSAR_MAKE),1)
	@echo " *    copylibs                         Copy libs to component directory"	
	@echo " *    printlibs                        Print libs to create"
	@echo " *    printlibfiles                    Print libs and their files"
endif	
	@echo " *    printmodules                     Print modules"
	@echo " *    printmodulefiles                 Print modules and their files"
	@echo " *"
	@echo " *  Control variables:"
	@echo " *  ------------------"
	@echo " *    LINK_LIBRARIES=1                 Link components as libraries instead of their object files"
	@echo " *                                       in order to reduce the length of the linker's argument list"
	@echo " *    USE_AUTOSAR_MAKE=0               Disable usage of AUTOSAR Makefiles"
	@echo " *    V=1                              Enable verbose mode for build"
	@echo " *    VV=1                             Enable verbose mode for make"
	@echo " *    REUSE_CPP_DEPEND=0               Reuse Cpp preprocessing used for dependency"
	@echo " *                                     Build optimization up 20-25%"
	@echo " *    DO_DEPEND=0                      Disable of depend files in extra cpp step. Default is 1"
	@echo " *                                     Useful for single build step without development"
	@echo " *    COMPILER_SUPPORTS_DEPEND=([0]|1) Cross compiler support cpp depend feature"
	@echo " *                                       gcc, clang cross compiler supports this feature always"
	@echo " *    AR_COMMAND_WITHOUT_SPACE=1       Use archive command without space between flags and target name"
	@echo " *                                       Cross-compiler specific option; it will be set in Makefile.static"
	@echo " *"
	@echo " *  Debugging:"
	@echo " *  ----------"
	@echo " *    p-<VAR>                          Print variable"
	@echo " *    print-<VAR>                      Print variable in verbose form"
	@echo " *"
	@echo " *"
	@echo " *  NAnt support:"
	@echo " *  -------------"
	@echo " *  To enable NAnt support, set NANT_USED to 1 in Makefile"
	@echo " *"
	@echo " *    nant_setup [PATH]                Set up NAnt / Update NAnt.build"
	@echo " *"
	@echo " *"
	@echo " *  ECO usage:"
	@echo " *  ----------"
	@echo " *  To integrate ECO into project, adapt Makefile as follows:"
	@echo " *"	
	@echo " *         include $$""(MAKESUPPORT_DIR)/Global.Makefile.target.$(VERSION).mk"
	@echo " *    -->  include Makefile_ECO.config"
	@echo " *    -->  include $$""(MAKESUPPORT_DIR_U)/quality/Makefile_ECO.mk"
	@echo " *"
	@echo " *"
	@echo " *  Useful make options:"
	@echo " *  --------------------"
	@echo " *    -j <n>                           Parallel exeuction of build"
	@echo " *    -k                               Continue in case of error as far as possible"
	@echo " *    --debug=b                        See responsibility of build"
	@echo " *    --debug=a                        See include files"
	@echo " *    -C <dir>                         Start make from another directory"
	@echo " *    -n                               No execution, but just print commands"
	@echo " *    -O[type]                         Use '-Otarget' when use -j option"
	@echo " *    -p                               Print database: Show all generated rules and variables"
	@echo " *                                       make help -p"
	@echo " *    -r                               Ignore default make-rules. This speeds a little the offset time (~0.3 s)"
	@echo " *"
	@echo " ***********************************************************************************"

#
#  Because of dependency to VERSION_INFO_FILE for non-builds, ignore dd targets
#
ifneq ($(wildcard $(VERSION_INFO_FILE)),)
  ifeq ($(VV),1)
    $(info )
    $(info include Makefile_delivery.mk)
  endif
  include $(MAKEFILE_DELIVERY)
else
  ifeq ($(VV),1)
    $(warning Warning: $(VERSION_INFO_FILE) not found -> no Makefile_delivery.mk included)
  endif
dd deliverydescription:
	@echo "warning target '$@' is not available when VERSION_INFO_FILE='$(VERSION_INFO_FILE)' is missing."
endif

include $(MAKESUPPORT_DIR_U)/Makefile_doxygen.mk

include $(MAKESUPPORT_DIR_U)/quality/Makefile_mssv.mk

include $(MAKESUPPORT_DIR_U)/quality/Makefile_vca.mk

include $(MAKESUPPORT_DIR_U)/Makefile_visual_studio.mk

include $(MAKESUPPORT_DIR_U)/Makefile_buildinfo.mk


##############################################################################
# NAntSupport section
##############################################################################
ifeq ($(NANT_USED),1)
NANT_GEN_FILE = Scripts/make/NAntGen.MakeSupport4.make

# Set default value for installation directory if it has not been set before
NANT_TARGET_DIR ?= D:/uti/NAntSupport/$(NANT_VERSION)
NANT_SOURCE_DIR ?= //vi.vector.int/Project2/DevelopmentTools/DevSupportTools/NAnt/03.latest

# Paths to NAntGen.make file
NANT_GEN_MAKE_LOCAL = $(NANT_TARGET_DIR)/$(NANT_GEN_FILE)
NANT_GEN_MAKE_REMOTE = $(NANT_SOURCE_DIR)/$(NANT_GEN_FILE)

# Try to include NAnt-files/version.make, the - prefix ensures that no error
# will be reported if the file doesn't exist
-include NAnt-files/version.make

# First try to include NAntGen.make from local directory
ifneq ($(strip $(wildcard $(NANT_GEN_MAKE_LOCAL))),)
NANT_SETUP_ROOT = $(NANT_TARGET_DIR)
-include $(NANT_GEN_MAKE_LOCAL)
else
# If include from local directory didn't work, use installer from network
NANT_SETUP_ROOT = $(NANT_SOURCE_DIR)
-include $(NANT_GEN_MAKE_REMOTE)
endif

endif #ifeq ($(NANT_USED),1)

# End of Global.Makefile.target.4.mk
