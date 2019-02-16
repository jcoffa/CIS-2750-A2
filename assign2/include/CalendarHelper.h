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

/**********************
 * Property constants *
 **********************/

#define NUM_CALPROPNAMES 4
const char *calPropNames[NUM_CALPROPNAMES] = {"CALSCALE", "METHOD", "PRODID", "VERSION"};

#define NUM_EVENTPROPNAMES 31
const char *evtPropNames[NUM_EVENTPROPNAMES] = {"ATTACH", "ATTENDEE", "CATEGORIES", "CLASS", "COMMENT", \
	"CONTACT", "CREATED", "DESCRIPTION", "DTEND", "DTSTAMP", "DTSTART", "DURATION", "EXDATE", "EXRULE", \
	"GEO", "LAST-MOD", "LOCATION", "ORGANIZER", "PRIORITY", "RDATE", "RECURID", "RELATED", "RESOURCES", \
	"RRULE", "RSTATUS", "SEQ", "STATUS", "SUMMARY", "TRANSP", "UID", "URL"};

#define NUM_ALARMPROPNAMES 8
const char *alPropNames[NUM_ALARMPROPNAMES] = {"ACTION", "ATTACH", "ATTENDEE", "DESCRIPTION", "DURATION", \
	"REPEAT", "SUMMARY", "TRIGGER"};

/***********************
 * Function Signatures *
 ***********************/

ICalErrorCode writeProperties(FILE *fout, List *props);

ICalErrorCode writeEvents(FILE *fout, List *events);

ICalErrorCode writeAlarms(FILE *fout, List *alarms);

ICalErrorCode getDateTimeAsWritable(char *result, DateTime dt);

ICalErrorCode higherPriority(ICalErrorCode currentHighest, ICalErrorCode newErr);

int equalsOneOfStr(const char *toCompare, int numArgs, const char **strings);

int vequalsOneOfStr(const char *toCompare, int numArgs, ...);

ICalErrorCode validateEvents(List *events);

ICalErrorCode validateAlarms(List *alarms);

ICalErrorCode validateProperties(List *properties, Type type);

ICalErrorCode validatePropertiesCal(List *properties);

ICalErrorCode validatePropertiesEv(List *properties);

ICalErrorCode validatePropertiesAl(List *properties);

ICalErrorCode validateDateTime(DateTime dt);

#endif
