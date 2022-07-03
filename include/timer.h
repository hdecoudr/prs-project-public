/*!
 * \ingroup game_group
 * \file timer.h
 * \brief Declaration of functions related to the 
 *        management of events.
 *
 * \version 1
 */


#ifndef TIMER_IS_DEF
#define TIMER_IS_DEF


/*!
 * \brief The timer_init() function is responsible to
 *        initialize the process signal mask and a thread 
 *        for event handling.
 *
 * \return This function always return \p **1**.
 *
 * \see worker()
 */
int timer_init(void);

/*!
 * \brief The timer_set() function is responsible to
 *        append an event to a list of events and 
 *        initialize the timer for the first event
 *        of the event list.
 *
 * \param delay Triggering delay of the event in milliseconds.
 * \param parameters Event paramesters.
 *
 * \see EventList
 * \see event_list_new()
 * \see event_list_insert()
 */
void timer_set(Uint32 delay, void* parameters);

/*!
 * \brief The sdl_push_event() function triggers
 *        an event.
 *
 * \param parameters Event parameters.
 */
void sdl_push_event(void* parameters);


#endif // TIMER_IS_DEF

