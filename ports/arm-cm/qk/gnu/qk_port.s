/*****************************************************************************
* Product: QK port to ARM Cortex-M (M0,M0+,M1,M3,M4,M7), GNU-ARM assembler
* Last Updated for Version: 5.6.0
* Date of the Last Update:  2015-12-14
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
*****************************************************************************/

    .syntax unified

    /* NOTE: keep in synch with QF_BASEPRI value defined in "qf_port.h" !!! */
    .equ QF_BASEPRI,(0xFF >> 2)


    .section .data
/*****************************************************************************
* Global priority of the next task to execute or zero to indicate return
* to the preempted task
*****************************************************************************/
QK_nextPrio_:
    .global QK_nextPrio_
    .word   0


/*****************************************************************************
* The QK_init function sets the priorities of PendSV to 0xFF (lowest).
* The priority is set within a critical section.
*****************************************************************************/
    .section .text.QK_init
    .global QK_init
    .type   QK_init, %function
    .thumb

QK_init:
    MRS     r0,PRIMASK        /* store the state of the PRIMASK in r0 */
    CPSID   i                 /* disable interrupts (set PRIMASK) */

    LDR     r1,=0xE000ED18    /* System Handler Priority Register */
    LDR     r2,[r1,#8]        /* load the System 12-15 Priority Register */
    MOVS    r3,#0xFF
    LSLS    r3,r3,#16
    ORRS    r2,r3             /* set PRI_14 (PendSV) to 0xFF */
    STR     r2,[r1,#8]        /* write the System 12-15 Priority Register */

    MSR     PRIMASK,r0        /* restore the original PRIMASK */
    BX      lr                /* return to the caller */
    .size   QK_init, . - QK_init


/*****************************************************************************
* The PendSV_Handler exception handler is used for handling the asynchronous
* preemption in QK. The use of the PendSV exception is the recommended
* and most efficient method for performing context switches with ARM Cortex-M.
*
* The PendSV exception should have the lowest priority in the whole system
* (0xFF, see QK_init). All other exceptions and interrupts should have higher
* priority. For example, for NVIC with 2 priority bits all interrupts and
* exceptions must have numerical value of priority lower than 0xC0. In this
* case the interrupt priority levels available to your applications are (in
* the order from the lowest urgency to the highest urgency): 0x80, 0x40, 0x00.
*
* NOTE: All ISRs in the QK application that post events must trigger the
* PendSV exception by calling the QK_ISR_EXIT() macro.
*
* Due to tail-chaining and its lowest priority, the PendSV exception will be
* entered immediately after the exit from the *last* nested interrupt (or
* exception). In QK, this is exactly the time when the QK scheduler needs to
* check for the asynchronous preemption.
*****************************************************************************/
    .section .text.PendSV_Handler
    .global PendSV_Handler    /* CMSIS-compliant exception name */
    .type   PendSV_Handler, %function
    .type   PendSV_sched_ret, %function /* to ensure the label is THUMB */

PendSV_Handler:

  .if  __ARM_ARCH == 6        /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    CPSID   i                 /* disable interrupts at processor level */
  .else                       /* M3/M4/M7 */

    MOVS    r0,#QF_BASEPRI
    MSR     BASEPRI,r0        /* selectively disable interrupts */
  .endif                      /* M3/M4/M7 */
    ISB                       /* reset the instruction pipeline */

    LDR     r0,=QK_nextPrio_
    LDR     r0,[r0]
    CMP     r0,#0
    BNE.N   PendSV_sched      /* if QK_nextPrio_ != 0, branch to scheduler */

    /* QK_nextPrio_ == 0: return to the preempted task... */
    ADD     sp,sp,#(8*4)      /* remove one 8-register exception frame */

  .if  __ARM_ARCH == 6        /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    CPSIE   i                 /* enable interrupts (clear PRIMASK) */
    MOVS    r0,#6
    MVNS    r0,r0             /* r0 := ~6 == 0xFFFFFFF9 */
    BX      r0                /* exception-return to the task */
  .else                       /* M3/M4/M7 */
    /* NOTE: r0 == 0 at this point */
    MSR     BASEPRI,r0        /* enable interrupts (clear BASEPRI) */
  .ifdef  __FPU_PRESENT       /* if VFP available... */
    POP     {r0,pc}           /* pop stack "aligner" and EXC_RETURN to PC */
  .else                       /* no VFP */
    MOVS    r0,#6
    MVNS    r0,r0             /* r0 := ~6 == 0xFFFFFFF9 */
    BX      r0                /* exception-return to the task */
  .endif                      /* VFP available */
  .endif                      /* M3/M4/M7 */

PendSV_sched:
  .ifdef  __FPU_PRESENT       /* if VFP available... */
    PUSH    {r0,lr}           /* push lr (EXC_RETURN) plus stack "aligner" */
  .endif                      /* VFP available */
    MOVS    r3,#1
    LSLS    r3,r3,#24         /* r3:=(1 << 24), set the T bit  (new xpsr) */
    LDR     r2,=QK_sched_     /* address of the QK scheduler   (new pc) */
    LDR     r1,=PendSV_sched_ret /* return address after the call (new lr) */

    SUB     sp,sp,#8*4        /* reserve space for exception stack frame */
    STR     r0,[sp]           /* save the prio argument        (new r0) */
    ADD     r0,sp,#5*4        /* r0 := 5 registers below the top of stack */
    STM     r0!,{r1-r3}       /* save xpsr,pc,lr */

    MOVS    r0,#6
    MVNS    r0,r0             /* r0 := ~6 == 0xFFFFFFF9 */
    BX      r0                /* exception-return to the QK scheduler */

PendSV_sched_ret:
    LDR     r0,=QK_nextPrio_
    MOVS    r1,#0
    STR     r1,[r0]           /* QK_nextPrio_ = 0; */

  .if  __ARM_ARCH == 6        /* Cortex-M0/M0+/M1 (v6-M, v6S-M)? */
    CPSIE   i                 /* enable interrupts (clear PRIMASK) */
  .else                       /* M3/M4/M7 */
  .ifdef  __FPU_PRESENT       /* if VFP available... */
    MRS     r0,CONTROL        /* r0 := CONTROL */
    BICS    r0,r0,#4          /* r0 := r0 & ~4 (FPCA bit) */
    MSR     CONTROL,r0        /* CONTROL := r0 (clear CONTROL[2] FPCA bit) */
  .endif                      /* VFP available */
    MOVS    r0,#0
    MSR     BASEPRI,r0        /* enable interrupts (clear BASEPRI) */
  .endif                      /* M3/M4/M7 */

    /* trigger PendSV to return to preempted task... */
    LDR     r0,=0xE000ED04    /* Interrupt Control and State Register */
    MOVS    r1,#1
    LSLS    r1,r1,#28         /* r0 := (1 << 28) (PENDSVSET bit) */
    STR     r1,[r0]           /* ICSR[28] := 1 (pend PendSV) */
    B       .                 /* wait for preemption by PendSV */
    .size   PendSV_Handler, . - PendSV_Handler

    .end
