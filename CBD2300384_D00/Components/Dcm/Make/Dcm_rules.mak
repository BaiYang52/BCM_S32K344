############################################################################### 
# File Name  : Dcm_rules.mak 
# Description: Rules makefile 
#------------------------------------------------------------------------------
# COPYRIGHT
#------------------------------------------------------------------------------
# Copyright (c) 2022 by Vector Informatik GmbH.  All rights reserved.
#------------------------------------------------------------------------------
# REVISION HISTORY
#------------------------------------------------------------------------------
# Version   Date        Author  Description
#------------------------------------------------------------------------------
# 1.00.00   2012-06-29  vissa   initial CFG5 version derived from
#                               Gw_AsrPduR_Cfg5
# 3.00.00   2014-11-20  vissa   Added Dcm_PBCfg.c
# 4.00.00   2015-02-12  vissa   New file infrastructure structure
# 4.01.00   2019-02-06  vircbl  Added support of component-based SIP structure
# 5.00.00   2020-12-18  visvkr  DCM-3391: SafeBSW for DCM - Unit Debug
# 5.01.00   2021-02-13  visepp  DCM-3722: SafeBSW for DCM - Unit Utility
# 5.02.00   2021-03-02  visepp  DCM-3915: SafeBSW for DCM - Unit Resource Manager
# 5.03.00   2021-03-17  visepp  DCM-4050: SafeBSW for DCM - Unit FBL Manager
#                       visuvo  DCM-4057: SafeBSW for DCM - Unit FBL Services
# 5.04.00   2021-05-05  visvkr  DCM-4096: SafeBSW for DCM - Unit Variant Manager
# 5.05.00   2021-05-14  visvkr  DCM-4092: SafeBSW for DCM - Unit Repeater
#           2021-05-21  visepp  DCM-4094: SafeBSW for DCM - Unit DEM Interface
# 5.06.00   2021-06-14  viswse  DCM-4102: SafeBSW for DCM - Unit Service 0x04
#           2021-06-14  viswse  DCM-4100: SafeBSW for DCM - Unit Service 0x14
#           2021-06-19  visvkr  DCM-4546: SafeBSW for DCM - Unit Configuration Wrapper
#           2021-06-23  viswse  DCM-4104: SafeBSW for DCM - Unit OBD-ID Manager
# 5.07.00   2021-07-09  visvkr  DCM-4106: SafeBSW for DCM - Unit Service 0x01
#           2021-07-09  visvkr  DCM-4114: SafeBSW for DCM - Unit Service 0x02
#           2021-07-19  visygr  DCM-4110: SafeBSW for DCM - Unit Service 0x08
#           2021-07-20  visygr  DCM-4112: SafeBSW for DCM - Unit Service 0x09
# 5.08.00   2021-08-05  viswse  DCM-4108: SafeBSW for DCM - Unit Service 0x06
#           2021-08-12  viswse  DCM-4116: SafeBSW for DCM - Unit OBD-DTC Manager
#           2021-08-19  visvkr  DCM-4119: SafeBSW for DCM - Unit OBD-DTC Services
# 5.09.00   2021-08-31  visvkr  DCM-4138: SafeBSW for DCM - Unit Service 0x3E
#           2021-09-10  visvkr  DCM-4098: SafeBSW for DCM - Unit Task Manager
#           2021-09-21  viswse  DCM-4842: SafeBSW for DCM - Unit Service 0x19 Utility - Part I
# 5.10.00   2021-10-07  visahe  DCM-4124: SafeBSW for DCM - Unit Mode Manager
# 5.11.00   2021-10-25  viswse  DCM-4140: SafeBSW for DCM - Unit Service 0x19
#           2021-10-29  viswse  DCM-4126: SafeBSW for DCM - Unit Service 0x11
# 5.12.00   2021-11-23  visuvo  DCM-4132: SafeBSW for DCM - Unit Memory Manager
#           2021-11-23  visepp  DCM-4136: SafeBSW for DCM - Unit Service 0x3D
#           2021-11-25  visepp  DCM-4134: SafeBSW for DCM - Unit Service 0x23
#           2021-11-25  visepp  DCM-4128: SafeBSW for DCM - Unit RID Manager
#           2021-12-01  visepp  DCM-4130: SafeBSW for DCM - Unit Service 0x31
#           2021-12-10  visepp  DCM-4142: SafeBSW for DCM - Unit Service 0x85
# 5.13.00   2021-12-20  visahe  DCM-4154: SafeBSW for DCM - Unit Service 0x28
# 5.14.00   2022-03-15  viswse  DCM-4156: SafeBSW for DCM - Unit Timer
# 5.15.00   2022-04-20  visahe  DCM-4152: SafeBSW for DCM - Unit Service 0x24
# 5.16.00   2022-05-09  visahe  DCM-4146: SafeBSW for DCM - Unit Service 0x22
#           2022-05-09  visahe  DCM-4150: SafeBSW for DCM - Unit Service 0x2F
#           2022-05-17  viswse  DCM-4148: SafeBSW for DCM - Unit Service 0x2E
#           2022-05-23  visahe  DCM-4158: SafeBSW for DCM - Unit Service 0x27
#           2022-05-23  viswse  DCM-4160: SafeBSW for DCM - Unit Authentication Manager
#           2022-05-30  visuvo  DCM-6032: SafeBSW for DCM - Optimize include structure
# 5.17.00   2022-06-13  viswse  DCM-4162: SafeBSW for DCM - Unit Service 0x29
#           2022-06-21  visahe  DCM-3430: SafeBSW for DCM - Unit Service 0x2A
#------------------------------------------------------------------------------
# TemplateVersion = 1.02
###############################################################################


###############################################################
# REGISTRY
#

#e.g.: LIBRARIES_TO_BUILD      +=    <MSN>
#e.g.: <MSN>_FILES = <MSN>\<MSN>.c
LIBRARIES_TO_BUILD      += Dcm
Dcm_FILES               = Dcm$(BSW_SRC_DIR)\Dcm.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Debug.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Uti.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_RsrcMgr.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_FblMgr.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc34.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc36.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc37.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_VarMgr.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Repeater.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_DemIf.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc01.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc02.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc03.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc04.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc06.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc07.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc08.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc09.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc0A.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc11.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc14.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc19.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc22.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc23.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc24.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc27.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc28.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc29.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc2A.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc2ASchd.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc2E.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc2F.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc31.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc3D.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc3E.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc85.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_CfgWrap.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_ObdIdMgr.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_ObdDtcMgr.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_PagedBuffer.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Net.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_State.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Tsk.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Diag.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_DidMgr.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Svc19Uti.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Mode.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_MemMgr.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_RidMgr.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Tmr.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_AuthMgr.c
Dcm_FILES               += Dcm$(BSW_SRC_DIR)\Dcm_Ext.c


# e.g.: CC_FILES_TO_BUILD       += <MSN>\<MSN>.c <MSN>\<MSN>_Irq.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Debug.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Uti.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_RsrcMgr.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_FblMgr.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc34.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc36.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc37.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_VarMgr.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Repeater.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_DemIf.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc01.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc02.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc03.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc04.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc06.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc07.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc08.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc09.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc0A.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc11.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc14.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc19.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc22.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc23.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc24.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc27.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc28.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc29.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc2A.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc2ASchd.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc2E.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc2F.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc31.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc3D.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc3E.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc85.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_CfgWrap.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_ObdIdMgr.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_ObdDtcMgr.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_PagedBuffer.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Net.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_State.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Tsk.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Diag.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_DidMgr.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Svc19Uti.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Mode.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_MemMgr.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_RidMgr.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Tmr.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_AuthMgr.c
CC_FILES_TO_BUILD       += Dcm$(BSW_SRC_DIR)\Dcm_Ext.c
CPP_FILES_TO_BUILD      +=
ASM_FILES_TO_BUILD      +=

#LIBRARIES_LINK_ONLY     += (not yet supported)
#OBJECTS_LINK_ONLY       += (not yet supported)

#-------------------------------------------------------------------------------------------------
#only define new dirs, OBJ, LIB, LOG were created automaticly 
#-------------------------------------------------------------------------------------------------
DIRECTORIES_TO_CREATE   +=

#DEPEND_GCC_OPTS         += (not yet supported)

# e.g.:  GENERATED_SOURCE_FILES += $(GENDATA_DIR)\drv_par.c
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\Dcm_Lcfg.c
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\Dcm_PBcfg.c

#e.g.: COMMON_SOURCE_FILES     += $(GENDATA_DIR)\v_par.c
COMMON_SOURCE_FILES     += 

#-------------------------------------------------------------------------------------------------
# <project>.dep & <projekt>.lnk & <project>.bin and.....
# all in err\ & obj\ & lst\ & lib\ & log\ will be deleted by clean-rule automaticly
# so in this clean-rule it is only necessary to define additional files which
# were not delete automaticly.
# e.g.: $(<PATH>)\can_test.c
#-------------------------------------------------------------------------------------------------
MAKE_CLEAN_RULES        +=
#MAKE_GENERATE_RULES     +=
#MAKE_COMPILER_RULES     +=
#MAKE_DEBUG_RULES        +=
#MAKE_CONFIG_RULES       +=
#MAKE_ADD_RULES          +=


###############################################################
# REQUIRED   (defined in BaseMake (global.Makefile.target.make...))
#
# SSC_ROOT		(required)
# PROJECT_ROOT	(required)
#
# LIB_OUTPUT_PATH	(optional)
# OBJ_OUTPUT_PATH	(optional)
#
# OBJ_FILE_SUFFIX	
# LIB_FILE_SUFFIX
#
###############################################################


###############################################################
# PROVIDE   this Section can be used to define own additional rules
#
# In vendorx_can_cfg.mak:
# Please configure the project file:
#CAN_CONFIG_FILE = $(PROJECT_ROOT)\source\network\can\my_can_config.cfg

#In vendorx_can_config :
#generate_can_config:
#$(SSC_ROOT)\core\com\can\tools\canconfiggen.exe -o $(CAN_CONFIG_FILE)


###############################################################
# SPECIFIC
#
# There are no rules defined for the Specific part of the 
# Rules-Makefile. Each author is free to create temporary 
# variables or to use other resources of GNU-MAKE
#
###############################################################


