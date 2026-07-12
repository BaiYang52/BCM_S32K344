############################################################################################
# File Name  : vItaHlp_rules.mak
# Description: Rules makefile
#-------------------------------------------------------------------------------------------
# COPYRIGHT
#-------------------------------------------------------------------------------------------
# Copyright (c) 2020 by Vector Informatik GmbH.  All rights reserved.
#-------------------------------------------------------------------------------------------
# REVISION HISTORY
#-------------------------------------------------------------------------------------------
# Version   Date        Author  Description
#-------------------------------------------------------------------------------------------
# 3.00.00   2019-02-25  visppy  Initial Version
# 4.00.00   2019-05-13  visppy  Testmodule OS
# 4.01.00   2019-07-09  vistun  Testmodule ETH
# 5.00.00   2019-08-30  visppy  Testmodule FLS
#           2019-10-31  visppy  Testmodule MCU
# 5.01.00   2019-12-11  visppy  Testmodule LIN
#           2020-02-06  visppy  Testmodule WDG
# 5.01.01   2021-03-12  visppy  Fix FILES name
#-------------------------------------------------------------------------------------------
# TemplateVersion = 1.02
############################################################################################

# Component files
CC_FILES_TO_BUILD       +=  vItaHlp$(BSW_SRC_DIR)\vItaHlp.c             \
                            vItaHlp$(BSW_SRC_DIR)\vItaHlp_Can.c         \
                            vItaHlp$(BSW_SRC_DIR)\vItaHlp_Eth.c         \
                            vItaHlp$(BSW_SRC_DIR)\vItaHlp_Os.c          \
                            vItaHlp$(BSW_SRC_DIR)\vItaHlp_Fls.c         \
                            vItaHlp$(BSW_SRC_DIR)\vItaHlp_Mcu.c         \
                            vItaHlp$(BSW_SRC_DIR)\vItaHlp_Lin.c         \
                            vItaHlp$(BSW_SRC_DIR)\vItaHlp_Wdg.c         \
                            vItaHlp$(BSW_SRC_DIR)\vItaHlp_WdgService.c \
                            vItaHlp$(BSW_SRC_DIR)\vItaHlp_ComIf.c       \
                            vItaHlp$(BSW_SRC_DIR)\vItaHlp_CommonUtils.c \
                            vItaHlp$(BSW_SRC_DIR)\vItaHlp_EventQueue.c  \
                            vItaHlp$(BSW_SRC_DIR)\vItaHlp_TestCom.c

GENERATED_SOURCE_FILES  +=  $(GENDATA_DIR)\vItaHlp_Can_Cfg.c            \
                            $(GENDATA_DIR)\vItaHlp_Eth_Cfg.c            \
                            $(GENDATA_DIR)\vItaHlp_Os_Cfg.c             \
                            $(GENDATA_DIR)\vItaHlp_Fls_Cfg.c            \
                            $(GENDATA_DIR)\vItaHlp_Mcu_Cfg.c            \
                            $(GENDATA_DIR)\vItaHlp_Lin_Cfg.c            \
                            $(GENDATA_DIR)\vItaHlp_Wdg_Cfg.c            \
                            $(GENDATA_DIR)\vItaHlp_TestCom_Cfg.c

# Library Settings
LIBRARIES_TO_BUILD      +=
vItaHlp_FILES           +=
