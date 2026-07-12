###############################################################################
# File Name  : Mcal_S32k_rules.mak                                            #
# Description: Autosar makefile Template                                      #
#              This makefile is a template to implement the common            #
#              features of each project.                                      #
#              It is included by the Global.Makefile.target.make.$(Version)   #
#              and is supported from version 3.24 .                           #
#                                                                             #
# This Template is based on AUTOSAR_BSW_MakefileInterface.doc version 0.4     #
#                                                                             #
#-----------------------------------------------------------------------------#
#               C O P Y R I G H T                                             #
#-----------------------------------------------------------------------------#
# Copyright (c) 2022 by Vector Informatik GmbH.  All rights reserved.         #
#                                                                             #
#-----------------------------------------------------------------------------#
#               R E V I S I O N   H I S T O R Y                               #
#-----------------------------------------------------------------------------#
# Date         Version  Sign    Description                                   #
# ----------   -------  ------  ----------------------------------------------#
# 2020-05-18   1.00.00  virgaj 	initial MIP;								  #
#								integration of S32K2XX_MCAL4_3_EAR_HF3_0_8_1 /#
#								S32K2XX_Crypto_MCAL4_3_EAR_0_8_1			  #
# 2020-10-15   2.00.00  virelt	Integration of SW32K3_RTD_4.4_0.8.0			  #
# 2020-10-26   2.00.01	virnid	Integration of SW32K3_RTD_4.4_0.8.0_P02		  #
# 2020-11-30   2.01.00	virnid	Integration of SW32K3_RTD_4.4_0.8.1			  #
# 2021-01-12   2.02.00  virnid  Integration of SW32K3_RTD_4.4_0.8.1_FBR_1.1   #
# 2021-03-30   2.03.00	virnid	Integration of SW32K3_RTD_4.4_0.9.0			  #
# 2021-12-08   2.04.00	virnid	Integration of Sbc and Spd					  #
# 2022-07-14   2.05.00	virnid	Integration of SW32K3_RTD_4.4_2.0.1			  #
#								and SW32K396_RTD_4.4_3.0.0_CD02				  #
# 2022-11-02   2.06.00	virnid	Integration of SW32K358_RTD_R21-11_3.0.0_CD03 #
#								and SW32K396_RTD_4.4_3.0.0_CD02_P01			  #
# 2023-01-02   2.07.00	jmaier	Integration of SW32K3_RTD_4.4_2.0.2 and 	  #
#								SW32K358_RTD_R21_11_3.0.0_CD03_P01_D2211 	  #
# 2023-01-03   2.08.00	virnid	Integration of SW32K3XX_RTD_R21-11_3.0.0_CD04 #
# 2023-02-03   2.08.01  jmaier  remove linker feature						  #
###############################################################################


###############################################################
# REGISTRY
#

#Defaults for component switches
#Can be overwritten in Makefile.project.part.defines
ifeq ($(MCAL_EXCLUDE_ADC),)
MCAL_EXCLUDE_ADC = 0
endif
ifeq ($(MCAL_EXCLUDE_BASE),)
MCAL_EXCLUDE_BASE = 0
endif
ifeq ($(MCAL_EXCLUDE_DIO),)
MCAL_EXCLUDE_DIO = 0
endif
ifeq ($(MCAL_EXCLUDE_ETH),)
MCAL_EXCLUDE_ETH = 1
endif
ifeq ($(MCAL_EXCLUDE_FEE),)
MCAL_EXCLUDE_FEE = 1
endif
ifeq ($(MCAL_EXCLUDE_FLS),)
MCAL_EXCLUDE_FLS = 0
endif
ifeq ($(MCAL_EXCLUDE_GPT),)
MCAL_EXCLUDE_GPT = 0
endif
ifeq ($(MCAL_EXCLUDE_I2C),)
MCAL_EXCLUDE_I2C = 0
endif
ifeq ($(MCAL_EXCLUDE_ICU),)
MCAL_EXCLUDE_ICU = 0
endif
ifeq ($(MCAL_EXCLUDE_MCL),)
MCAL_EXCLUDE_MCL = 0
endif
ifeq ($(MCAL_EXCLUDE_MCU),)
MCAL_EXCLUDE_MCU = 0
endif
ifeq ($(MCAL_EXCLUDE_OCU),)
MCAL_EXCLUDE_OCU = 0
endif
ifeq ($(MCAL_EXCLUDE_PORT),)
MCAL_EXCLUDE_PORT = 0
endif
ifeq ($(MCAL_EXCLUDE_PWM),)
MCAL_EXCLUDE_PWM = 0
endif
ifeq ($(MCAL_EXCLUDE_SPI),)
MCAL_EXCLUDE_SPI = 0
endif
ifeq ($(MCAL_EXCLUDE_UART),)
MCAL_EXCLUDE_UART = 0
endif
ifeq ($(MCAL_EXCLUDE_WDG),)
MCAL_EXCLUDE_WDG = 0
endif

ifeq ($(MCAL_EXCLUDE_SBC_FS_26),)
MCAL_EXCLUDE_SBC_FS_26 = 0
endif
ifeq ($(MCAL_EXCLUDE_WDG_43_FS26),)
MCAL_EXCLUDE_WDG_43_FS26 = 0
endif

ifeq ($(MCAL_EXCLUDE_BIST),)
MCAL_EXCLUDE_BIST = 0
endif
ifeq ($(MCAL_EXCLUDE_EMCEM),)
MCAL_EXCLUDE_EMCEM = 0
endif
ifeq ($(MCAL_EXCLUDE_SAFETYBASE),)
MCAL_EXCLUDE_SAFETYBASE = 0
endif

ifeq ($(MCAL_EXCLUDE_CRY),)
MCAL_EXCLUDE_CRY = 0
endif
ifeq ($(EXCLUDE_HSE_PACKAGE),)
EXCLUDE_HSE_PACKAGE = 1
endif
#Path to Mcal source and include files
MCAL_PATH = ..\ThirdParty\Mcal_S32k\Supply\SW32K3_RTD_4.4_2.0.1
HSE_PATH =
SPD_PATH = 
SBC_PATH = 

MCAL_MOD_V = T40D34M20I1R0
SPD_MOD_V = 
SBC_MOD_V = 

ASR_VERSION = 4.4.0

BASE_PACKAGE_NAME = Base

ETH_PACKAGE_NAME =

ADC_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\Adc_TS_$(MCAL_MOD_V)
CRYPTO_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\Crypto_TS_$(MCAL_MOD_V)
DIO_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\Dio_TS_$(MCAL_MOD_V)
ETH_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\$(ETH_PACKAGE_NAME)_TS_$(MCAL_MOD_V)
FEE_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\Fee_TS_$(MCAL_MOD_V)
FLS_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\Fls_TS_$(MCAL_MOD_V)
GPT_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\Gpt_TS_$(MCAL_MOD_V)
I2C_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\I2c_TS_$(MCAL_MOD_V)
ICU_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\Icu_TS_$(MCAL_MOD_V)
MCL_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\Mcl_TS_$(MCAL_MOD_V)
MCU_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\Mcu_TS_$(MCAL_MOD_V)
OCU_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\Ocu_TS_$(MCAL_MOD_V)
PORT_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\Port_TS_$(MCAL_MOD_V)
PWM_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\Pwm_TS_$(MCAL_MOD_V)
SPI_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\Spi_TS_$(MCAL_MOD_V)
UART_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\Uart_TS_$(MCAL_MOD_V)
WDG_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\Wdg_TS_$(MCAL_MOD_V)

BASE_BASE_PATH = $(MCAL_PATH)\eclipse\plugins\$(BASE_PACKAGE_NAME)_TS_$(MCAL_MOD_V)

SBC_FS_26_BASE_PATH = $(SBC_PATH)\eclipse\plugins\Sbc_fs26_TS_$(SBC_MOD_V)
WDG_FS_26_BASE_PATH = $(SBC_PATH)\eclipse\plugins\Wdg_43_fs26_TS_$(SBC_MOD_V)

BIST_BASE_PATH = $(SPD_PATH)\eclipse\plugins\Bist_TS_$(SPD_MOD_V)
EMCEM_BASE_PATH = $(SPD_PATH)\eclipse\plugins\eMcem_TS_$(SPD_MOD_V)
SAFETYBASE_BASE_PATH = $(SPD_PATH)\eclipse\plugins\SafetyBase_TS_$(SPD_MOD_V)

HSE_BASE_PATH = $(HSE_PATH)\hse_full_mem

ifneq ($(MCAL_EXCLUDE_ADC),1)
CC_FILES_TO_BUILD  += $(ADC_BASE_PATH)\src\Adc*.c	\
					  $(ADC_BASE_PATH)\src\Bctu*.c	\
					  $(ADC_BASE_PATH)\src\DSPSS_Api*.c		\
					  $(ADC_BASE_PATH)\src\Sdadc*.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\Adc_*.c	\
							$(GENDATA_DIR)\src\Bctu*.c	\
							$(GENDATA_DIR)\src\Sdadc*.c
							
CC_INCLUDE_PATH    += $(ADC_BASE_PATH)\include		\
						$(ADC_BASE_PATH)\src
endif

ifneq ($(MCAL_EXCLUDE_CRY),1)
CC_FILES_TO_BUILD  +=   $(CRYPTO_BASE_PATH)\src\Crypto*.c	\
						$(CRYPTO_BASE_PATH)\src\Hse_*.c		\
						$(CRYPTO_BASE_PATH)\src\Mu_*.c

CC_INCLUDE_PATH   += $(CRYPTO_BASE_PATH)\include

GENERATED_SOURCE_FILES += $(GENDATA_DIR)\src\Crypto_*.c		\
							$(GENDATA_DIR)\src\HseSrv*.c


ifneq ($(EXCLUDE_HSE_PACKAGE),1)
#HSE package					 
CC_INCLUDE_PATH += $(HSE_BASE_PATH)\interface\config	\
					$(HSE_BASE_PATH)\interface\inc_common \
					$(HSE_BASE_PATH)\interface\inc_custom \
					$(HSE_BASE_PATH)\interface\inc_services \
					$(HSE_BASE_PATH)\interface
endif

ifneq ($(EXCLUDE_HSE_PACKAGE),0)
#HSE Example files from Crypto module; used if HSE package is not available
CC_INCLUDE_PATH += $(CRYPTO_BASE_PATH)\examples\S32DS\Crypto_Example_001\include
endif

endif

ifneq ($(MCAL_EXCLUDE_DIO),1)
CC_FILES_TO_BUILD    += $(DIO_BASE_PATH)\src\Dio*.c	\
						$(DIO_BASE_PATH)\src\Siul2*.c
						
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\Dio_*.c

CC_INCLUDE_PATH    += $(DIO_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_ETH),1)
CC_FILES_TO_BUILD  +=   $(ETH_BASE_PATH)\src\Eth*.c	\
						$(ETH_BASE_PATH)\src\Gmac*.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\Eth_*.c \
							$(GENDATA_DIR)\src\Gmac*.c						

CC_INCLUDE_PATH    += $(ETH_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_FEE),1)
CC_FILES_TO_BUILD  += $(FEE_BASE_PATH)\src\Fee*.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\Fee_*.c 

CC_INCLUDE_PATH    += $(FEE_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_FLS),1)
CC_FILES_TO_BUILD  += $(FLS_BASE_PATH)\src\Fls*.c	\
					  $(FLS_BASE_PATH)\src\C40*.c	\
					  $(FLS_BASE_PATH)\src\Qspi_Ip*.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\Fls_*.c \
							$(GENDATA_DIR)\src\C40*.c \
							$(GENDATA_DIR)\src\Qspi*.c 

CC_INCLUDE_PATH    += $(FLS_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_GPT),1)
CC_FILES_TO_BUILD  += $(GPT_BASE_PATH)\src\Gpt*.c	\
					  $(GPT_BASE_PATH)\src\*_Ip.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\Gpt_*.c \
							$(GENDATA_DIR)\src\Pit*.c \
							$(GENDATA_DIR)\src\Rtc*.c \
							$(GENDATA_DIR)\src\Emios_Gpt*.c \
							$(GENDATA_DIR)\src\Stm*.c 

CC_INCLUDE_PATH    += $(GPT_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_I2C),1)
CC_FILES_TO_BUILD  += $(I2C_BASE_PATH)\src\CDD_I2c*.c	\
					  $(I2C_BASE_PATH)\src\Flexio_*.c	\
					  $(I2C_BASE_PATH)\src\I2c*.c		\
					  $(I2C_BASE_PATH)\src\Lpi2c*.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\CDD_I2c*.c  \
						   $(GENDATA_DIR)\src\Flexio_I2c*.c \
						   $(GENDATA_DIR)\src\I2c*.c \
						   $(GENDATA_DIR)\src\Lpi2c*.c

CC_INCLUDE_PATH    += $(I2C_BASE_PATH)\include 
endif

ifneq ($(MCAL_EXCLUDE_ICU),1)
CC_FILES_TO_BUILD  +=   $(ICU_BASE_PATH)\src\Icu*.c	\
						$(ICU_BASE_PATH)\src\*_Irq.c	\
						$(ICU_BASE_PATH)\src\*_Ip.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\Icu*.c  \
						   $(GENDATA_DIR)\src\Wkpu*.c \
						   $(GENDATA_DIR)\src\Cmp*.c \
						   $(GENDATA_DIR)\src\Emios_Icu*.c \
						   $(GENDATA_DIR)\src\Siul2_Icu*.c 

CC_INCLUDE_PATH    += $(ICU_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_MCL),1)
CC_FILES_TO_BUILD  += $(MCL_BASE_PATH)\src\CDD_Mcl*.c \
					  $(MCL_BASE_PATH)\src\*_Common.c \
					  $(MCL_BASE_PATH)\src\Mcl*.c \
					  $(MCL_BASE_PATH)\src\Cache*.c \
					  $(MCL_BASE_PATH)\src\Dma*.c \
					  $(MCL_BASE_PATH)\src\Emios_Mcl*.c \
					  $(MCL_BASE_PATH)\src\Flexio_Mcl*.c \
					  $(MCL_BASE_PATH)\src\Lcu*.c \
					  $(MCL_BASE_PATH)\src\Trgmux*.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\CDD_Mcl*.c \
							$(GENDATA_DIR)\src\Dma*.c \
							$(GENDATA_DIR)\src\Emios_Mcl*.c \
							$(GENDATA_DIR)\src\Flexio_Mcl*.c \
							$(GENDATA_DIR)\src\Lcu*.c \
							$(GENDATA_DIR)\src\Trgmux*.c 
					  
CC_INCLUDE_PATH    += $(MCL_BASE_PATH)\include	\
						$(MCL_BASE_PATH)\src
endif

ifneq ($(MCAL_EXCLUDE_MCU),1)
CC_FILES_TO_BUILD  += $(MCU_BASE_PATH)\src\Mcu*.c \
					  $(MCU_BASE_PATH)\src\Clock*.c \
					  $(MCU_BASE_PATH)\src\Power*.c \
					  $(MCU_BASE_PATH)\src\Ram*.c \
					  $(MCU_BASE_PATH)\src\MC_*.c \
					  $(MCU_BASE_PATH)\src\PMC_*.c
						
GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\Mcu_*.c \
							$(GENDATA_DIR)\src\Clock*.c \
							$(GENDATA_DIR)\src\Power*.c \
							$(GENDATA_DIR)\src\Ram*.c 

CC_INCLUDE_PATH    += $(MCU_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_OCU),1)
CC_FILES_TO_BUILD  += $(OCU_BASE_PATH)\src\Ocu*.c \
					  $(OCU_BASE_PATH)\src\Emios*.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\Ocu_*.c \
							$(GENDATA_DIR)\src\Emios_Ocu*.c

CC_INCLUDE_PATH    += $(OCU_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_PORT),1)
CC_FILES_TO_BUILD  += $(PORT_BASE_PATH)\src\Port*.c \
					  $(PORT_BASE_PATH)\src\*_Ip.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\Port_*.c			\
							$(GENDATA_DIR)\src\Siul2_Port*.c	\
							$(GENDATA_DIR)\src\Tspc_Port*.c		\
							$(GENDATA_DIR)\src\Igf_Port*.c
					  
CC_INCLUDE_PATH    += $(PORT_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_PWM),1)
CC_FILES_TO_BUILD  += $(PWM_BASE_PATH)\src\Pwm*.c		\
					  $(PWM_BASE_PATH)\src\Emios*.c		\
					  $(PWM_BASE_PATH)\src\FlexPwm*.c	\
					  $(PWM_BASE_PATH)\src\Flexio*.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\Pwm_*.c			\
							$(GENDATA_DIR)\src\Emios_Pwm*.c		\
							$(GENDATA_DIR)\src\FlexPwm*.c		\
							$(GENDATA_DIR)\src\Flexio_Pwm*.c
				  
CC_INCLUDE_PATH    += $(PWM_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_SPI),1)
CC_FILES_TO_BUILD  +=   $(SPI_BASE_PATH)\src\Spi*.c \
						$(SPI_BASE_PATH)\src\Flexio_Spi*.c \
						$(SPI_BASE_PATH)\src\Lpspi*.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\Spi_*.c \
							$(GENDATA_DIR)\src\Flexio_Spi*.c \
							$(GENDATA_DIR)\src\Lpspi*.c 
CC_INCLUDE_PATH    += $(SPI_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_UART),1)
CC_FILES_TO_BUILD  +=   $(UART_BASE_PATH)\src\Uart*.c \
						$(UART_BASE_PATH)\src\Flexio_Uart*.c \
						$(UART_BASE_PATH)\src\Lpuart*.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\Uart*.c \
							$(GENDATA_DIR)\src\Flexio_Uart*.c \
							$(GENDATA_DIR)\src\Lpuart*.c 
						
CC_INCLUDE_PATH    += $(UART_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_WDG),1)
CC_FILES_TO_BUILD  += $(WDG_BASE_PATH)\src\Wdg*.c \
					  $(WDG_BASE_PATH)\src\Swt*.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\Wdg_43_Instance*.c \
							$(GENDATA_DIR)\src\Wdg_Cfg*.c \
							$(GENDATA_DIR)\src\Swt*.c 
												  
CC_INCLUDE_PATH    += $(WDG_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_SBC_FS_26),1)
CC_FILES_TO_BUILD  += $(SBC_FS_26_BASE_PATH)\src\CDD_Sbc_fs26*.c \
					  $(SBC_FS_26_BASE_PATH)\src\sbc*.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\CDD_Sbc_fs26*.c
			  
CC_INCLUDE_PATH    += $(SBC_FS_26_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_WDG_43_FS26),1)
CC_FILES_TO_BUILD  += $(WDG_FS_26_BASE_PATH)\src\Wdg*.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\Wdg_43_fs26*.c
				  
CC_INCLUDE_PATH    += $(WDG_FS_26_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_BIST),1)
CC_FILES_TO_BUILD  += $(BIST_BASE_PATH)\src\Bist*.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\Bist*.c
			
CC_INCLUDE_PATH    += $(BIST_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_EMCEM),1)
CC_FILES_TO_BUILD  += $(EMCEM_BASE_PATH)\src\eMcem*.c

GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\eMcem*.c
					
CC_INCLUDE_PATH    += $(EMCEM_BASE_PATH)\include
endif

ifneq ($(MCAL_EXCLUDE_SAFETYBASE),1)
# CC_FILES_TO_BUILD  += $(SAFETYBASE_BASE_PATH)\src\SafetyBase*.c
						
CC_INCLUDE_PATH    += $(SAFETYBASE_BASE_PATH)\include

# GENERATED_SOURCE_FILES  += $(GENDATA_DIR)\src\SafetyBase*.c
endif

CC_INCLUDE_PATH    += $(BASE_BASE_PATH)\include
CC_INCLUDE_PATH    += $(BASE_BASE_PATH)\header

ifneq ($(MCAL_EXCLUDE_BASE),1)
CC_FILES_TO_BUILD += $(BASE_BASE_PATH)\src\OsIf_Timer*.c

GENERATED_SOURCE_FILES += $(GENDATA_DIR)\src\OsIf_*.c
endif

ADDITIONAL_INCLUDES     += $(GENDATA_DIR)\include


CPP_FILES_TO_BUILD      +=
ASM_FILES_TO_BUILD      +=

#LIBRARIES_LINK_ONLY     += (not yet supported)
#OBJECTS_LINK_ONLY       += (not yet supported)

#-------------------------------------------------------------------------------------------------
#only define new dirs, OBJ, LIB, LOG were created automaticly
#-------------------------------------------------------------------------------------------------
DIRECTORIES_TO_CREATE   +=

#DEPEND_GCC_OPTS         += (not yet supported)






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


