########################################################################################################################
# File Name  : Makefile_tools.mk
# Description: This Sub-Makefile contains commands to be used in Makefiles.
#              Please set options here only when tool changes the mode like
#              'gcc -E' which could be equal to 'cpp#
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
# 04.00.01  2018-10-10  vircvo  Modified ASMPP
# 04.00.02  2018-10-15  vircbl  Modified header, added BETA disclaimer
# 04.01.20  2019-06-03  vircbl  Updated file header and added doxygen documentation
# 04.01.27  2020-01-07  vircbl  Modify: Changed default value of DEPEND from 'clang' to 'cpp'
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
#  Makefile_tools.mk
#  =================
#
#  This Sub-Makefile contains default variable assignments for all tools/binaries needed by MakeSupport. It is included
#  by Global.Makefile.target.\$(VERSION).mk.
#
#  @ingroup mod_Makefile
#


#
#  clang has missing documentation for options -x <language>
#   found at http://clang-developers.42468.n3.nabble.com/detail-the-language-option-td4035293.html
#
#  possible values are:
#  - c
#  - c-header
#  - cpp-output
#  - c++
#  - c++-cpp-output
#  - objective-c
#  - objc-cpp-output
#  - assembler
#  - assembler-with-cpp
#  - ada
#  - f77
#  - f77-cpp-input
#  - ratfor
#  - java
#  - treelang
#
#
ASMPP?=clang -E -x assembler-with-cpp -Wno-unused-command-line-argument
#ASMPP?=cpp -P
AWK=awk
CAT=cat
CP?=cp
CPP=cpp
DEPEND?=cpp
DEPEND_ASM?=cpp
DOXYGEN?=doxygen
ELFCONVERTER_BIN?=$(ROOT_U)/Misc/OsElfConverter/ElfConverter.exe
ECHO?=echo
ECHOESC?=echo -e
FLOCK=flock
GAWK=awk
GREP=grep
LD?=$(CC)
LS=ls
MKDIR=mkdir -p
MV=mv
OBJDUMP=objdump
PRINTF=printf
READELF=readelf
RM=rm -f
RMDIR=rm  -rfd    #  because of possible behavior of unset variables set -d to avoid delete from '/'
SED?=sed
SIZE=size
SORT=sort
TOUCH=touch
TR?=tr
XSLT?=xsltproc
#XSLT?=MSXML
XML?=xmlstarlet
