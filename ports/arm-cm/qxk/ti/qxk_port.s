;*****************************************************************************
; Product: QXK port to ARM Cortex-M (M0,M0+,M1,M3,M4,M7), TI-ARM assembler
; Last Updated for Version: 5.6.0
; Date of the Last Update:  2015-12-12
;
;                    Q u a n t u m     L e a P s
;                    ---------------------------
;                    innovating embedded systems
;
; Copyright (C) Quantum Leaps, LLC. All rights reserved.
;
; This program is open source software: you can redistribute it and/or
; modify it under the terms of the GNU General Public License as published
; by the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; Alternatively, this program may be distributed and modified under the
; terms of Quantum Leaps commercial licenses, which expressly supersede
; the GNU General Public License and are specifically designed for
; licensees interested in retaining the proprietary status of their code.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program. If not, see <http://www.gnu.org/licenses/>.
;
; Contact information:
; http://www.state-machine.com
; mailto:info@state-machine.com
;*****************************************************************************

    .global QXK_start_        ; start the QXK multitasking
    .global QXK_stackInit_    ; initialize the stack of each thread
    .global PendSV_Handler    ; CMSIS-compliant PendSV exception

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    .global QF_set_BASEPRI    ; set BASEPRI register
  .endif                      ; M3/M4/M7
    .global QXK_ISR_ENTRY     ; inform QXK about interrupt entry
    .global QXK_ISR_EXIT      ; inform QXK about interrupt exit
    .global assert_failed     ; low-level assert handler

    .ref    QXK_attr_         ; QXK attribute structure
    .ref    QXK_threadRet_    ; return from a thread function
    .ref    QXK_sched_        ; external reference
    .ref    Q_onAssert        ; external reference

    ; NOTE: keep in synch with QF_BASEPRI value defined in "qf_port.h" !!!
QF_BASEPRI        .equ (0xFF >> 2)

    ; NOTE: keep in synch with the QXK struct in "qxk.h" !!!
QXK_CURR          .equ 0
QXK_NEXT          .equ 4
QXK_INT_NEST      .equ 8

    ; NOTE: keep in synch with the QMActive struct in "qf.h/qxk.h" !!!
QMACTIVE_OSOBJECT .equ 40
QMACTIVE_THREAD   .equ 44

    .text
    .thumb
;*****************************************************************************
; The QXK_start_ function starts QXK multitasking.
; The C signature: void QXK_start_(void);
;
; NOTE: QXK_start_() must be called with interrupts disabled and
; returns with interrupts **enabled**.
;*****************************************************************************
QXK_start_:
    LDR     r1,SHPR_addr      ; System Handler Priority Register
    LDR     r2,[r1,#8]        ; load the System 12-15 Priority Register
    MOVS    r3,#0xFF
    LSLS    r3,r3,#16
    ORRS    r2,r3             ; set PRI_14 (PendSV) to 0xFF
    STR     r2,[r1,#8]        ; write the System 12-15 Priority Register

    ; set the MSP to the top of the C-STACK, which is the first entry
    ; in the vector table. (This recovers any stack used so far by main().)
    LDR     r0,VTOR_addr      ; r0 := address of Vector Table Offset register
    LDR     r0,[r0,#0]        ; r0 := contents of VTOR
    LDR     r0,[r0]           ; r0 := VT[0] (first entry is the top of stack)
    MSR     MSP,r0            ; main SP := initial top of stack
    ISB                       ; flush the instruction pipeline

    ; set the current QXK thread to the next QXK thread
    LDR     r1,QXK_attr_addr
    LDR     r2,[r1,#QXK_NEXT] ; r2 := QXK_attr_.next
    STR     r2,[r1,#QXK_CURR] ; QXK_attr_.curr := r2

    ; get the top of stack of the current QXK thread
    LDR     r0,[r2,#QMACTIVE_THREAD] ; r0 := QXK_attr_.next->thread (SP)
    ; pop r4-r11 from the PSP
    MOVS    r1,r0             ; r1 := top of stack
    ADDS    r0,r0,#(4*4)      ; point r0 to the 4 high registers r7-r11
    LDMIA   r0!,{r4-r7}       ; pop the 4 high registers into low registers
    MOV     r8,r4             ; move low registers into high registers
    MOV     r9,r5
    MOV     r10,r6
    MOV     r11,r7
    LDMIA   r1!,{r4-r7}       ; pop the low registers
    ; NOTE: at this point r0 holds the new top of stack

    MSR     PSP,r0            ; set PSP to the thread's SP
    ISB                       ; flush the instruction pipeline

    ; switch CPU to using the Process Stack Pointer (PSP)
    MRS     r0,CONTROL
    MOVS    r1,#(1 << 1)
    ORRS    r0,r0,r1          ; set the Active Stack Pointer
    MSR     CONTROL,r0        ; switch to PSP
    DSB                       ; make sure all data access completes
    ISB                       ; flush the instruction pipeline

    ; fake return from an exception...
    POP     {r0-r3}           ; pop R0..R3
                              ; NOTE: R0 holds the 'par' argument of the
                              ; thread function
    POP     {r1,r2}           ; pop R12 and LR into low registers r1,r2
    MOV     r12,r1
    MOV     lr,r2
    POP     {r1,r2}           ; pop PC to R1 and xPSR to R2
                              ; NOTE: it's OK to clobber R1 and R2
  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__

  .if __TI_VFP_SUPPORT__      ; if VFP available...
    ; enable VFP by enabling CP10 and CP11 coprocessors
    LDR.W   r3,CPACR_addr     ; r3 := &CPACR
    LDR     r2,[r3]           ; r2 := CPACR
    ORR     r2,r2,#(0xF << 20); r2 := r2 | (CP10 | CP11)
    STR     r2,[r3]           ; CPACR = r2
    DSB                       ; make sure all data access completes
    ISB                       ; reset the instruction pipeline

    ; enable Automatic State Preservation and Lazy Stacking in the VFP
    LDR     r3,FPCCR_addr     ; r3 := &FPCCR
    LDR     r2,[r3]           ; r2 := FPCCR
    ORR     r2,r2,#(3 << 30)  ; set ASPEN | LSPEN
    STR     r2,[r3]           ; FPCCR &= ~(ASPEN | LSPEN)

    ; clear the VFP Context Active (FPCA) bit in CONTROL
    MRS     r2,CONTROL        ; r2 := CONTROL (NOTE: it's OK to clobber R2)
    BICS    r2,r2,#(1 << 2)   ; r2 := r2 & ~(1 << 2) (FPCA bit)
    MSR     CONTROL,r2        ; CONTROL := r2 (clear CONTROL[2] FPCA bit)
  .endif                      ; VFP available

    MOVS    r2,#0             ; NOTE: it's OK to clobber R2
    MSR     BASEPRI,r2        ; enable interrupts (clear BASEPRI)
  .else                       ; M0/M0+/M1
    CPSIE   i                 ; enable interrupts (clear PRIMASK)
  .endif                      ; M0/M0+/M1

    BX      r1                ; return to the "interrupted" thread (PC)
    .endasmfunc

;*****************************************************************************
; The PendSV_Handler exception handler is used for context switching in QXK.
; The use of the PendSV exception is the recommended and most efficient
; method for performing context switches with ARM Cortex-M.
;
; The PendSV exception should have the lowest priority in the whole system
; (0xFF, see QXK_start_). All other exceptions and interrupts should have
; higher priority. For example, for NVIC with 2 priority bits all interrupts
; and exceptions must have numerical value of priority lower than 0xC0.
; In this case the interrupt priority levels available to your applications
; are (in the order from the lowest to the highest urgency): 0x80, 0x40, 0x00.
;
; Also, *all* "kernel-aware" ISRs in the QXK application must call the
; QXK_ISR_EXIT() macro to trigger the PendSV exception.
;
; The C signature (CMSIS): void PendSV_Handler(void);
;
; NOTE:
; Due to tail-chaining and its lowest priority, the PendSV exception will be
; entered immediately after the exit from the *last* nested interrupt (or
; exception). In QXK, this is exactly the time when the QXK context switch
; must occur.
;*****************************************************************************
PendSV_Handler: .asmfunc

    MRS     r0,PSP            ; r0 := Process Stack Pointer
    ISB                       ; flush the instruction pipeline

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
     STMDB   r0!,{r4-r11}      ; save r4-r11 on top of the exception frame
  .if __TI_VFP_SUPPORT__      ; if VFP available...
    TST     lr,#(1 << 4)      ; is it return with the VFP exception frame?
    IT      EQ                ; if lr[4] is zero...
    VSTMDBEQ r0!,{s16-s31}    ; ... save VFP registers s16..s31
  .endif                      ; VFP available
    MOVS    r3,#QF_BASEPRI
    MSR     BASEPRI,r3        ; selectively disable interrupts
  .else                       ; M0/M0+/M1
    SUBS    r0,r0,#(8*4)      ; make room for 8 registers r4-r11
    MOVS    r1,r0             ; r1 := temporary PSP (do not clobber r0!)
    STMIA   r1!,{r4-r7}       ; save the low registers
    MOV     r4,r8             ; move the high registers to low registers...
    MOV     r5,r9
    MOV     r6,r10
    MOV     r7,r11
    STMIA   r1!,{r4-r7}       ; save the high registers
    ; NOTE: at this point r0 still holds the top of stack
    CPSID   i                 ; disable interrupts (set PRIMASK)
  .endif                      ; M0/M0+/M1
    ISB                       ; flush the instruction pipeline

    ; store the SP of the current QXK thread...
    LDR     r1,QXK_attr_addr
    LDR     r2,[r1,#QXK_CURR]
    STR     r0,[r2,#QMACTIVE_THREAD] ; QXK_attr_.curr->thread := r0

  .if __TI_VFP_SUPPORT__      ; if VFP available...
    ; store the LR of the current QXK thread...
    STR     lr,[r2,#QMACTIVE_OSOBJECT]  ; QXK_attr_.curr->osObject := lr
  .endif                      ; VFP available

    ; set current to the next...
    LDR     r2,[r1,#QXK_NEXT] ; r2 := QXK_attr_.next
    STR     r2,[r1,#QXK_CURR] ; QXK_attr_.curr := r2

    ; restore the SP of the next thread...
    LDR     r0,[r2,#QMACTIVE_THREAD] ; r0 := QXK_attr_.next->thread (SP)

  .if __TI_VFP_SUPPORT__      ; if VFP available...
    ; restore the LR of the next thread...
    LDR     lr,[r2,#QMACTIVE_OSOBJECT]   ; lr := QXK_attr_.next->osObject
  .endif                      ; VFP available

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    ; exit the critical section
    MOVS    r3,#0
    MSR     BASEPRI,r3        ; enable interrupts (clear BASEPRI)

  .if __TI_VFP_SUPPORT__      ; if VFP available...
    TST     lr,#(1 << 4)      ; is it return to the VFP exception frame?
    IT      EQ                ; if lr[4] is zero...
    VLDMIAEQ r0!,{s16-s31}    ; ... restore VFP registers s16..s31
  .endif                      ; VFP available

    LDMIA   r0!,{r4-r11}      ; restore r4-r11 from the next thread's stack

  .else                       ; M0/M0+/M1
    ; exit the critical section
    CPSIE   i                 ; enable interrupts (clear PRIMASK)

    MOVS    r1,r0             ; r1 := top of stack
    ADDS    r0,r0,#(4*4)      ; point r0 to the 4 high registers r7-r11
    LDMIA   r0!,{r4-r7}       ; pop the 4 high registers into low registers
    MOV     r8,r4             ; move low registers into high registers
    MOV     r9,r5
    MOV     r10,r6
    MOV     r11,r7
    LDMIA   r1!,{r4-r7}       ; pop the low registers
    ; NOTE: at this point r0 holds the new top of stack
  .endif                      ; M0/M0+/M1
    DSB                       ; make sure all data access completes

    ; set the PSP to the next thread's SP
    MSR     PSP,r0            ; Process Stack Pointer := r0
    ISB                       ; flush the instruction pipeline

    BX      lr                ; return to the next thread
    .endasmfunc


;*****************************************************************************
; Initialize the private stack of a QXK thread.
;
; NOTE: the function aligns the stack to the 8-byte boundary for
; compatibility with the AAPCS. Additionally, the function pre-fills
; the stack with the known bit pattern (0xDEADBEEF).
;
; The C signature:
; void QXK_stackInit_(void *act, QActionHandler thread,
;                     void *stkSto, uint_fast16_t stkSize);
;
; NOTE: QXK_stackInit_() must be called before the QF is made
; aware of this QXK thread. In that case there can be no external
; communication with this thread, so no critical section is needed.
;*****************************************************************************
QXK_stackInit_: .asmfunc
    ; assignment of parameters (AAPCS)
    ; r0 - QMActive pointer (act)
    ; r1 - thread routine
    ; r2 - begining of stack
    ; r3 - size of stack [bytes]

    ADDS    r3,r2,r3          ; r3 := end of stack (top of stack)
    MOV     r12,r0            ; save r0 in r12

    ; round up the beginning of stack to the 8-byte boundary
    ; r2 := (((r2 -1) >> 3) + 1) << 3;
    SUBS    r0,r2,#1
    LSRS    r0,r0,#3
    ADDS    r0,r0,#1
    LSLS    r2,r0,#3

    ; round down the end of stack to the 8-byte boundary
    ; r3 := (r3 >> 3) << 3;
    LSRS    r0,r3,#3
    LSLS    r3,r0,#3

    SUBS    r3,r3,#16*4       ; r3 := top of the 16-register stack frame

    MOV     r0,r12
    STR     r1,[r0,#QMACTIVE_OSOBJECT] ; temporarily save the thread routine

    ; pre-fill the stack with STACK_FILL_const
    LDR     r0,STACK_FILL_const
    MOV     r1,r0

QXK_stackInit_fill:
    STMIA   r2!,{r0,r1}
    CMP     r2,r3
    BLT.N   QXK_stackInit_fill

    ; prepare the standard exception (without VFP) stack frame...

    MOV     r0,r12            ; restore r0 from r12

    MOVS    r2,#0x04
    STR     r2,[r3,#0*4]      ; r4

    MOVS    r2,#0x05
    STR     r2,[r3,#1*4]      ; r5

    MOVS    r2,#0x06
    STR     r2,[r3,#2*4]      ; r6

    MOVS    r2,#0x07
    STR     r2,[r3,#3*4]      ; r7

    MOVS    r2,#0x08
    STR     r2,[r3,#4*4]      ; r8

    MOVS    r2,#0x09
    STR     r2,[r3,#5*4]      ; r9

    MOVS    r2,#0x0A
    STR     r2,[r3,#6*4]      ; r10

    MOVS    r2,#0x0B
    STR     r2,[r3,#7*4]      ; r11

    STR     r0,[r3,#8*4]      ; r0 (argument to thread routine, act pointer)

    MOVS    r2,#0x01
    STR     r2,[r3,#9*4]      ; r1

    MOVS    r2,#0x02
    STR     r2,[r3,#10*4]     ; r2

    MOVS    r2,#0x03
    STR     r2,[r3,#11*4]     ; r3

    MOVS    r2,#0x0C
    STR     r2,[r3,#12*4]     ; r12

    LDR     r2,QXK_threadRet_addr
    STR     r2,[r3,#13*4]     ; LR (return address)

    LDR     r1,[r0,#QMACTIVE_OSOBJECT] ; r1 := saved thread routine
    STR     r1,[r3,#14*4]     ; PC (entry point, thread routine)

    MOVS    r2,#1
    LSLS    r2,r2,#24         ; r2 := 0x01000000
    STR     r2,[r3,#15*4]     ; xPSR

    STR     r3,[r0,#QMACTIVE_THREAD] ; act->thread := top of stack

    MOVS    r2,#2
    MVNS    r2,r2             ; r2 := ~2 == 0xFFFFFFFD
    STR     r2,[r0,#QMACTIVE_OSOBJECT] ; act->thread.osObject := lr

    BX      lr                ; return to the caller
    .endasmfunc


;*****************************************************************************
; The QF_set_BASEPRI function sets the BASEPRI register to the value
; passed in r0.
; NOTE: The BASEPRI register is implemented only in ARMv7 architecture
; and is **not** available in ARMv6 (M0/M0+/M1)
;
; C prototype: void QF_set_BASEPRI(unsigned basePri);
;*****************************************************************************
  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
QF_set_BASEPRI: .asmfunc
    MSR     BASEPRI,r0        ; set BASEPRI
    BX      lr                ; return to the caller
    .endasmfunc
  .endif                      ; M3/M4/M7


;*****************************************************************************
; Inform QXK about interrupt entry.
; C prototype: void QXK_ISR_ENTRY(void);
; C pseudocode:
; void QXK_ISR_ENTRY(void) {
;     int_disable();
;     ++QXK_attr_.intNest;
;     int_enable();
; }
;*****************************************************************************
QXK_ISR_ENTRY: .asmfunc
  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MOVS    r0,#QF_BASEPRI
    MSR     BASEPRI,r0        ; selectively disable interrupts
  .else                       ; M0/M0+/M1
    CPSID   i                 ; disable interrupts (set PRIMASK)
  .endif                      ; M0/M0+/M1
    ISB                       ; flush the instruction pipeline

    ; ++QXK_attr_.intNest
    LDR     r1,QXK_attr_addr  ; address of the QXK_attr
    LDR     r0,[r1,#QXK_INT_NEST]
    ADDS    r0,r0,#1
    STR     r0,[r1,#QXK_INT_NEST]

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MOVS    r0,#0
    MSR     BASEPRI,r0        ; set BASEPRI (disable interrupts)
  .else                       ; M0/M0+/M1
    CPSIE   i                 ; enable interrupts (set PRIMASK)
  .endif                      ; M0/M0+/M1

    BX      lr                ; return to the caller
    .endasmfunc


;*****************************************************************************
; Inform QXK about interrupt exit.
; C prototype: void QXK_ISR_EXIT(void);
; C pseudocode:
; void QXK_ISR_EXIT(void) {
;     int_disable();
;     if (--QXK_attr_.intNest == 0) {
;        QXK_sched_();
;     }
;     int_enable();
; }
;*****************************************************************************
QXK_ISR_EXIT: .asmfunc

  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MOVS    r0,#QF_BASEPRI
    MSR     BASEPRI,r0        ; selectively disable interrupts
  .else                       ; M0/M0+/M1
    CPSID   i                 ; disable interrupts (set PRIMASK)
  .endif                      ; M0/M0+/M1
    ISB                       ; flush the instruction pipeline

    ; --QXK_attr_.intNest
    LDR     r1,QXK_attr_addr  ; address of the QXK_attr
    LDR     r0,[r1,#QXK_INT_NEST]
    SUBS    r0,r0,#1
    STR     r0,[r1,#QXK_INT_NEST]

    ; if QXK_attr_.intNest != 0, branch to QK_ISR_EXIT_ret
    CMP     r0,#0
    BNE     QXK_ISR_EXIT_ret

    PUSH    {r0,lr}           ; push lr (return address) plus stack "aligner"
    BL      QXK_sched_        ; call QXK scheduler
    POP     {r0,r1}           ; pop the stack "aligner" and the saved LR
    MOV     lr,r1

QXK_ISR_EXIT_ret:
  .if __TI_TMS470_V7M3__ | __TI_TMS470_V7M4__ ; | __TI_TMS470_V7M7__
    MOVS    r0,#0
    MSR     BASEPRI,r0        ; set BASEPRI (disable interrupts)
  .else                       ; M0/M0+/M1
    CPSIE   i                 ; enable interrupts (set PRIMASK)
  .endif                      ; M0/M0+/M1

    BX      lr                ; return to the caller
    .endasmfunc


;*****************************************************************************
; The assert_failed() function restores the SP (in case stack is corrupted)
; and calls Q_onAssert(module, loc)
; C prototype: void assert_failed(char const *module, int loc);
;*****************************************************************************
assert_failed: .asmfunc
    LDR     r0,VTOR_addr      ; r0 := address of Vector Table Offset register
    LDR     r0,[r0,#0]        ; r0 := contents of VTOR
    LDR     r0,[r0]           ; r0 := VT[0] (first entry is the top of stack)
    MSR     MSP,r0            ; main SP := initial top of stack
    ISB                       ; flush the instruction pipeline
    BL      Q_onAssert
    .endasmfunc

;*****************************************************************************
; Constants for PC-relative LDR
;*****************************************************************************
SHPR_addr:        .word 0xE000ED18
ICSR_addr:        .word 0xE000ED04
VTOR_addr:        .word 0xE000ED08
CPACR_addr:       .word 0xE000ED88
FPCCR_addr:       .word 0xE000EF34
STACK_FILL_const: .word 0xDEADBEEF


;*****************************************************************************
; Addresses for PC-relative LDR
;*****************************************************************************
QXK_attr_addr:    .word QXK_attr_
QXK_sched_addr:   .word QXK_sched_
QXK_threadRet_addr: .word QXK_threadRet_
    