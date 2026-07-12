/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  E2E_SM.c
 *        \brief  E2E source file for AUTOSAR E2E State Machine
 *
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults within
 *                the communication link. E2E Library provides mechanisms for E2E protection, adequate for safety-related
 *                communication having requirements up to ASIL D. The E2E Profile check()-functions verify data in one cycle.
 *                In contrary, the state machine builds up a state out of several results of check() function within a
 *                reception window, which is then provided to the consumer (RTE/SWC/COM).
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "E2E_SM.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

#if !defined (E2ESM_LOCAL) /* COV_E2E_COMPATIBILITY */
# define E2ESM_LOCAL static
#endif

#if !defined (E2ESM_LOCAL_INLINE) /* COV_E2E_COMPATIBILITY */
# define E2ESM_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef P2CONST (E2E_SMCheckStateType, AUTOMATIC, E2E_APPL_DATA) E2E_SMConstStatePtrType;

/**********************************************************************************************************************
 *  LOCAL MACRO DEFINITIONS
 *********************************************************************************************************************/

#define E2E_SM_INC_WRAPAROUND(x, max) (((x)==(max)) ? (0u)  : ((x)+1u)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define E2E_SM_DEC_WRAPAROUND(x, max) (((x)==(0u))  ? (max) : ((x)-1u)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define E2E_START_SEC_CODE
#include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  E2E_SMCheckParams()
 *********************************************************************************************************************/
/*!
 * \brief      Check function of E2Elib StateMachine call parameters
 * \details    Check configuration of E2Elib StateMachine
 * \param[in]  ProfileStatus  Unified profile status value
 * \param[in]  ConfigPtr      Pointer to StateMachine configuration
 * \param[in]  StatePtr       Pointer to StateMachine state
 * \return     Standard return value, indicating success/error
 * \pre        -
 * \context    TASK
 * \reentrant  FALSE
 * \synchronous TRUE
 */
E2ESM_LOCAL FUNC (Std_ReturnType, E2E_CODE) E2E_SMCheckParams(E2E_SMProfileStatusType ProfileStatus,
                                                            E2E_SMConfigPtrType ConfigPtr,
                                                            E2E_SMConstStatePtrType StatePtr);


/**********************************************************************************************************************
 *  E2E_SMAddStatus()
 *********************************************************************************************************************/
/*!
 * \brief          Status ring buffer handling function of E2Elib StateMachine
 * \details        Profile status is added to ring buffer, OkCount and ErrorCount are computed
 * \param[in]      ProfileStatus  Unified profile status value
 * \param[in]      ConfigPtr      Pointer to StateMachine configuration
 * \param[in,out]  StatePtr       Pointer to StateMachine state
 * \pre            -
 * \context        TASK
 * \reentrant      FALSE
 * \synchronous    TRUE
 */
E2ESM_LOCAL FUNC (void, E2E_CODE) E2E_SMAddStatus(E2E_SMProfileStatusType ProfileStatus,
                                                E2E_SMConfigPtrType ConfigPtr,
                                                E2E_SMStatePtrType StatePtr);


/**********************************************************************************************************************
 *  E2E_CheckHandleSMNoData()
 *********************************************************************************************************************/
/*!
 * \brief         Check function of E2Elib StateMachine state SMNoData.
 * \details       -
 * \param[in]     ProfileStatus  Unified profile status value
 * \param[in]     ConfigPtr      Pointer to StateMachine configuration
 * \param[in,out] StatePtr       Pointer to StateMachine state
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
E2ESM_LOCAL FUNC (void, E2E_CODE) E2E_CheckHandleSMNoData(E2E_SMProfileStatusType ProfileStatus,
                                                        E2E_SMConfigPtrType ConfigPtr,
                                                        E2E_SMStatePtrType StatePtr);
    

/**********************************************************************************************************************
 *  E2E_CheckHandleSMInit()
 *********************************************************************************************************************/
/*!
 * \brief         Check function of E2Elib StateMachine state SMInit
 * \details       Check function of E2Elib StateMachine state SMInit
 * \param[in]     ProfileStatus  Unified profile status value
 * \param[in]     ConfigPtr      Pointer to StateMachine configuration
 * \param[in,out] StatePtr       Pointer to StateMachine state
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
E2ESM_LOCAL FUNC (void, E2E_CODE) E2E_CheckHandleSMInit(E2E_SMProfileStatusType ProfileStatus,
                                                      E2E_SMConfigPtrType ConfigPtr,
                                                      E2E_SMStatePtrType StatePtr);


/**********************************************************************************************************************
 *  E2E_CheckHandleSMValid()
 *********************************************************************************************************************/
/*!
 * \brief         Check function of E2Elib StateMachine state SMValid
 * \details       Check function of E2Elib StateMachine state SMValid
 * \param[in]     ProfileStatus  Unified profile status value
 * \param[in]     ConfigPtr      Pointer to StateMachine configuration
 * \param[in,out] StatePtr       Pointer to StateMachine state
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
E2ESM_LOCAL FUNC (void, E2E_CODE) E2E_CheckHandleSMValid(E2E_SMProfileStatusType ProfileStatus,
                                                       E2E_SMConfigPtrType ConfigPtr,
                                                       E2E_SMStatePtrType StatePtr);


/**********************************************************************************************************************
 *  E2E_CheckHandleSMInvalid()
 *********************************************************************************************************************/
/*!
 * \brief         Check function of E2Elib StateMachine state SMInvalid
 * \details       Check function of E2Elib StateMachine state SMInvalid
 * \param[in]     ProfileStatus  Unified profile status value
 * \param[in]     ConfigPtr      Pointer to StateMachine configuration
 * \param[in,out] StatePtr       Pointer to StateMachine state
 * \return        Standard return value, indicating success/error
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
E2ESM_LOCAL FUNC (void, E2E_CODE) E2E_CheckHandleSMInvalid(E2E_SMProfileStatusType ProfileStatus,
                                                         E2E_SMConfigPtrType ConfigPtr,
                                                         E2E_SMStatePtrType StatePtr);


/**********************************************************************************************************************
 *  E2E_GetWindowSizeByState()
 *********************************************************************************************************************/
/*!
 * \brief       Get the window size by the current state.
 * \details     -
 * \param[in]   ConfigPtr      Pointer to StateMachine configuration
 * \param[in]   State          State machine state
 * \return      Window size
 * \pre         -
 * \context     TASK
 * \reentrant   FALSE
 * \synchronous TRUE
 */
E2ESM_LOCAL FUNC (uint8, E2E_CODE) E2E_GetWindowSizeByState(E2E_SMConfigPtrType ConfigPtr,
                                                         E2E_SMStateType State);


/**********************************************************************************************************************
 *  E2E_SMClearStatus()
 *********************************************************************************************************************/
/*!
 * \brief         Clear all attributes in the state pointer.
 * \details       -
 * \param[in]     ConfigPtr      Pointer to StateMachine configuration
 * \param[in,out] StatePtr       Pointer to StateMachine state
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
E2ESM_LOCAL FUNC (void, E2E_CODE) E2E_SMClearStatus(E2E_SMStatePtrType StatePtr,
                                                 E2E_SMConfigPtrType ConfigPtr);           


/**********************************************************************************************************************
 *  E2E_SMClearRemainingStatus()
 *********************************************************************************************************************/
/*!
 * \brief         Clear all elements from the profile state window which are new available in the next state.
 * \details       -
 * \param[in]     ConfigPtr      Pointer to StateMachine configuration
 * \param[in,out] StatePtr       Pointer to StateMachine state
 * \param[in]     NextState      Next state of state machine
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
E2ESM_LOCAL FUNC (void, E2E_CODE) E2E_SMClearRemainingStatus(E2E_SMConfigPtrType ConfigPtr,
                                                            E2E_SMConstStatePtrType StatePtr,
                                                            E2E_SMStateType NextState);

/**********************************************************************************************************************
 * E2E_SMClearForTransitToInvalid()
 *********************************************************************************************************************/
/*!
 * \brief         Clear the full profile state window or the remaining elements depending on configuration.
 * \details       -
 * \param[in]     ConfigPtr      Pointer to StateMachine configuration
 * \param[in,out] StatePtr       Pointer to StateMachine state
 * \pre           -
 * \context       TASK
 * \reentrant     FALSE
 * \synchronous   TRUE
 */
E2ESM_LOCAL FUNC (void, E2E_CODE) E2E_SMClearForTransitToInvalid(E2E_SMConfigPtrType ConfigPtr,
                                                            E2E_SMStatePtrType StatePtr);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  E2E_SMCheckParams()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
E2ESM_LOCAL FUNC (Std_ReturnType, E2E_CODE) E2E_SMCheckParams(E2E_SMProfileStatusType ProfileStatus,
                                                            E2E_SMConfigPtrType ConfigPtr,
                                                            E2E_SMConstStatePtrType StatePtr)
{
    Std_ReturnType ret;
    
    /* #10 Check if ProfileStatus is valid. */
    if ((ProfileStatus == E2E_P_OK)
        || (ProfileStatus == E2E_P_REPEATED)
        || (ProfileStatus == E2E_P_WRONGSEQUENCE)
        || (ProfileStatus == E2E_P_ERROR)
        || (ProfileStatus == E2E_P_NOTAVAILABLE)
        || (ProfileStatus == E2E_P_NONEWDATA))
    {
      ret = E2E_E_OK;
    }
    else
    {
      ret = E2E_E_INPUTERR_WRONG;
    }

    /* #20 Check if WindowSizeValid is valid. */
    if (ConfigPtr->WindowSizeValid == 0u )  /* constr_3176 */ 
    {
        ret = E2E_E_INPUTERR_WRONG;
    }
    /* #30 Check if WindowSizeInvalid is greater than WindowSizeValid. */
    else if (ConfigPtr->WindowSizeInvalid > ConfigPtr->WindowSizeValid ) /* constr_6301 */
    {
        ret = E2E_E_INPUTERR_WRONG;    
    }
    /* #40 Check if WindowSizeInit is greater than WindowSizeValid. */
    else if (ConfigPtr->WindowSizeInit > ConfigPtr->WindowSizeValid ) /* constr_6302 */
    {
        ret = E2E_E_INPUTERR_WRONG;    
    }
    /* #50 Check if MaxErrorStateValid is smaller than MaxErrorStateInit. */
    else if (ConfigPtr->MaxErrorStateValid < ConfigPtr->MaxErrorStateInit ) /* constr_3177 */
    {
        ret = E2E_E_INPUTERR_WRONG;    
    }
    /* #60 Check if MaxErrorStateInit is smaller than MaxErrorStateInvalid. */
    else if (ConfigPtr->MaxErrorStateInit < ConfigPtr->MaxErrorStateInvalid ) /* constr_3177 */
    {
        ret = E2E_E_INPUTERR_WRONG;    
    }
    /* #70 Check if MinOkStateValid is zero. */
    else if (ConfigPtr->MinOkStateValid == 0u ) /* constr_3178 */
    {
        ret = E2E_E_INPUTERR_WRONG;    
    }
    /* #80 Check if MinOkStateValid is greater than MinOkStateInit. */
    else if (ConfigPtr->MinOkStateValid > ConfigPtr->MinOkStateInit  ) /* constr_3178 */
    {
        ret = E2E_E_INPUTERR_WRONG;    
    }
    /* #90 Check if MinOkStateInit is greater than MinOkStateInvalid. */
    else if (ConfigPtr->MinOkStateInit > ConfigPtr->MinOkStateInvalid  ) /* constr_3178 */
    {
        ret = E2E_E_INPUTERR_WRONG;    
    }
    /* #100 Check if MinOkStateInit + MaxErrorStateInit is greater than WindowSizeValid. */
    else if ((ConfigPtr->MinOkStateInit + ConfigPtr->MaxErrorStateInit) > ConfigPtr->WindowSizeValid ) /* constr_3179 */
    {
        ret = E2E_E_INPUTERR_WRONG;    
    }
    /* #110 Check if MinOkStateValid + MaxErrorStateValid is greater than WindowSizeValid. */
    else if ((ConfigPtr->MinOkStateValid + ConfigPtr->MaxErrorStateValid) > ConfigPtr->WindowSizeValid ) /* constr_3180 */
    {
        ret = E2E_E_INPUTERR_WRONG;    
    }
    /* #120 Check if MinOkStateInvalid + MaxErrorStateInvalid is greater than WindowSizeValid. */
    else if ((ConfigPtr->MinOkStateInvalid + ConfigPtr->MaxErrorStateInvalid) > ConfigPtr->WindowSizeValid ) /* constr_3181 */
    {
        ret = E2E_E_INPUTERR_WRONG;    
    }
    /* #130 Check if ProfileStatusWindow is null. */
    else if ( StatePtr->ProfileStatusWindow == NULL_PTR )
    {
        ret = E2E_E_INPUTERR_WRONG;    
    }
    /* #140 Check if WindowTopIndex is greater of equal than WindowSizeValid. */
    else if (StatePtr->WindowTopIndex >= ConfigPtr->WindowSizeValid)
    {
        ret = E2E_E_INPUTERR_WRONG;
    }
    /* #150 Check if SMState is valid. */
    else
    {
        switch (StatePtr->SMState)
        {
            case E2E_SM_VALID: break;
            case E2E_SM_DEINIT: break;
            case E2E_SM_NODATA: break;
            case E2E_SM_INIT: break;
            case E2E_SM_INVALID: break;
            default: ret = E2E_E_INPUTERR_WRONG; break;
        }
    }

    return ret;    
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */


/**********************************************************************************************************************
 *  E2E_SMAddStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
E2ESM_LOCAL FUNC (void, E2E_CODE) E2E_SMAddStatus(E2E_SMProfileStatusType ProfileStatus,
                                                E2E_SMConfigPtrType ConfigPtr,
                                                E2E_SMStatePtrType StatePtr)
{
    uint8 windowCnt;
    uint8 windowIdx;
    uint8 currentWindowSize;

    /* #10 Add ProfileStatus to top of ProfileStatusWindow array */
    StatePtr->ProfileStatusWindow[StatePtr->WindowTopIndex] = (uint8)ProfileStatus; /* SBSW_E2E_03 */

    /* #20 Get window size by the current state. */
    currentWindowSize = E2E_GetWindowSizeByState(ConfigPtr, StatePtr->SMState); /* SBSW_E2E_03 */

    /* #30 Set OkCount to number of elements in the current window with values E2E_P_OK */
    /* #31 Set ErrorCount to number of elements in the current window with values E2E_P_ERROR */
    StatePtr->OkCount = 0u; /* SBSW_E2E_03 */
    StatePtr->ErrorCount = 0u; /* SBSW_E2E_03 */
    windowIdx = StatePtr->WindowTopIndex;
    for (windowCnt = 0u; windowCnt < currentWindowSize; windowCnt++) /* FETA_E2E_SM_ADD_STATUS */
    {
        if (StatePtr->ProfileStatusWindow[windowIdx] == (uint8)E2E_P_OK)
        {
            StatePtr->OkCount++; /* SBSW_E2E_03 */
        }
        else if (StatePtr->ProfileStatusWindow[windowIdx] == (uint8)E2E_P_ERROR)
        {
            StatePtr->ErrorCount++; /* SBSW_E2E_03 */
        }
        else
        {
            /* No other status count is relevant - do nothing */
        }

        windowIdx = E2E_SM_DEC_WRAPAROUND(windowIdx, ConfigPtr->WindowSizeValid - 1u); 
    }

    /* #40 Increment WindowTopIndex */
    /* According to PRS_E2E_00669 in E2E Protocol Specification R20-11, ProfileStatusWindow has size WindowSizeValid. */
    StatePtr->WindowTopIndex = E2E_SM_INC_WRAPAROUND(StatePtr->WindowTopIndex, ConfigPtr->WindowSizeValid - 1u);  /* SBSW_E2E_03 */
}


/**********************************************************************************************************************
 *  E2E_CheckHandleSMNoData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
E2ESM_LOCAL FUNC (void, E2E_CODE) E2E_CheckHandleSMNoData(E2E_SMProfileStatusType ProfileStatus,
                                                        E2E_SMConfigPtrType ConfigPtr,
                                                        E2E_SMStatePtrType StatePtr)
{
    E2E_SMAddStatus (ProfileStatus, ConfigPtr, StatePtr); /* SBSW_E2E_03 */

    /* State->WindowTopIndex == 0 means the window is filled with errors. */
    if((ConfigPtr->TransitToInvalidExtended == TRUE) && (StatePtr->WindowTopIndex == 0u))
    {
        E2E_SMClearForTransitToInvalid(ConfigPtr, StatePtr); /* SBSW_E2E_03 */
        StatePtr->SMState = E2E_SM_INVALID; /* SBSW_E2E_03 */
    }
    else if ((ProfileStatus != E2E_P_ERROR) && (ProfileStatus != E2E_P_NONEWDATA))
    {
        E2E_SMClearStatus(StatePtr, ConfigPtr); /* SBSW_E2E_03 */
        StatePtr->SMState = E2E_SM_INIT; /* SBSW_E2E_03 */
    }
    else
    {
        StatePtr->SMState = E2E_SM_NODATA; /* SBSW_E2E_03 */
    }
}

/**********************************************************************************************************************
 *  E2E_CheckHandleSMInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
E2ESM_LOCAL FUNC (void, E2E_CODE) E2E_CheckHandleSMInit(E2E_SMProfileStatusType ProfileStatus,
                                                      E2E_SMConfigPtrType ConfigPtr,
                                                      E2E_SMStatePtrType StatePtr)
{
    boolean counterRelatedFault;

    E2E_SMAddStatus (ProfileStatus, ConfigPtr, StatePtr); /* SBSW_E2E_03 */

    /* The alternative expression from "Figure 6.190: E2E state machine check" is used to avoid the QAC finding 3103. */
    counterRelatedFault = ((StatePtr->WindowTopIndex == 0u)
        ? (StatePtr->OkCount < ConfigPtr->MinOkStateInit)
        : ((StatePtr->WindowTopIndex - StatePtr->OkCount) > (ConfigPtr->WindowSizeInit - ConfigPtr->MinOkStateInit)));

    if ((StatePtr->ErrorCount > ConfigPtr->MaxErrorStateInit)
        || ((ConfigPtr->TransitToInvalidExtended == TRUE) && (counterRelatedFault == TRUE)))
    {
        E2E_SMClearForTransitToInvalid(ConfigPtr, StatePtr); /* SBSW_E2E_03 */
        StatePtr->SMState = E2E_SM_INVALID; /* SBSW_E2E_03 */
    }
    else if (StatePtr->OkCount >= ConfigPtr->MinOkStateInit)
    {
        E2E_SMClearRemainingStatus(ConfigPtr, StatePtr, E2E_SM_VALID); /* SBSW_E2E_03 */
        StatePtr->SMState = E2E_SM_VALID; /* SBSW_E2E_03 */
    }
    else
    {
        StatePtr->SMState = E2E_SM_INIT; /* SBSW_E2E_03 */
    }
}


/**********************************************************************************************************************
 *  E2E_CheckHandleSMValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
E2ESM_LOCAL FUNC (void, E2E_CODE) E2E_CheckHandleSMValid(E2E_SMProfileStatusType ProfileStatus,
                                                       E2E_SMConfigPtrType ConfigPtr,
                                                       E2E_SMStatePtrType StatePtr)
{
    /* #10 Add ProfileStatus to ProfileStatusWindow and recalculate OkCount and ErrorCount */
    E2E_SMAddStatus (ProfileStatus, ConfigPtr, StatePtr); /* SBSW_E2E_03 */

    /* #20 If the ErrorCount is smaller than allowed maximum and OkCount is greater than minimum, stay in state E2E_SM_VALID. */
    if (  (StatePtr->ErrorCount <= ConfigPtr->MaxErrorStateValid)
       && (StatePtr->OkCount    >= ConfigPtr->MinOkStateValid)  )
    {
        StatePtr->SMState = E2E_SM_VALID; /* SBSW_E2E_03 */
    }
    /* #30 Otherwise: */
    else
    {
        /* #40 If ClearToInvalid is TRUE, clear the status. */
        if(ConfigPtr->ClearToInvalid == TRUE)
        {
            E2E_SMClearStatus(StatePtr, ConfigPtr); /* SBSW_E2E_03 */
        }

        /* #50 Change status to E2E_SM_INVALID. */
        StatePtr->SMState = E2E_SM_INVALID; /* SBSW_E2E_03 */
    }
}


/**********************************************************************************************************************
 *  E2E_CheckHandleSMInvalid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
E2ESM_LOCAL FUNC (void, E2E_CODE) E2E_CheckHandleSMInvalid(E2E_SMProfileStatusType ProfileStatus,
                                                         E2E_SMConfigPtrType ConfigPtr,
                                                         E2E_SMStatePtrType StatePtr)
{
    /* #10 Add ProfileStatus to ProfileStatusWindow and recalculate OkCount and ErrorCount. */
    E2E_SMAddStatus (ProfileStatus, ConfigPtr, StatePtr); /* SBSW_E2E_03 */

    /* #20 If ErrorCount is smaller than allowed maximum and OkCount is greater than minimum:*/
    if (  (StatePtr->ErrorCount <= ConfigPtr->MaxErrorStateInvalid)
       && (StatePtr->OkCount    >= ConfigPtr->MinOkStateInvalid)
       )
    {
        /* #30 Clear the remaining status. */
        E2E_SMClearRemainingStatus(ConfigPtr, StatePtr, E2E_SM_VALID); /* SBSW_E2E_03 */

        /* #40 Change state to E2E_SM_VALID. */
        StatePtr->SMState = E2E_SM_VALID; /* SBSW_E2E_03 */
    }
    /* #50 Otherwise: */
    else
    {
        /* #60 If the ProfileStatus is E2E_P_ERROR, clear the status. */
        if(ProfileStatus == E2E_P_ERROR) 
        {   
            E2E_SMClearStatus(StatePtr, ConfigPtr); /* SBSW_E2E_03 */
        }

        /* #70 Stay in E2E_SM_INVALID. */
    }
}

/**********************************************************************************************************************
 *  E2E_GetWindowSizeByState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
E2ESM_LOCAL FUNC (uint8, E2E_CODE) E2E_GetWindowSizeByState(E2E_SMConfigPtrType ConfigPtr,
                                                         E2E_SMStateType State)
{
    uint8 windowSize = 0;

    if(State == E2E_SM_INIT)
    {
        windowSize = ConfigPtr->WindowSizeInit;
    }
    else if(State == E2E_SM_VALID)
    {
        windowSize = ConfigPtr->WindowSizeValid;
    }
    else /* (State == E2E_SM_INVALID) */
    {
        windowSize = ConfigPtr->WindowSizeInvalid;
    }

    return windowSize;   
}

/**********************************************************************************************************************
 *  E2E_SMClearStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
E2ESM_LOCAL FUNC (void, E2E_CODE) E2E_SMClearStatus(E2E_SMStatePtrType StatePtr,
                                                 E2E_SMConfigPtrType ConfigPtr)
{
    uint8 i; 

    /* According to PRS_E2E_00669 in E2E Protocol Specification R20-11, ProfileStatusWindow has size WindowSizeValid. */
    for (i = 0; i < ConfigPtr->WindowSizeValid; i++) /* FETA_E2E_SM_CLEAR_STATUS */
    {
        StatePtr->ProfileStatusWindow[i] = (uint8)E2E_P_NOTAVAILABLE; /* SBSW_E2E_03 */
    }

    StatePtr->WindowTopIndex = 0; /* SBSW_E2E_03 */
    StatePtr->OkCount = 0; /* SBSW_E2E_03 */
    StatePtr->ErrorCount = 0; /* SBSW_E2E_03 */
}    

/**********************************************************************************************************************
 *  E2E_SMClearRemainingStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
E2ESM_LOCAL FUNC (void, E2E_CODE) E2E_SMClearRemainingStatus(E2E_SMConfigPtrType ConfigPtr,
                                                            E2E_SMConstStatePtrType StatePtr,
                                                            E2E_SMStateType NextState)
{
    uint8 currentWindowSize;
    uint8 nextWindowSize;

    currentWindowSize = E2E_GetWindowSizeByState(ConfigPtr, StatePtr->SMState); /* SBSW_E2E_03 */
    nextWindowSize = E2E_GetWindowSizeByState(ConfigPtr, NextState); /* SBSW_E2E_03 */

    if(currentWindowSize < nextWindowSize)
    {
        uint8 clearCnt;
        uint8 clearIdx = StatePtr->WindowTopIndex; /* WindowTopIndex points already to an element which is not in the current window. */

        /* Set all elements in ProfileStatusWindow to E2E_P_NOTAVAILABLE which are not in the current window. */
        for(clearCnt = 0; clearCnt < (ConfigPtr->WindowSizeValid - currentWindowSize); clearCnt++) /* FETA_E2E_CLEAR_REMAINING */
        {
            StatePtr->ProfileStatusWindow[clearIdx] = (uint8)E2E_P_NOTAVAILABLE; /* SBSW_E2E_03 */

            /* According to PRS_E2E_00669 in E2E Protocol Specification R20-11, ProfileStatusWindow has size WindowSizeValid. */
            clearIdx = E2E_SM_INC_WRAPAROUND(clearIdx, ConfigPtr->WindowSizeValid - 1u);
        }
    }
}

/**********************************************************************************************************************
 * E2E_SMClearForTransitToInvalid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
E2ESM_LOCAL FUNC (void, E2E_CODE) E2E_SMClearForTransitToInvalid(E2E_SMConfigPtrType ConfigPtr,
                                                            E2E_SMStatePtrType StatePtr)
{
    if(ConfigPtr->ClearToInvalid == TRUE)
    {
        E2E_SMClearStatus(StatePtr, ConfigPtr); /* SBSW_E2E_03 */
    }
    else
    {
        E2E_SMClearRemainingStatus(ConfigPtr, StatePtr, E2E_SM_INVALID); /* SBSW_E2E_03 */
    }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  E2E_SMCheckInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_SMCheckInit (E2E_SMStatePtrType StatePtr,
                                                 E2E_SMConfigPtrType ConfigPtr)
{  
    uint8 i;
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* #10 Check if both StatePtr and ConfigPtr are valid */
    if ( (StatePtr != NULL_PTR) && (ConfigPtr != NULL_PTR) )
    {
        /* #20 Initialize ProfileStatus for all elements within the window */
        /* According to PRS_E2E_00669 in E2E Protocol Specification R20-11, ProfileStatusWindow has size WindowSizeValid. */
        for (i = 0; i < ConfigPtr->WindowSizeValid; i++) /* FETA_E2E_SM_CHECK_INIT */
        {
            StatePtr->ProfileStatusWindow[i] = (uint8)E2E_P_NOTAVAILABLE; /* SBSW_E2E_03 */
        }
        /* #30 Initialize parameters of state structure */
        StatePtr->WindowTopIndex = 0; /* SBSW_E2E_03 */
        StatePtr->OkCount = 0; /* SBSW_E2E_03 */
        StatePtr->ErrorCount = 0; /* SBSW_E2E_03 */
        StatePtr->SMState = E2E_SM_NODATA; /* SBSW_E2E_03 */
        ret = E2E_E_OK;
    }

    return ret;
}


/**********************************************************************************************************************
 *  E2E_SMCheck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_SMCheck (E2E_SMProfileStatusType ProfileStatus,
                                             E2E_SMConfigPtrType ConfigPtr,
                                             E2E_SMStatePtrType StatePtr)
{
    Std_ReturnType ret = E2E_E_INPUTERR_NULL;

    /* #10 Check if both StatePtr and ConfigPtr are valid */
    if ( (StatePtr != NULL_PTR) && (ConfigPtr != NULL_PTR) )
    {
        /* #20 Check if parameters of Config- and State-structure are valid */
        ret = E2E_SMCheckParams (ProfileStatus, ConfigPtr, StatePtr); /* SBSW_E2E_03 */
        if (ret == E2E_E_OK)
        {
            /* #30 Compute state machine */
            switch (StatePtr->SMState)
            {
                case E2E_SM_DEINIT:
                    ret = E2E_E_WRONGSTATE;
                    break;
        
                case E2E_SM_NODATA:
                    E2E_CheckHandleSMNoData (ProfileStatus, ConfigPtr, StatePtr); /* SBSW_E2E_03 */
                    break;
        
                case E2E_SM_INIT:
                    E2E_CheckHandleSMInit (ProfileStatus, ConfigPtr, StatePtr); /* SBSW_E2E_03 */
                    break;
        
                case E2E_SM_VALID:
                    E2E_CheckHandleSMValid (ProfileStatus, ConfigPtr, StatePtr); /* SBSW_E2E_03 */
                    break;
        
                case E2E_SM_INVALID:
                    E2E_CheckHandleSMInvalid (ProfileStatus, ConfigPtr, StatePtr); /* SBSW_E2E_03 */
                    break;
        
                default:  ret = E2E_E_INTERR;  break; /* COV_E2E_MISRA */
            }
        }
    }
    return ret;
}

#define E2E_STOP_SEC_CODE
#include "E2E_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: E2E_SM.c
 *********************************************************************************************************************/
