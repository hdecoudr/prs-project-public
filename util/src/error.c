/*!
 * \ingroup util_group
 * \file error.c
 * \brief Implementation of functions related to errors.
 * 
 * Implementation of the functions declared in the \ref 
 * error.h header.
 *
 * \author H. Decoudras
 * \version 1
 */

#include "error.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


/*************************************************************
 *************************************************************
 *
 * Exit on error.
 *
 *************************************************************/
void exit_on_error(int assertion)
{
    if (assertion)
    {
        if (errno) 
        {
            fprintf(stderr, "[%d]: %s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        
        fprintf(stderr, "An error occured!\n");
        exit(EXIT_FAILURE);
    }
}

