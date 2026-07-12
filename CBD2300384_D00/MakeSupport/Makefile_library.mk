########################################################################################################################
# File Name  : Makefile_library.mk
# Description: This Sub-Makefile provides library creation targets.
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
#  Makefile_library.mk
#  ===================
#
#  This Sub-Makefile provides library creation targets.
#  It is included by Global.Makefile.target.\$(VERSION).mk if the value of 'LIBRARY_USED' is 1 and 'LIBRARY_SOURCES' is not empty.
#
#
#  Targets:
#  --------
#  - libs, library
#  - clean
#
#  @ingroup mod_Makefile
#


.PHONY: library libs

TARGET_LIBRARY=$(LIB_PATH)/lib$(PROJECT_NAME).$(LIB_SUFFIX)

library libs:: $(TARGET_LIBRARY)

LIBRARY_OBJECTS=$(addprefix $(OBJ_PATH)/$(ROOT_FROM_PRJROOT)/, \
                   $(call os_path, \
                     $(call extsubst,$(ASM_SUFFIXES) c cpp c++,$(OBJ_SUFFIX),$(LIBRARY_SOURCES)) \
                    ) \
                 )

# Depending on the used archiver a space between the flags and the target name is needed or not.
# If necessary, the variable $(AR_COMMAND_WITHOUT_SPACE) is set to '1' in the Makefile.static
ifneq ($(AR_COMMAND_WITHOUT_SPACE),1)
$(TARGET_LIBRARY): $(LIBRARY_OBJECTS)  $(dir $(TARGET_LIBRARY))/.dirstamp
	@$(ECHO) "AR         $@"
	@$(RM) $@
	$(Q)$(AR_ENV) $(AR) $(ARFLAGS) $@ $(LIBRARY_OBJECTS) $(AR_REDIRECT_OUTPUT)
else
$(TARGET_LIBRARY): $(LIBRARY_OBJECTS)  $(dir $(TARGET_LIBRARY))/.dirstamp
	@$(ECHO) "AR         $@"
	@$(RM) $@
	$(Q)$(AR_ENV) $(AR) $(ARFLAGS)$@ $(LIBRARY_OBJECTS) $(AR_REDIRECT_OUTPUT)
endif

.SECONDARY:: $(LIB_PATH)/.dirstamp

cleanlibs::
	@echo "CLEAN      libraries"
	$(Q)$(RM) $(TARGET_LIBRARY)
	$(Q)$(RMDIR) $(LIB_PATH_U)
	
clean:: cleanlibs
