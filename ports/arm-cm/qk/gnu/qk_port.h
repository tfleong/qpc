/**
* @file
* @brief QK/C port to ARM Cortex-M, GNU-ARM compiler
* @cond
******************************************************************************
* Last Updated for Version: 5.5.2
* Date of the Last Update:  2015-11-08
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. state-machine.com.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* http://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef qk_port_h
#define qk_port_h

/* determination if the code executes in the ISR context */
#define QK_ISR_CONTEXT_() (QK_get_IPSR() != (uint32_t)0)

__attribute__((always_inline))
static inline uint32_t QK_get_IPSR(void) {
    uint32_t regIPSR;
    __asm volatile ("mrs %0,ipsr" : "=r" (regIPSR));
    return regIPSR;
}


/* QK interrupt entry and exit */
#define QK_ISR_ENTRY() ((void)0)

#define QK_ISR_EXIT()  do { \
    uint_fast8_t nextPrio_; \
    QF_INT_DISABLE(); \
    nextPrio_ = QK_schedPrio_(); \
    if (nextPrio_ != (uint_fast8_t)0) { \
        QK_nextPrio_ = nextPrio_; \
        (*Q_UINT2PTR_CAST(uint32_t, 0xE000ED04U) = (uint32_t)(1U << 28)); \
    } \
    QF_INT_ENABLE(); \
} while (0)

extern uint_fast8_t QK_nextPrio_; /* priority of the next task to execute */

#include "qk.h" /* QK platform-independent public interface */

#endif /* qk_port_h */
