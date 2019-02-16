/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 27, 2019     *
 *                                  *
 *  Assignment 2, CIS*2750          *
 *  CalendarHelper.c                *
 ************************************/

#include "CalendarHelper.h"
#include "LinkedListHelper.h"
#include "Parsing.h"

/* Writes the property list 'props' to the file pointed to by 'fout' in the proper
 * iCalendar syntax.
 */
ICalErrorCode writeProperties(FILE *fout, List *props) {
    if (fout == NULL || props == NULL) {
        return WRITE_ERROR;
    }

    if (getLength(props) == 0) {
        return OK;
    }

    Property *toWrite;
    ListIterator iter = createReverseIterator(props);
    while ((toWrite = (Property *)previousElement(&iter)) != NULL) {
        fprintf(fout, "%s%c%s\r\n", \
                toWrite->propName, \
                // The comparison below is essentially "does toWrite->propDescr contain a ':' character?"
                // If it does, then it contains parameters, and therefore the name and description
                // must be delimited by a semicolon (;) instead of a colon (:)
                (strcspn(toWrite->propDescr, ":") != strlen(toWrite->propDescr)) ? ';' : ':', \
                toWrite->propDescr);
    }

    return OK;
}

/* Writes the event list 'events' to the file pointed to by 'fout' in the proper
 * iCalendar syntax, including opening and closing VEVENT tags.
 */
ICalErrorCode writeEvents(FILE *fout, List *events) {
    if (fout == NULL || events == NULL) {
        return WRITE_ERROR;
    }

    if (getLength(events) == 0) {
        return OK;
    }

    ICalErrorCode err;
    Event *toWrite;
    ListIterator iter = createReverseIterator(events);
    char dateTimeData[100];
    while ((toWrite = (Event *)previousElement(&iter)) != NULL) {
        fprintf(fout, "BEGIN:VEVENT\r\n");
        fprintf(fout, "UID:%s\r\n", toWrite->UID);
        if ((err = getDateTimeAsWritable(dateTimeData, toWrite->creationDateTime)) != OK) {
            return err;
        }
        fprintf(fout, "DTSTAMP:%s\r\n", dateTimeData);
        if ((err = getDateTimeAsWritable(dateTimeData, toWrite->startDateTime)) != OK) {
            return err;
        }
        fprintf(fout, "DTSTART:%s\r\n", dateTimeData);
        if ((err = writeProperties(fout, toWrite->properties)) != OK) {
            return err;
        }
        if ((err = writeAlarms(fout, toWrite->alarms)) != OK) {
            return err;
        }
        fprintf(fout, "END:VEVENT\r\n");
    }

    return OK;
}

/* Writes the alarm list 'alarms' to the file pointed to by 'fout' in the proper
 * iCalendar syntax, including opening and closing VALARM tags.
 */
ICalErrorCode writeAlarms(FILE *fout, List *alarms) {
    if (fout == NULL || alarms == NULL) {
        return WRITE_ERROR;
    }

    if (getLength(alarms) == 0) {
        return OK;
    }

    ICalErrorCode err;
    Alarm *toWrite;
    ListIterator iter = createReverseIterator(alarms);
    while ((toWrite = (Alarm *)previousElement(&iter)) != NULL) {
        fprintf(fout, "BEGIN:VALARM\r\n");
        fprintf(fout, "ACTION:%s\r\n", toWrite->action);
        fprintf(fout, "TRGIGER:%s\r\n", toWrite->trigger);
        if ((err = writeProperties(fout, toWrite->properties)) != OK) {
            return err;
        }
        fprintf(fout, "END:VALARM\r\n");
    }

    return OK;
}

/* Puts the relevent information from the Datetime structure 'dt' into the
 * string 'result' using the proper iCalendar syntax so that it may be written
 * to a file (after it is prepended by the proper DT___ tag).
 */
ICalErrorCode getDateTimeAsWritable(char *result, DateTime dt) {
    if (result == NULL) {
        return OTHER_ERROR;
    }

    snprintf(result, 100, "%sT%s%s", dt.date, dt.time, (dt.UTC) ? "Z" : "");

    return OK;
}

/* Returns the error code with the higher priority out of 'currentHighest' or 'newErr'.
 * The error code heirarchy is as follows:
 * 		Priority lvl 5/5: INV_CAL
 * 		Priority lvl 4/5: INV_EVENT
 * 		Priority lvl 3/5: INV_ALARM
 * 		Priority lvl 2/5: OTHER_ERROR
 * 		Priority lvl 1/5: everything except OK
 * 		Priority lvl 0/5: OK
 */
ICalErrorCode higherPriority(ICalErrorCode currentHighest, ICalErrorCode newErr) {
	switch (newErr) {
		case INV_CAL:
			currentHighest = newErr;
			break;

		case INV_EVENT:
			if (currentHighest != INV_CAL) {
				currentHighest = newErr;
			}
			break;

		case INV_ALARM:
			if (currentHighest != INV_CAL && currentHighest != INV_EVENT) {
				currentHighest = newErr;
			}
			break;

		case OTHER_ERROR:
			if (currentHighest != INV_CAL && currentHighest != INV_EVENT && currentHighest != INV_ALARM) {
				currentHighest = newErr;
			}
			break;

		case OK:
			// If 'currentHighest' is already OK, then replacing it with another OK does nothing.
			// If 'currentHighest' isn't already OK, then nothing happens, since OK has the lowest priority.
			// In any situation, if 'newErr' is OK, then nothing happens.
			// This case is more for clarity and explanatory reasons than actual functionality.
			break;

		default:
			if (currentHighest != INV_CAL && currentHighest != INV_EVENT && currentHighest != INV_ALARM \
			    && currentHighest != OTHER_ERROR) {
				currentHighest = newErr;
			}
	}

	return currentHighest;
}

/* Returns the index of the string in 'strings' that matches the string 'toCompare'.
 * Returns -1 if 'toCompare' didn't match any of them.
 * This function uses case insensitive string comparison.
 *
 * The number of strings must be known and passed into the function for the variable 'numArgs'.
 */
int equalsOneOfStr(const char *toCompare, int numArgs, const char **strings) {
	char *upper = strUpperCopy(toCompare);

	for (int i = 0; i < numArgs; i++) {
		if (strcmp(upper, strings[i]) == 0) {
			// 'toCompare' matches one of the strings passed
			free(upper);
			return i;
		}
	}

	// 'toCompare' did not match any of the strings passed
	free(upper);
	return -1;
}

/* Returns the index of the string in the variable arguments list that matches the string 'toCompare'.
 * Returns -1 if 'toCompare' didn't match any of them.
 * * This function uses case insensitive string comparison.
 *
 * The number of variable arguments must be known and passed into the function for the variable 'numArgs'.
 */
int vequalsOneOfStr(const char *toCompare, int numArgs, ...) {
	va_list ap;
	char *upper = strUpperCopy(toCompare);

	// initialize 'ap', with 'numArgs' as the last known argument
	va_start(ap, numArgs);

	for (int i = 0; i < numArgs; i++) {
		char *temp = va_arg(ap, char *);
		if (strcmp(upper, temp) == 0) {
			// 'toCompare' matches one of the strings passed
			free(upper);
			va_end(ap);
			return i;
		}
	}

	// 'toCompare' did not match any of the strings passed
	free(upper);
	va_end(ap);
	return -1;
}

/* Validates a list of events to determine whether each event conforms to the iCalendar
 * specification. Returns the highest priority error, or OK if every event in the list
 * conforms to the specification.
 *
 * Highest priority error for this function: INV_EVENT (Priority lvl 4/5)
 */
ICalErrorCode validateEvents(List *events) {
	if (events == NULL) {
		return INV_CAL;
	}

	// Calendars must have at least 1 event
	if (getLength(events) < 1) {
		return INV_CAL;
	}

	Event *ev;
	ICalErrorCode err, highestPriority;
	highestPriority = OK;
	ListIterator iter = createIterator(events);

	while ((ev = (Event *)nextElement(&iter)) != NULL) {
		// validate UID
		if (ev->UID == NULL) {
			return INV_EVENT;
		}

		// UID can't be empty or longer than 1000 characters (including '\0')
		int lenUID = strlen(ev->UID);
		if (lenUID == 0 || lenUID >= 1000) {
			return INV_EVENT;
		}

		// validate creation and start DateTimes
		if ((err = validateDateTime(ev->creationDateTime) != OK)) {
			if (err == INV_DT) {
				return INV_EVENT;
			}
			highestPriority = higherPriority(highestPriority, err);
		}
		if ((err = validateDateTime(ev->startDateTime)) != OK) {
			if (err == INV_DT) {
				return INV_EVENT;
			}
			highestPriority = higherPriority(highestPriority, err);
		}

		// validate event properties
		if (ev->properties == NULL) {
			return INV_EVENT;
		}
		if ((err = validateProperties(ev->properties, EVENT)) != OK) {
			highestPriority = higherPriority(highestPriority, err);
		}

		// validate event alarms
		if ((err = validateAlarms(ev->alarms)) != OK) {
			highestPriority = higherPriority(highestPriority, err);
		}

		// a check to fail faster in the case where the highest priority error has already been reached
		if (highestPriority == INV_EVENT) {
			return INV_EVENT;
		}
	}

	return highestPriority;
}

/* Validates a list of alarms to determine whether each alarm conforms to the iCalendar
 * specification. Returns the highest priority error, or OK if every alarm in the list
 * conforms to the specification.
 *
 * Highest priority error for this function: INV_ALARM (Priority lvl 3/5)
 */
ICalErrorCode validateAlarms(List *alarms) {
	if (alarms == NULL) {
		return INV_CAL;
	}

	Alarm *alm;
	ICalErrorCode err, highestPriority;
	highestPriority = OK;
	ListIterator iter = createIterator(alarms);

	while ((alm = (Alarm *)nextElement(&iter)) != NULL) {
		// validate action
		if (alm->action == NULL) {
			return INV_ALARM;
		}

		// action can't be empty or longer than 200 characters (including '\0')
		int lenAction = strlen(alm->action);
		if (lenAction == 0 || lenAction >= 200) {
			return INV_ALARM;
		}

		// validate trigger
		if (alm->trigger == NULL) {
			return INV_ALARM;
		}

		// trigger can't be an empty string
		if (strcmp("", alm->trigger) == 0) {
			return INV_ALARM;
		}

		// validate alarm properties
		if (alm->properties == NULL) {
			return INV_ALARM;
		}
		if ((err = validateProperties(alm->properties, ALARM)) != OK) {
			highestPriority = higherPriority(highestPriority, err);
		}

		// a check to fail faster in the case where the highest priority error has already been reached
		if (highestPriority == INV_ALARM) {
			return INV_ALARM;
		}
	}

	return highestPriority;
}

/* Validates a list of properties to determine whether each property conforms to the iCalendar
 * specification. Returns the highest priority error, or OK if every property in the list
 * conforms to the specification.
 * Depending on the value of 'type', validateProperties() passes control to the proper
 * property validation function. This is necessary since a property can be valid, but appear
 * in an invalid location, which causes it to be invalid.
 *
 * For example, CALCSCALE:GREGORIAN is a valid property for a Calendar object. If one
 * shows up outside of a Calendar, it is invalid even though the syntax is correct
 * and the property exists in the iCalendar specification.
 *
 * Highest priority error for this function: INV_CAL (Priority lvl 5/5)
 */
ICalErrorCode validateProperties(List *properties, Type type) {
	ICalErrorCode toReturn;

	switch (type) {
		case CALENDAR:
			toReturn = validatePropertiesCal(properties);
			break;

		case EVENT:
			toReturn = validatePropertiesEv(properties);
			break;

		case ALARM:
			toReturn = validatePropertiesAl(properties);
			break;
	}

	return toReturn;
}

/* Validates a list of properties to determine whether each property conforms to the iCalendar
 * specification with respect to the valid properties of the Calendar itself.
 * Returns the highest priority error, or OK if every property in the list conforms to the specification.
 *
 * Highest priority error for this function: INV_CAL (Priority lvl 5/5)
 */
ICalErrorCode validatePropertiesCal(List *properties) {
	if (properties == NULL) {
		return OTHER_ERROR;
	}

	ICalErrorCode highestPriority = OK;
	Property *prop;
	ListIterator iter = createIterator(properties);
	while ((prop = (Property *)nextElement(&iter)) != NULL) {
		// validate
	}

	return highestPriority;
}

/* Validates a list of properties to determine whether each property conforms to the iCalendar
 * specification with respect to the valid properties of an Event.
 * Returns the highest priority error, or OK if every property in the list conforms to the specification.
 *
 * Highest priority error for this function: INV_EVENT (Priority lvl 4/5)
 */
ICalErrorCode validatePropertiesEv(List *properties) {
	if (properties == NULL) {
		return OTHER_ERROR;
	}

	ICalErrorCode highestPriority = OK;
	Property *prop;
	ListIterator iter = createIterator(properties);
	while ((prop = (Property *)nextElement(&iter)) != NULL) {
		// validate
	}

	return highestPriority;
}

/* Validates a list of properties to determine whether each property conforms to the iCalendar
 * specification with respect to the valid properties of an Alarm.
 * Returns the highest priority error, or OK if every property in the list conforms to the specification.
 *
 * Highest priority error for this function: INV_ALARM (Priority lvl 3/5)
 */
ICalErrorCode validatePropertiesAl(List *properties) {
	if (properties == NULL) {
		return OTHER_ERROR;
	}

	ICalErrorCode highestPriority = OK;
	Property *prop;
	ListIterator iter = createIterator(properties);
	while ((prop = (Property *)nextElement(&iter)) != NULL) {
		// validate
	}

	return highestPriority;
}

/* Validates a single DateTime to determine whether it conforms to the iCalendar
 * specification. Returns the highest priority error, or OK if the DateTime
 * conforms to the specification.
 *
 * Highest priority error for this function: INV_DT (Priority lvl 1/5)
 */
ICalErrorCode validateDateTime(DateTime dt) {
	int lenDate = strlen(dt.date);
	int lenTime = strlen(dt.time);

	// date must be of the form YYYYMMDD = 8 characters
	// time must be of the form HHMMSS = 6 characters
	if (lenDate != 8 || lenTime != 6) {
		return INV_DT;
	}

	// check if the date contains only numbers
	for (int i = 0; i < lenDate; i++) {
		if (!isdigit((dt.date)[i])) {
			return INV_DT;
		}
	}

	// check if the time contains only numbers
	for (int i = 0; i < lenTime; i++) {
		if (!isdigit((dt.time)[i])) {
			return INV_DT;
		}
	}

	return OK;
}

