/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 27, 2019     *
 *                                  *
 *  Assignment 2, CIS*2750          *
 *  CalendarHelper.h                *
 ************************************/

#ifndef CALENDARHELPER_H
#define CALENDARHELPER_H

/*************
 * Libraries *
 *************/

#include <stdio.h>

#include "CalendarParser.h"

/***********************
 * Function Signatures *
 ***********************/

ICalErrorCode writeProperties(FILE *fout, const List *props);

ICalErrorCode writeEvents(FILE *fout, const List *events);

ICalErrorCode writeAlarms(FILE *fout, const List *alarms);

ICalErrorCode writeDateTime(FILE *fout, DateTime dt);

#endif
