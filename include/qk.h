/**
* @file
* @brief QK/C (preemptive non-blocking kernel) platform-independent
* public interface.
* @ingroup qk
* @cond
******************************************************************************
* Last updated for version 5.6.0
* Last updated on  2014-12-14
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
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
* Web:   www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* @endcond
*/
#ifndef qk_h
#define qk_h

#include "qequeue.h"  /* QK kernel uses the native QF event queue  */
#include "qmpool.h"   /* QK kernel uses the native QF memory pool  */
#include "qpset.h"    /* QK kernel uses the native QF priority set */

/****************************************************************************/
/* QF configuration for QK */

/*! This macro defines the type of the event queue used for the
* active objects. */
/**
* @note This is just an example of the macro definition. Typically, you need
* to define it in the specific QF port file (qf_port.h). In case of QK, which
* always depends on the native QF queue, this macro is defined at the level
* of the platform-independent interface qk.h.
*/
#define QF_EQUEUE_TYPE         QEQueue

/*! OS-dependent per-thread operating-system object */
/**
* @description
* The use of this member depends on the CPU. For example, in port to
* ARM Cortex-M with FPU this member is used to store the LR.
*/
#define QF_OS_OBJECT_TYPE      void*

/*! OS-dependent representation of the private thread */
/**
* @description
* QK uses this member to store the start priority of the AO,
* which is needed when the QK priority-ceiling mutex is used.
*/
#define QF_THREAD_TYPE         uint_fast8_t

#if (QF_MAX_ACTIVE <= 8)
    extern QPSet8  QK_readySet_; /*!< QK ready-set of AOs */
#else
    extern QPSet64 QK_readySet_; /*!< QK ready-set of AOs */
#endif

/****************************************************************************/
/*! QK scheduler */
void QK_sched_(uint_fast8_t p);

/*! Find the highest-priority task ready to run */
uint_fast8_t QK_schedPrio_(void);

/* public-scope objects */
extern uint_fast8_t volatile QK_currPrio_; /*!< current task priority */

#ifndef QK_ISR_CONTEXT_
    extern uint_fast8_t volatile QK_intNest_;  /*!< ISR nesting level */
#endif /* QK_ISR_CONTEXT_ */

/****************************************************************************/
/*! QK initialization */
/**
* QK_init() is called from QF_init() in qk.c. This function is
* defined in the QK ports.
*/
void QK_init(void);

/*! QK idle callback (customized in BSPs for QK) */
/**
* QK_onIdle() is called continuously by the QK idle loop. This callback
* gives the application an opportunity to enter a power-saving CPU mode,
* or perform some other idle processing.
*
* @note QK_onIdle() is invoked with interrupts enabled and must also
* return with interrupts enabled.
*
* @sa QV_onIdle()
*/
void QK_onIdle(void);


/****************************************************************************/
/*! Priority Ceiling Mutex the QK preemptive kernel */
typedef struct {
    uint8_t prioCeiling;
    uint8_t lockNest;
} QMutex;

/*! initialize the QK priority-ceiling mutex */
void QMutex_init(QMutex * const me, uint_fast8_t prioCeiling);

/*! lock the QK priority-ceiling mutex */
void QMutex_lock(QMutex * const me);

/*! unlock the QK priority-ceiling mutex */
void QMutex_unlock(QMutex * const me);


/****************************************************************************/
/*! get the current QK version number string of the form "X.Y.Z" */
#define QK_getVersion() (QP_versionStr)


/****************************************************************************/
/* interface used only inside QP implementation, but not in applications */
#ifdef QP_IMPL

    #if (QF_MAX_ACTIVE <= 8)
        #define QK_prioNotEmpty(set_)    QPSet8_notEmpty((set_))
        #define QK_prioIsSet(set_, p_)   QPSet8_hasElement((set_), (p_))
        #define QK_prioFindMax(set_, p_) QPSet8_findMax((set_), (p_))
        #define QK_prioInsert(set_, p_)  QPSet8_insert((set_), (p_))
        #define QK_prioRemove(set_, p_)  QPSet8_remove((set_), (p_))
    #else
        #define QK_prioNotEmpty(set_)    QPSet64_notEmpty((set_))
        #define QK_prioIsSet(set_, p_)   QPSet64_hasElement((set_), (p_))
        #define QK_prioFindMax(set_, p_) QPSet64_findMax((set_), (p_))
        #define QK_prioInsert(set_, p_)  QPSet64_insert((set_), (p_))
        #define QK_prioRemove(set_, p_)  QPSet64_remove((set_), (p_))
    #endif

    #ifndef QK_ISR_CONTEXT_
        /*!
        * Internal port-specific macro that reports the execution context
        * (ISR vs. thread).
        */
        /*!
        * @returns true if the code executes in the ISR context and false
        * otherwise
        */
        #define QK_ISR_CONTEXT_() (QK_intNest_ != (uint_fast8_t)0)
    #endif /* QK_ISR_CONTEXT_ */

    #define QACTIVE_EQUEUE_WAIT_(me_) \
        (Q_ASSERT_ID(0, (me_)->eQueue.frontEvt != (QEvt *)0))

    #define QACTIVE_EQUEUE_SIGNAL_(me_) do { \
        QK_prioInsert(&QK_readySet_, (me_)->prio); \
        if (!QK_ISR_CONTEXT_()) { \
            uint_fast8_t p = QK_schedPrio_(); \
            if (p != (uint_fast8_t)0) { \
                QK_sched_(p); \
            } \
        } \
    } while (0)

    #define QACTIVE_EQUEUE_ONEMPTY_(me_) \
        QK_prioRemove(&QK_readySet_, (me_)->prio)

    /* native QF event pool operations */
    #define QF_EPOOL_TYPE_            QMPool
    #define QF_EPOOL_INIT_(p_, poolSto_, poolSize_, evtSize_) \
        (QMPool_init(&(p_), (poolSto_), (poolSize_), (evtSize_)))
    #define QF_EPOOL_EVENT_SIZE_(p_)  ((uint_fast16_t)(p_).blockSize)
    #define QF_EPOOL_GET_(p_, e_, m_) ((e_) = (QEvt *)QMPool_get(&(p_), (m_)))
    #define QF_EPOOL_PUT_(p_, e_)     (QMPool_put(&(p_), (e_)))

#endif /* QP_IMPL */

#endif /* qk_h */
