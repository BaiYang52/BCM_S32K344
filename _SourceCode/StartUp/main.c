/* Copyright 2023 NXP */
/* License: BSD 3-clause
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the
       names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
*/
/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#include "S32K344.h"

#if defined (__ghs__)
    #define __INTERRUPT_SVC  __interrupt
    #define __ASM_VOLATILE __asm volatile
    #define __NO_RETURN__ _Pragma("ghs nowarning 111")
#elif defined (__ICCARM__)
    #define __INTERRUPT_SVC  __svc
    #define __ASM_VOLATILE asm volatile
    #define __NO_RETURN__ _Pragma("diag_suppress=Pe111")
#elif defined ( __DCC__ )
  #define __INTERRUPT_SVC __attribute__ ((interrupt ("SWI")))
  #define __ASM_VOLATILE __asm volatile
  #define __NO_RETURN__
#elif defined (__GNUC__)
    #define __INTERRUPT_SVC  __attribute__ ((interrupt ("SVC")))
    #define __ASM_VOLATILE __asm volatile
    #define __NO_RETURN__
#else
    #define __INTERRUPT_SVC
    #define __NO_RETURN__
    #define __ASM_VOLATILE __asm volatile
#endif

int counter, accumulator = 0, limit_value = 1000000;

int main(void) {
    counter = 0;

    for (;;) {
        counter++;

        if (counter >= limit_value) {
            __ASM_VOLATILE ("svc 0");
            counter = 0;
        }
    }
    /* to avoid the warning message for GHS and IAR: statement is unreachable*/
    __NO_RETURN__
    return 0;
}

__INTERRUPT_SVC void SVC_Handler() {
    accumulator += counter;
}
