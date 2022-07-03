/*!
 * \ingroup game_group
 * \file tempo.c
 * \brief Implementation of functions related to
 *        the management of events.
 *
 * Implementation of the functions declared in the \ref
 * timer.h header.
 *
 * \author H.Decoudras
 * \version 1
 */

#include <SDL.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>

#include "timer.h"
#include "eventlist.h"
#include "error.h"


#ifdef PADAWAN


/*!
 * \brief List of events.
 */
static EventList* event_list = NULL;

/*!
 * \brief Mutex for the list of events. 
 */
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


/*!
 * \brief The signal_handler() function is responsible
 *        to deliver and rearm events.
 *
 * \param sig Signal intercepted.
 */
static void signal_handler(int sig);

/*!
 * \brief The worker() function initializes a thread
 *        responsible to intercept and events.
 *
 * \param data Unused.
 *
 * \return The thread identifier.
 *
 * \see signal_handler()
 */
static void* worker(void* data);


/*************************************************************
 *************************************************************
 *
 * Init timer.
 *
 *************************************************************/
int timer_init(void)
{
    sigset_t mask; 
    int result = sigemptyset(&mask);
    exit_on_error(result < 0);

    result = sigaddset(&mask, SIGALRM);
    exit_on_error(result < 0);

    result = pthread_sigmask(SIG_BLOCK, &mask, NULL);
    exit_on_error(result);

    pthread_t thread;
    result = pthread_create(&thread, NULL, worker, NULL);
    exit_on_error(result);

    return 1;
}

/*************************************************************
 *************************************************************
 *
 * Set timer.
 *
 *************************************************************/
void timer_set(Uint32 delay, void* param)
{
    pthread_mutex_lock(&mutex);
    
    EventList* event_node = event_list_new(delay, param);
    event_list_insert(&event_list, event_node);
    
    if (event_list == event_node)
    {
        event_list_timer_start(event_list); 
    }

    pthread_mutex_unlock(&mutex);
}


/*************************************************************
 *************************************************************
 *
 * Signal signal_handler.
 *
 *************************************************************/
void signal_handler(int sig)
{
    pthread_mutex_lock(&mutex);
    
    switch(sig)
    {
        case SIGALRM:
        {
            if (event_list->next != NULL)
            {
                unsigned long prev = event_list->when; 
                sdl_push_event(event_list->parameters);
                event_list_remove_top(&event_list);

                fprintf(
                    stderr, 
                    "Thread [%lx] received signal SIGALRM [%d]\n",
                    pthread_self(), 
                    sig
                );
               
                /* Rearm */
                
                event_list_timer_update(
                    event_list, 
                    prev
                );                    
            }
            else
            {
                sdl_push_event(event_list->parameters);
                event_list_remove_top(&event_list);
            }

            break;
        }

        default:
        {
            fprintf(
                stderr, 
                "Thread [%lx] received signal [%d]\n", 
                pthread_self(), 
                sig
            );
            break;
        }
    } 

    pthread_mutex_unlock(&mutex);
}

/*************************************************************
 *************************************************************
 *
 * Worker thread.
 *
 *************************************************************/
void* worker(void* data)
{
    fprintf(stderr, "Thread [%lx] started!\n", pthread_self());    
    
    sigset_t mask; 
    int result = sigfillset(&mask);
    exit_on_error(result < 0);
    
    result = sigdelset(&mask, SIGALRM);
    exit_on_error(result < 0);

    struct sigaction act;
    act.sa_handler = signal_handler;
    act.sa_flags = 0;
    result = sigemptyset(&act.sa_mask);
    exit_on_error(result < 0);
  
    result = sigaction(SIGALRM, &act, NULL);
    exit_on_error(result < 0);
    
    while (1)
    {
        sigsuspend(&mask);
    }
    
    return (void*)pthread_self();
}


#endif

