/*****************************************************************************
* Model: calc_stm32f469i-disco.qm
* File:  ./touchscreen.c
*
* This code has been generated by QM tool (see state-machine.com/qm).
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*****************************************************************************/
/*${.::touchscreen.c} ......................................................*/
#include "calc.h"

TS_StateTypeDef TS_State = {0};
CalcEvt *ce;

/*${AOs::Touchscreen} ......................................................*/
typedef struct {
/* protected: */
    QActive super;
} Touchscreen;

/* protected: */
static QState Touchscreen_initial(Touchscreen * const me, QEvt const * const e);
static QState Touchscreen_idle(Touchscreen * const me, QEvt const * const e);
static QState Touchscreen_process(Touchscreen * const me, QEvt const * const e);


static Touchscreen l_touchscreen; /* the only instance of the Touchscreen class */

/* global-scope definitions ---------------------------------------*/
QActive *const AO_Touchscreen = &l_touchscreen.super;    /* the opaque pointer */

/*${AOs::Touchscreen_ctor} .................................................*/
void Touchscreen_ctor(void) {
    Touchscreen *me = &l_touchscreen;
    QActive_ctor(&me->super, Q_STATE_CAST(&Touchscreen_initial));
}
/*${AOs::Touchscreen} ......................................................*/
/*${AOs::Touchscreen::SM} ..................................................*/
static QState Touchscreen_initial(Touchscreen * const me, QEvt const * const e) {
    /* ${AOs::Touchscreen::SM::initial} */
    (void)e;
    return Q_TRAN(&Touchscreen_idle);
}
/*${AOs::Touchscreen::SM::idle} ............................................*/
static QState Touchscreen_idle(Touchscreen * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${AOs::Touchscreen::SM::idle::TOUCH} */
        case TOUCH_SIG: {
            /* ${AOs::Touchscreen::SM::idle::TOUCH::[TS_State.touchDetected==1]} */
            if (TS_State.touchDetected == 1) {
                BSP_TS_GetState(&TS_State);
                status_ = Q_HANDLED();
            }
            /* ${AOs::Touchscreen::SM::idle::TOUCH::[else]} */
            else {
                BSP_TS_GetState(&TS_State);
                /* ${AOs::Touchscreen::SM::idle::TOUCH::[else]::[TS_State.touchDetected==1]} */
                if (TS_State.touchDetected == 1) {
                    status_ = Q_TRAN(&Touchscreen_process);
                }
                /* ${AOs::Touchscreen::SM::idle::TOUCH::[else]::[else]} */
                else {
                    status_ = Q_HANDLED();
                }
            }
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*${AOs::Touchscreen::SM::idle::process} ...................................*/
static QState Touchscreen_process(Touchscreen * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${AOs::Touchscreen::SM::idle::process} */
        case Q_ENTRY_SIG: {
            if((TS_State.touchX[0] > KEY_C_X_MIN) && (TS_State.touchX[0] < KEY_C_X_MAX) &&
            (TS_State.touchY[0] > KEY_C_Y_MIN) && (TS_State.touchY[0] < KEY_C_Y_MAX)){
                ce = Q_NEW(CalcEvt, C_SIG);
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_CE_X_MIN) && (TS_State.touchX[0] < KEY_CE_X_MAX) &&
            (TS_State.touchY[0] > KEY_CE_Y_MIN) && (TS_State.touchY[0] < KEY_CE_Y_MAX)){
                ce = Q_NEW(CalcEvt, CE_SIG);
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_0_X_MIN) && (TS_State.touchX[0] < KEY_0_X_MAX) &&
            (TS_State.touchY[0] > KEY_0_Y_MIN) && (TS_State.touchY[0] < KEY_0_Y_MAX)){
                ce = Q_NEW(CalcEvt, DIGIT_0_SIG);
                ce->key_code = '0';
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_1_X_MIN) && (TS_State.touchX[0] < KEY_1_X_MAX) &&
            (TS_State.touchY[0] > KEY_1_Y_MIN) && (TS_State.touchY[0] < KEY_1_Y_MAX)){
                ce = Q_NEW(CalcEvt, DIGIT_1_9_SIG);
                ce->key_code = '1';
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_2_X_MIN) && (TS_State.touchX[0] < KEY_2_X_MAX) &&
            (TS_State.touchY[0] > KEY_2_Y_MIN) && (TS_State.touchY[0] < KEY_2_Y_MAX)){
                ce = Q_NEW(CalcEvt, DIGIT_1_9_SIG);
                ce->key_code = '2';
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_3_X_MIN) && (TS_State.touchX[0] < KEY_3_X_MAX) &&
            (TS_State.touchY[0] > KEY_3_Y_MIN) && (TS_State.touchY[0] < KEY_3_Y_MAX)){
                ce = Q_NEW(CalcEvt, DIGIT_1_9_SIG);
                ce->key_code = '3';
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_4_X_MIN) && (TS_State.touchX[0] < KEY_4_X_MAX) &&
            (TS_State.touchY[0] > KEY_4_Y_MIN) && (TS_State.touchY[0] < KEY_4_Y_MAX)){
                ce = Q_NEW(CalcEvt, DIGIT_1_9_SIG);
                ce->key_code = '4';
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_5_X_MIN) && (TS_State.touchX[0] < KEY_5_X_MAX) &&
            (TS_State.touchY[0] > KEY_5_Y_MIN) && (TS_State.touchY[0] < KEY_5_Y_MAX)){
                ce = Q_NEW(CalcEvt, DIGIT_1_9_SIG);
                ce->key_code = '5';
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_6_X_MIN) && (TS_State.touchX[0] < KEY_6_X_MAX) &&
            (TS_State.touchY[0] > KEY_6_Y_MIN) && (TS_State.touchY[0] < KEY_6_Y_MAX)){
                ce = Q_NEW(CalcEvt, DIGIT_1_9_SIG);
                ce->key_code = '6';
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_7_X_MIN) && (TS_State.touchX[0] < KEY_7_X_MAX) &&
            (TS_State.touchY[0] > KEY_7_Y_MIN) && (TS_State.touchY[0] < KEY_7_Y_MAX)){
                ce = Q_NEW(CalcEvt, DIGIT_1_9_SIG);
                ce->key_code = '7';
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_8_X_MIN) && (TS_State.touchX[0] < KEY_8_X_MAX) &&
            (TS_State.touchY[0] > KEY_8_Y_MIN) && (TS_State.touchY[0] < KEY_8_Y_MAX)){
                ce = Q_NEW(CalcEvt, DIGIT_1_9_SIG);
                ce->key_code = '8';
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_9_X_MIN) && (TS_State.touchX[0] < KEY_9_X_MAX) &&
            (TS_State.touchY[0] > KEY_9_Y_MIN) && (TS_State.touchY[0] < KEY_9_Y_MAX)){
                ce = Q_NEW(CalcEvt, DIGIT_1_9_SIG);
                ce->key_code = '9';
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_POINT_X_MIN) && (TS_State.touchX[0] < KEY_POINT_X_MAX) &&
            (TS_State.touchY[0] > KEY_POINT_Y_MIN) && (TS_State.touchY[0] < KEY_POINT_Y_MAX)){
                ce = Q_NEW(CalcEvt, POINT_SIG);
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_ADD_X_MIN) && (TS_State.touchX[0] < KEY_ADD_X_MAX) &&
            (TS_State.touchY[0] > KEY_ADD_Y_MIN) && (TS_State.touchY[0] < KEY_ADD_Y_MAX)){
                ce = Q_NEW(CalcEvt, OPER_SIG);
                ce->key_code = '+';
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_SUB_X_MIN) && (TS_State.touchX[0] < KEY_SUB_X_MAX) &&
            (TS_State.touchY[0] > KEY_SUB_Y_MIN) && (TS_State.touchY[0] < KEY_SUB_Y_MAX)){
                ce = Q_NEW(CalcEvt, OPER_SIG);
                ce->key_code = '-';
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_MUL_X_MIN) && (TS_State.touchX[0] < KEY_MUL_X_MAX) &&
            (TS_State.touchY[0] > KEY_MUL_Y_MIN) && (TS_State.touchY[0] < KEY_MUL_Y_MAX)){
                ce = Q_NEW(CalcEvt, OPER_SIG);
                ce->key_code = '*';
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_DIV_X_MIN) && (TS_State.touchX[0] < KEY_DIV_X_MAX) &&
            (TS_State.touchY[0] > KEY_DIV_Y_MIN) && (TS_State.touchY[0] < KEY_DIV_Y_MAX)){
                ce = Q_NEW(CalcEvt, OPER_SIG);
                ce->key_code = '/';
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            else if((TS_State.touchX[0] > KEY_EQUAL_X_MIN) && (TS_State.touchX[0] < KEY_EQUAL_X_MAX) &&
            (TS_State.touchY[0] > KEY_EQUAL_Y_MIN) && (TS_State.touchY[0] < KEY_EQUAL_Y_MAX)){
                ce = Q_NEW(CalcEvt, EQUALS_SIG);
                QACTIVE_POST(AO_Calc, &ce->super, me);
            }
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&Touchscreen_idle);
            break;
        }
    }
    return status_;
}

