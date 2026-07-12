############################################################################### 
# File Name  : VttCrypto_30_Vtt_rules.mak 
# Description: Rules makefile 
#------------------------------------------------------------------------------
# COPYRIGHT
#------------------------------------------------------------------------------
# Copyright (c) 2019 by Vector Informatik GmbH.  All rights reserved.
#------------------------------------------------------------------------------
# REVISION HISTORY
#------------------------------------------------------------------------------
# Version   Date        Author  Description
#------------------------------------------------------------------------------
# 1.00.00   2017-11-22  Gmi     Initial Version
# 1.00.01   2018-01-04  Tof     Aded Vtt prefix to filenames
# 1.01.00   2019-02-06  vircbl  Added support of component-based SIP structure
#------------------------------------------------------------------------------
# TemplateVersion = 1.02
###############################################################################

# Component Files
CC_FILES_TO_BUILD       += Crypto_30_Vtt$(BSW_SRC_DIR)\Crypto_30_Vtt*.c
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\Crypto_30_Vtt_Cfg.c $(GENDATA_DIR)\Crypto_30_Vtt_PBcfg.c

# Library Settings
LIBRARIES_TO_BUILD      += Crypto_30_Vtt
Crypto_FILES            += Crypto_30_Vtt$(BSW_SRC_DIR)\Crypto_30_Vtt*.c

