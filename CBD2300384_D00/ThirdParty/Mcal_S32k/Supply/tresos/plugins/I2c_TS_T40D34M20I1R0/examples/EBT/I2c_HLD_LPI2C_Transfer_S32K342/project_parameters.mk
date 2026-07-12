#The path to the GCC installation dir
GCC_DIR = C:/gcc/bin

#The path to the EB Tresos installation dir
TRESOS_DIR = D:/Build_tools/EB/tresos_28_2_0

#The path to the T32 installation dir
T32_DIR = C:Build_tools

#The path to the VDK installation dir
VDK_DIR = 

#The path to the Tresos plugins directory
PLUGINS_DIR = ../../../../

#The path to the Tresos add-on plugins directory
PLUGINS_DIR_ADDON = ../../../../

#The paths to the additional directories to be included at build phase
ADDITIONAL_INCLUDE_DIRS = 

#Loading project to ram or flash
LOAD_TO=

# ------------------------------------------------------------------------------------
#Example specific parameters - do not modify 

#MCAL modules used
MCAL_MODULE_LIST := Base Det Dem Rte Platform Os Mcu Port I2c Resource Mcl

#MCAL modules used - only for examples based on 2 software products
MCAL_MODULE_LIST_ADDON := 

#The package name for the MCAL release
AR_PKG_NAME = TS_T40D34M20I1R0

#The package name for the MCAL release - only for examples based on 2 software products
AR_PKG_NAME_ADDON = 

#The derivative of the device
EXAMPLE_DERIVATIVE = S32K342

#The core type of the device
EXAMPLE_CORE_TYPE = M7

#The name of the elf file
ELFNAME = main

#The test base address 
TEST_BASE_ADDR = 0x2040BF00

# The test block size
TEST_BLOCK_SIZE = 255

#The name of the Tresos project 
TRESOS_PROJECT_NAME = I2c_HLD_LPI2C_Transfer_S32K342

#The number of variants for the Tresos project 
VARIANT_NO = 1

#Connection type of lauterbach
PBI = replace_pbi
