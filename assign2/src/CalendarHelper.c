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
 * 	1. INV_CAL
 * 	2. INV_EVENT
 * 	3. INV_ALARM
 * 	4. INV_OTHER and INV_DT
 * 	5. everything else
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
		case INV_DT:
			if (currentHighest != INV_CAL && currentHighest != INV_EVENT && currentHighest != INV_ALARM) {
				currentHighest = newErr;
			}
			break;

		default:
			if (currentHighest != INV_CAL && currentHighest != INV_EVENT && currentHighest != INV_ALARM \
			    && currentHighest != OTHER_ERROR && currentHighest != INV_DT) {
				currentHighest = newErr;
			}
	}

	return currentHighest;
}

/* Validates a list of events to see if they
 *
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
		if ((err = validateProperties(ev->properties)) != OK) {
			highestPriority = higherPriority(highestPriority, err);
		}

		// validate event alarms
		if ((err = validateAlarms(ev->alarms)) != OK) {
			highestPriority = higherPriority(highestPriority, err);
		}
	}

	return highestPriority;
}

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
		if ((err = validateProperties(alm->properties)) != OK) {
			highestPriority = higherPriority(highestPriority, err);
		}
	}

	return highestPriority;
}

ICalErrorCode validateProperties(List *properties) {
	// this one is going to be a monster
}

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

