/*!
 * \ingroup game_group
 * \file eventlist.h
 * \brief Declaration of functions related to events.
 *
 * \author H.Decoudras
 * \version 1
 */

#ifndef DEF_EVENTLIST_H
#define DEF_EVENTLIST_H

#include <sys/time.h>


/*!
 * \brief The \ref event_list_node structure represents
 *        a singly linked list of events.
 */
struct event_list_node
{
    /*!
     * \brief Next event.
     */
    struct event_list_node* next;

    /*!
     * \brief Time remaining to trigger the event.
     */
    struct itimerval timer;

    /*!
     * \brief Time remaining to trigger the event
     *        from 2016 in microseconds.
     */
    unsigned long int when;

    /*!
     * \brief Event parameters.
     */
    void* parameters;
};


/*!
 * \brief Type definition of the \ref event_list_node
 *        structure.
 *
 * \see event_list_node
 */
typedef struct event_list_node EventList;


/*!
 * \brief The event_list_new() function creates an event.
 *
 * This event is ready to be inserted in an existing
 * list of events and is supposed to be armed with
 * the [setitimer(int which, const struct itimerval\* new_value, struct itimerval\* old_value)](https://man7.org/linux/man-pages/man2/setitimer.2.html) system calls.
 *
 * \param delay Number of milliseconds to wait to trigger the event.
 * \param parameters Event parameters.
 *
 * \return An event.
 *
 * \see EventList
 * \see event_list_insert()
 * \see event_list_timer_start()
 * \see event_list_timer_update()
 */
EventList* event_list_new(unsigned int delay, void* parameters);

/*!
 * \brief The event_list_insert() function inserts an
 *        event in a list.
 *
 * The events are sorted by date of delivery.
 *
 * \param list List of events.
 * \param node Event to insert.
 */
void event_list_insert(EventList** list, EventList* node);

/*!
 * \brief The event_list_remove_top() function removes
 *        the first event from the list.
 *
 * \param list List of events.
 */
void event_list_remove_top(EventList** list);

/*!
 * \brief The event_list_timer_start() function starts
 *        the timer of an event.
 *
 * A `SIGALRM` will be raised when the event is triggered.
 *
 * \param list An event.
 */
void event_list_timer_start(EventList* node);

/*!
 * \brief The event_list_timer_update() function updates
 *        the timer of an event.
 *
 * A `SIGALRM` will be raised when the event is triggered.
 *
 * \param node An event.
 * \param prev Previous event delivery date since 2016 in
 *             microseconds.
 */
void event_list_timer_update(EventList* node, unsigned long int prev);


#endif // DEF_EVENTLIST_H

