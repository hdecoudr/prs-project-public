/*!
 * \file eventlist.c
 * \brief Implementation of functions related to events.
 *
 * Implementation of the functions declared in the \ref 
 * eventlist.h header.
 *
 * \author H.Decoudras
 * \version 1
 */

#include "eventlist.h"
#include "error.h"

#include <stdlib.h>


/*************************************************************
 *************************************************************
 *
 * New event.
 *
 *************************************************************/
EventList* event_list_new(unsigned int delay, void* parameters)
{
    EventList* node = 
        (EventList*)malloc(sizeof(struct event_list_node));
    exit_on_error(node == NULL);

    node->parameters = parameters;
    node->timer.it_value.tv_sec = delay / 1000;
    node->timer.it_value.tv_usec = (delay % 1000) * 1000;
    node->timer.it_interval.tv_sec = 0;
    node->timer.it_interval.tv_usec = 0;

    struct timeval tv;
    gettimeofday(&tv ,NULL);
    tv.tv_sec -= 3600UL * 24 * 365 * 46; 
    node->when = tv.tv_sec * 1000000UL + tv.tv_usec + delay * 1000;

    node->next = NULL;
    return node;
}

/*************************************************************
 *************************************************************
 *
 * Insert event.
 *
 *************************************************************/
void event_list_insert(EventList** list, EventList* node)
{
    EventList* prev = NULL;
    EventList* current = *list;
    while (current && current->when < node->when)
    {
        prev = current;
        current = current->next;
    }

    node->next = current;
    
    if (prev)
    {
        prev->next = node;
    }
    else
    {
        *list = node;
    }
}

/*************************************************************
 *************************************************************
 *
 * Remove top event.
 *
 *************************************************************/
void event_list_remove_top(EventList** list)
{
    EventList* next = (*list)->next;
    free(*list);
    *list = next;
}

/*************************************************************
 *************************************************************
 *
 * Start event.
 *
 *************************************************************/
void event_list_timer_start(EventList* list)
{
    int result = setitimer(ITIMER_REAL, &list->timer, NULL);
    exit_on_error(result < 0);
}

/*************************************************************
 *************************************************************
 *
 * Update next event.
 *
 *************************************************************/
void event_list_timer_update(EventList* list, unsigned long int prev)
{
    list->timer.it_value.tv_sec = (list->when - prev) / 1000000;
    list->timer.it_value.tv_usec = (list->when - prev) % 1000000;
    list->timer.it_interval.tv_sec = 0;
    list->timer.it_interval.tv_usec = 0;

    int result = setitimer(
        ITIMER_REAL, 
        &list->timer, 
        NULL
    );
    exit_on_error(result < 0);
}

