/*****************************************************************************
* Model: blinky_stm32f469i-disco.qm
* File:  ./blinky.c
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
/*${.::blinky.c} ...........................................................*/
#include "blinky.h"

/*${AOs::Blinky} ...........................................................*/
typedef struct {
/* protected: */
    QActive super;

/* private: */
    QTimeEvt timeEvt;
} Blinky;

/* protected: */
static QState Blinky_initial(Blinky * const me, QEvt const * const e);
static QState Blinky_off(Blinky * const me, QEvt const * const e);
static QState Blinky_on(Blinky * const me, QEvt const * const e);


static Blinky l_blinky;

QActive *const AO_Blinky = &l_blinky.super;

/*${AOs::Blinky_ctor} ......................................................*/
void Blinky_ctor(void) {
    Blinky *me = &l_blinky;
    QActive_ctor(&me->super, Q_STATE_CAST(&Blinky_initial));
    QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0u);
}
/*${AOs::Blinky} ...........................................................*/
/*${AOs::Blinky::SM} .......................................................*/
static QState Blinky_initial(Blinky * const me, QEvt const * const e) {
    /* ${AOs::Blinky::SM::initial} */
    (void)e;
    QTimeEvt_armX(&me->timeEvt, BSP_TICKS_PER_SEC, BSP_TICKS_PER_SEC);
    return Q_TRAN(&Blinky_off);
}
/*${AOs::Blinky::SM::off} ..................................................*/
static QState Blinky_off(Blinky * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${AOs::Blinky::SM::off} */
        case Q_ENTRY_SIG: {
            BSP_LED_Off(LED3);
            status_ = Q_HANDLED();
            break;
        }
        /* ${AOs::Blinky::SM::off::TIMEOUT} */
        case TIMEOUT_SIG: {
            status_ = Q_TRAN(&Blinky_on);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*${AOs::Blinky::SM::on} ...................................................*/
static QState Blinky_on(Blinky * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${AOs::Blinky::SM::on} */
        case Q_ENTRY_SIG: {
            BSP_LED_On(LED3);
            status_ = Q_HANDLED();
            break;
        }
        /* ${AOs::Blinky::SM::on::TIMEOUT} */
        case TIMEOUT_SIG: {
            status_ = Q_TRAN(&Blinky_off);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

