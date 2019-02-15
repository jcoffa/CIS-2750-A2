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

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

#include "CalendarParser.h"

/****************
 * Enumerations *
 ****************/

typedef enum type {
	CALENDAR,
	EVENT,
	ALARM
} Type;

/***********************
 * Function Signatures *
 ***********************/

ICalErrorCode writeProperties(FILE *fout, List *props);

ICalErrorCode writeEvents(FILE *fout, List *events);

ICalErrorCode writeAlarms(FILE *fout, List *alarms);

ICalErrorCode getDateTimeAsWritable(char *result, DateTime dt);

ICalErrorCode higherPriority(ICalErrorCode currentHighest, ICalErrorCode newErr);

bool equalsOneOfStr(const char *toCompare, int numArgs, ...);

ICalErrorCode validateEvents(List *events);

ICalErrorCode validateAlarms(List *alarms);

ICalErrorCode validateProperties(List *properties, Type type);

ICalErrorCode validatePropertiesCal(List *properties);

ICalErrorCode validatePropertiesEv(List *properties);

ICalErrorCode validatePropertiesAl(List *properties);

ICalErrorCode validateDateTime(DateTime dt);

#endif
