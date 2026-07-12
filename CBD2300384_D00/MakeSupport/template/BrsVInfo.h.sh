
################################################################################
#
#  This script generates the BrsVInfo.h file dependent on
#  MakeSupport variables
#
#  Required variables:
#  -------------------
#  - VERSION
#  - SUB_VERSION
#  - CC_VERSION_STRING
#  - CFLAGS
#  - AS_VERSION_STRING
#  - ASFLAGS
#  - LD_VERSION_STRING
#  - LDFLAGS
#  - AR_VERSION_STRING
#  - ARFLAGS
#  - EXT_T1                 (optional)
#  - EXT_T1_VERSION_STRING  (optional)
#  - EXT_T1_FLAGS           (optional)
#  - EXT_T2                 (optional)
#  - EXT_T2_VERSION_STRING  (optional)
#  - EXT_T2_FLAGS           (optional)
#  - COMPILER_MANUFACTURER
#  - DERIVATIVE
#
#  Usage in any Makefile:
#    VERSION=$(VERSION) SUB_VERSION=$(SUB_VERSION).... $(MAKESUPPORT_DIR_U)/BrsVInfo.h.sh $(GENDATA)/BrsVInfo.h
#

#######################################
#
#  Stop on every error
#
set -e

#######################################
#
#  Init section
#
outfile=${1}

#######################################
#
#  Check section
#
if [ $# != 1 ]; then
  echo "`basename $0` <outfile>"
fi

#######################################
#
#  MAIN section
#

#
#  CAT section
#
cat <<+++ > ${outfile}
/* This file is generated automatically by MakeSupport ${VERSION}.${SUB_VERSION} */
#ifndef __BRSVINFO_H
#define __BRSVINFO_H

#ifndef _BRS_V_INFO_VALID
  #error Include of BrsVInfo.h is only allowed in context of BrsCtrl.c
#endif

#ifndef _BRS_ENABLE_FLAG_ACCESS_
  #define _BRS_ENABLE_FLAG_ACCESS_
#endif

#if defined ( C_COMP_IAR_M16C )
  #if defined ( _NEAR_ )
  /* Set const data to const segment, don\'t initialize */
  #pragma memory=constseg(CONST_DATA) :far
  #endif
#endif

#define BRS_ENABLE_COMPILER_VERSION
#define BRS_ENABLE_ASSEMBLER_VERSION
#define BRS_ENABLE_LINKER_VERSION
#define BRS_ENABLE_LIBRARIAN_VERSION

#if defined ( BRS_ENABLE_TOOL_VERSIONINFO )

V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsCompilerVersion[] = "${CC_VERSION_STRING}";
#if defined ( BRS_ENABLE_FLAG_ACCESS )
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsCompilerAdditional[] = "${CFLAGS}";
#endif /* defined ( BRS_ENABLE_FLAG_ACCESS ) */

V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsAssemblerVersion[] = "${AS_VERSION_STRING}";
#if defined ( BRS_ENABLE_FLAG_ACCESS )
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsAssemblerAdditional[] = "${ASFLAGS}";
#endif /* defined ( BRS_ENABLE_FLAG_ACCESS ) */

V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsLinkerVersion[] = "${LD_VERSION_STRING}";
#if defined ( BRS_ENABLE_FLAG_ACCESS )
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsLinkerAdditional[] = "${LDFLAGS}";
#endif /* defined ( BRS_ENABLE_FLAG_ACCESS ) */

V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsLibrarianVersion[] = "${AR_VERSION_STRING}";
#if defined ( BRS_ENABLE_FLAG_ACCESS )
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsLibrarianParameter[] = "${ARFLAGS}";
#endif /* defined ( BRS_ENABLE_FLAG_ACCESS ) */
+++

#
#  EXT_T1 section
#
if [ -n "${EXT_T1}" ]; then
  echo "    retrieving External Tool 1 Version"
  cat << +++ >> ${outfile}

V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsExtTool1Version[] ="${EXT_T1_VERSION_STRING}";

#if defined ( BRS_ENABLE_FLAG_ACCESS )
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsExtTool1Parameter[] ="${EXT_T1_FLAGS}";
#endif /* defined ( BRS_ENABLE_FLAG_ACCESS ) */

+++
fi

#
#  EXT_T2 section
#
if [ -n "${EXT_T2}" ]; then
  echo "    retrieving External Tool 2 Version"
cat <<+++ >> ${outfile}

V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsExtTool2Version[] ="${EXT_T2_VERSION_STRING}";
#if defined ( BRS_ENABLE_FLAG_ACCESS )
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsExtTool2Parameter[] ="${EXT_T2_FLAGS}";
#endif /* defined ( BRS_ENABLE_FLAG_ACCESS ) */

+++
fi

#
#  CAT section
#
cat <<+++ >> ${outfile}

#endif /* defined ( BRS_ENABLE_TOOL_VERSIONINFO ) */

V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsCompilerDef[]   = "${COMPILER_MANUFACTURER}";
V_MEMROM0 V_MEMROM1 vsint8 V_MEMROM2 kBrsControllerDef[] = "${DERIVATIVE}";

#if defined ( C_COMP_IAR_M16C )
  #if defined ( _NEAR_ )
  /* Normal segment usage */
  #pragma memory=default
  #endif
#endif

#endif /* __BRSVINFO_H */
+++
