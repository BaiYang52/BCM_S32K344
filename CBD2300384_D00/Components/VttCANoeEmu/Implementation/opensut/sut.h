/* OpenSUT programming interface, version 3.0.0
 * Copyright (c) Vector Informatik GmbH. All rights reserved.
 *
 * OpenSUT is a programming interface for controlling and communicating with virtual ECUs.
 */

/*! \file sut.h
 * Contains general functions for SUT lifetime management and event loop control.
 */

#pragma once

#include "opensut/typedefs.h"

#pragma pack(push)
#pragma pack(8)

/*! Initializes the SUT. This function needs to be called prior any other OpenSUT function.
 *  More specifically, the following state machine describes SUT lifetime model:
 *
 *  \verbatim
 *          +---------------+                         +-----------------------------------------------------+
 *  (o) --> |      SUT      |                         |                          SUT                        |
 *          | uninitialized |                         |     (o)              initialized                    |
 *          |               |                         |      |                                              |
 *          |               | --- opensut_init -----> |      V                                              |
 *          |               |                         | +--------+                              +---------+ |
 *          |               | <-- opensut_shutdown -- | | paused | -- opensut_run_async_for --> | running | |
 *          |               |                         | |        | <-- (managed transition) --- |         | |
 *          |               |                         | +--------+                              +---------+ |
 *          +---------------+                         +-----------------------------------------------------+
 *  \endverbatim
 *  Any interaction with the SUT must happen in 'SUT initialized' state.
 *
 *  @param in_init_flags used to set different initialization options. For possible values see \ref opensut_init_flags.
 *  @return
 *  - OPENSUT_OK if initialization was successful
 *  - OPENSUT_PARAMETER_INVALID Invalid in_init_flags
 *  - OPENSUT_FAILED if SUT was already initialized or some error occurred during initialization,
 *          e.g. an out of memory condition or configuration error.
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_init(opensut_init_flags in_init_flags);

/*! Deinitializes the SUT, which comprises the following actions:
 *  - If the SUT is still in substate "running": attempt to cancel ongoing execution.
 *  - Any queued callbacks are discarded.
 *  - Any callback handlers are unregistered.
 *  - Non-volatile memory model is persisted.
 *  - Any dynamic memory allocated for SUT execution management is freed.
 *  - Initialized memory (SUT data section) is re-initialized.
 *  The function does not return until all of these steps are completed.
 *  @return
 *  - OPENSUT_OK if SUT deinitialization was successful.
 *  - OPENSUT_NOTINITIALIZED if SUT is (already) uninitialized
 *  - OPENSUT_FAILED if an error occurred during deinitialization.
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_shutdown();

/*! Grants a virtual time budget for SUT execution.
 *  The SUT will execute asynchronously until the budget is used up.
 *  The first call to this function will start the simulation even if called with a time budget of zero.
 *  The function returns immediately, even if SUT execution is still ongoing.
 *  The execution model is as-fast-as-possible, meaning that internal timers do not consume any wallclock time.
 *  If a coupling between real and virtual time is needed, this function should be called repeatedly with sufficiently
 *  small time slices, with sleep intervals in between. A main loop in such is sketched below:
 *  \code{.c}
 *   clock_t real_start = clock();
 *   opensut_time_t real_now, lead_time, virtual_now = 0, increment = 1000000; // 1ms
 *   while (1) {
 *     opensut_run_async_for(increment);
 *     opensut_do_callbacks(OPENSUT_TIME_INFINITE);
 *     virtual_now += increment;
 *     real_now = 1000000LL * (clock() - real_start);
 *     lead_time = virtual_now - real_now;
 *     if (lead_time > 0) // Positive lead time means that SUT executes faster than real-time.
 *       usleep((useconds_s)(lead_time / 1000)); // Compensate by suspending execution.
 *     // Negative lead time would mean that SUT executes slower than real-time.
 *   }
 *  \endcode
 *  The function can be called, when the SUT is already in state running, which means, that the budget of the
 *  the previous call is not used up yet. In this case, the new time budget for SUT execution is the sum of the
 *  remain budget from the previous call and the budget from the new call.
 *
 *  @param in_diff_time virtual time budget
 *  @return
 *  - OPENSUT_OK if successful
 *  - OPENSUT_PARAMETER_INVALID if a negative value was passed
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_run_async_for(opensut_time_duration_t in_diff_time);

/*! Retrieves the (future) timestamp of the next pending event
 *  while SUT is in 'paused' state. This function might be useful for the realization of coupling algorithms
 *  between virtual time and real time: Since the SUT follows an as-fast-as-possible execution model, it needs
 *  to be throttled by stressing the intervals between calls to opensut_run_async_for(). But in what granularity
 *  should we make those calls? opensut_get_time_of_next_event() may support this decision with facts in that it returns
 *  the timestamp of the next pending event. The difference between that timestamp and the current virtual time is the
 *  time jump which will happen when calling opensut_run_async_for(). For achieving close-to-real-time behavior, we
 *  should accomodate for that time jump by consuming actual wallclock time.
 *  A good strategy might be to accept "small" (e.g. < 100µs) time jumps without any action, but to compensate for
 *  larger jumps by suspending execution until wallclock time has reached the future virtual time.
 *
 *  @param out_time receives the timestamp
 *  @return
 *  - OPENSUT_OK if successful
 *  - OPENSUT_PARAMETER_INVALID if a null pointer was passed
 *  - OPENSUT_FAILED if SUT is not in 'paused' state
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_get_time_of_next_event(opensut_time_t* out_time);

/*! Returns a handle to a notification object, which will be in the signaled state when the following conditions
 *  is true: A callback is queued or the time budget granted by opensut_run_async_for() is used up. The notification
 *  object is in non signaled state, when no callback is queued and the SUT is in state running. Synchronizing with the
 *  notification object depends on the operating system: Under Windows, out_handle maps to a HANDLE which may be passed
 *  to WaitForMultipleObjects (or similar API calls). Under Linux, out_handle maps to a file descriptor, which may be
 *  passed to epoll_wait. The handle is valid as long as the SUT is 'initialized' state. Its lifetime is managed by API
 *  implementation. Hence, it is not necessary to close it.
 *  @param out_handle receives the notification object handle
 *  @return
 *  - OPENSUT_OK if a valid handle was returned
 *  - OPENSUT_PARAMETER_INVALID if out_handle is a null pointer
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_get_poll_handle(portable_handle_t* out_handle);

/*! Invokes callbacks which queue up until the time budget granted by opensut_run_async_for() is used
 *  up (transition from 'running' to 'paused'), or the specified timeout has elapsed, or SUT execution was cancelled
 *  (calling opensut_shutdown() whilst in 'running' state). Then returns.
 *  All callbacks are made in context of the calling thread.
 *  @param in_timeout depicts the wallclock(!) time-out to wait. A value of 0 means that the function just processes
 *  those callbacks which already are in the queue and then returns immediately (typically with OPENSUT_TIMEOUT).
 *  A value of OPENSUT_TIME_INFINITE causes the function to wait indefinitely until one of the remaining conditions
 *  becomes true.
 *  @return
 *  - OPENSUT_OK if call was successful and the SUT is now in 'paused' state
 *  - OPENSUT_TIMEOUT if SUT is still in 'running' state, due to elapsed in_timeout. This is not an error
 *    condition. The function may have processed several callbacks prior to returning OPENSUT_TIMEOUT.
 *  - OPENSUT_PARAMETER_INVALID if in_timeout is invalid
 *  - OPENSUT_NOTINITIALIZED if SUT is uninitialized.
 *  - OPENSUT_FAILED if an error occurred during execution of the callbacks.
 */
OPENSUT_FUNCTION(opensut_result_t) opensut_do_callbacks(opensut_time_duration_t in_timeout);

#pragma pack(pop)
