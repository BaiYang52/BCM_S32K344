/* Copyright 2020-2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 ****************************************************************************/

        EXTERN  .data_init$$Base
        EXTERN  DATA$$Base
        EXTERN  DATA$$Length
        EXTERN  .bss$$Base
        EXTERN  .bss$$Length
        EXTERN  CSTACK$$Base
        EXTERN  CSTACK$$Length
        EXTERN  HEAP$$Base
        EXTERN  HEAP$$Length
#if defined (__ITCM_PRESENT) && (__ITCM_PRESENT == 1U) && (__ITCM_INIT)
        EXTERN  ITCM$$Base
        EXTERN  ITCM$$Length
#endif
#if defined (__DTCM_PRESENT) && (__DTCM_PRESENT == 1U) && (__DTCM_INIT)
        EXTERN  DTCM$$Base
        EXTERN  DTCM$$Length
#endif
        PUBLIC  __ecc_table_start__
        PUBLIC  __ecc_table_end__
        PUBLIC  __copy_table_start__
        PUBLIC  __copy_table_end__
        PUBLIC  __zero_table_start__
        PUBLIC  __zero_table_end__

;; ECC table
        SECTION  .ecc_tables:CONST(4)

__ecc_table_start__
#ifdef START_FROM_FLASH
        DCD DATA$$Base
        DCD 0
        DCD DATA$$Length
        DCD 0
#endif
        DCD .bss$$Base
        DCD 0
        DCD .bss$$Length
        DCD 0
        DCD CSTACK$$Base
        DCD 0
        DCD CSTACK$$Length
        DCD 0
        DCD HEAP$$Base
        DCD 0
        DCD HEAP$$Length
        DCD 0
#if defined (__ITCM_PRESENT) && (__ITCM_PRESENT == 1U) && (__ITCM_INIT)
        DCD ITCM$$Base
        DCD 0
        DCD ITCM$$Length
        DCD 0
#endif
#if defined (__DTCM_PRESENT) && (__DTCM_PRESENT == 1U) && (__DTCM_INIT)
        DCD DTCM$$Base
        DCD 0
        DCD DTCM$$Length
        DCD 0
#endif
__ecc_table_end__

;; Zero table (initialized in IAR startup)
        SECTION .zero_tables:CONST(4)
__zero_table_start__
        ;DCD .bss$$Base
        ;DCD .bss$$Length
__zero_table_end__

;; Copy table  (initialized in IAR startup)
        SECTION .copy_tables:CONST(4)
__copy_table_start__
        ;DCD .data_init$$Base
        ;DCD DATA$$Base
        ;DCD DATA$$Length
__copy_table_end__

        END
