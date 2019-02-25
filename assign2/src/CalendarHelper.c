/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 27, 2019     *
 *                                  *
 *  Assignment 2, CIS*2750          *
 *  CalendarHelper.c                *
 ************************************/

#include "CalendarHelper.h"
#include "Debug.h"
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
	debugMsg("\t-----START writeEvents()-----\n");
    if (fout == NULL || events == NULL) {
		errorMsg("\t\tEither the file pointer or the event List is NULL\n");
        return WRITE_ERROR;
    }

    if (getLength(events) == 0) {
		debugMsg("\t\tNo events to write\n");
        return OK;
    }

    ICalErrorCode err;
    Event *toWrite;
    ListIterator iter = createReverseIterator(events);
    char dateTimeData[100];
    while ((toWrite = (Event *)previousElement(&iter)) != NULL) {
        fprintf(fout, "BEGIN:VEVENT\r\n");
        fprintf(fout, "UID:%s\r\n", toWrite->UID);

		debugMsg("\t\tWrote BEGIN:VEVENT and UID\n");

        if ((err = getDateTimeAsWritable(dateTimeData, toWrite->creationDateTime)) != OK) {
			errorMsg("\t\tEncountered error when getting the creationDateTime as a writable string\n");
            return err;
        }
        fprintf(fout, "DTSTAMP:%s\r\n", dateTimeData);

		debugMsg("\t\tWrote DTSTAMP\n");

        if ((err = getDateTimeAsWritable(dateTimeData, toWrite->startDateTime)) != OK) {
			errorMsg("\t\tEncountered error when getting the startDateTime as a writable string\n");
            return err;
        }
        fprintf(fout, "DTSTART:%s\r\n", dateTimeData);

		debugMsg("\t\tWrote DTSTART\n");

        if ((err = writeProperties(fout, toWrite->properties)) != OK) {
			errorMsg("\t\tEncountered error when writing the properties\n");
            return err;
        }
        if ((err = writeAlarms(fout, toWrite->alarms)) != OK) {
			errorMsg("\t\tEncountered error when writing the alarms\n");
            return err;
        }
        fprintf(fout, "END:VEVENT\r\n");
		debugMsg("\t\tWrote END:VEVENT\n");
    }
	successMsg("\t\t-----END writeEvents()-----\n");

    return OK;
}

/* Writes the alarm list 'alarms' to the file pointed to by 'fout' in the proper
 * iCalendar syntax, including opening and closing VALARM tags.
 */
ICalErrorCode writeAlarms(FILE *fout, List *alarms) {
	debugMsg("\t\t-----START writeAlarms()-----\n");
    if (fout == NULL || alarms == NULL) {
		errorMsg("\t\t\tEither the file pointer or alarms List is NULL\n");
        return WRITE_ERROR;
    }

    if (getLength(alarms) == 0) {
		debugMsg("\t\t\tNo alarms to write\n");
        return OK;
    }

    ICalErrorCode err;
    Alarm *toWrite;
    ListIterator iter = createReverseIterator(alarms);
    while ((toWrite = (Alarm *)previousElement(&iter)) != NULL) {
        fprintf(fout, "BEGIN:VALARM\r\n");
        fprintf(fout, "ACTION:%s\r\n", toWrite->action);
        fprintf(fout, "TRIGGER:%s\r\n", toWrite->trigger);

		debugMsg("\t\t\tWrote BEGIN:VALARM, ACTION, and TRIGGER\n");

        if ((err = writeProperties(fout, toWrite->properties)) != OK) {
			errorMsg("\t\t\tEncountered error when writing properties\n");
            return err;
        }
        fprintf(fout, "END:VALARM\r\n");
		debugMsg("\t\t\tWrote END:VALARM\n");
    }
	successMsg("\t\t\t-----END writeAlarms()-----\n");

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
	ICalErrorCode toReturn = OK;

	char *printCur = printError(currentHighest);
	char *printNew = printError(newErr);
	debugMsg("Current priority error: %s\n", printCur);
	debugMsg("New error: %s\n", printNew);
	
	switch (newErr) {
		case INV_CAL:
			toReturn = newErr;
			break;

		case INV_EVENT:
			if (currentHighest != INV_CAL) {
				toReturn = newErr;
			}
			break;

		case INV_ALARM:
			if (currentHighest != INV_CAL && currentHighest != INV_EVENT) {
				toReturn = newErr;
			}
			break;

		case OTHER_ERROR:
			if (currentHighest != INV_CAL && currentHighest != INV_EVENT && currentHighest != INV_ALARM) {
				toReturn = newErr;
			}
			break;

		default:
			if (currentHighest != INV_CAL && currentHighest != INV_EVENT && currentHighest != INV_ALARM \
			    && currentHighest != OTHER_ERROR) {
				toReturn = newErr;
			}
	}

	char *returnErr = printError(toReturn);
	debugMsg("Returning error: %s\n", returnErr);
	return toReturn;
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
	debugMsg("\t-----START validateEvents()-----\n");
	if (events == NULL) {
		return INV_CAL;
	}

	// Calendars must have at least 1 event
	if (getLength(events) < 1) {
		errorMsg("\t\tEvent List is empty\n");
		return INV_CAL;
	}

	Event *ev;
	ICalErrorCode err, highestPriority;
	highestPriority = OK;
	ListIterator iter = createIterator(events);

	while ((ev = (Event *)nextElement(&iter)) != NULL) {
		// validate UID
		if (ev->UID == NULL) {
			debugMsg("\t\tUID is NULL\n");
			return INV_EVENT;
		}

		// UID can't be empty or longer than 1000 characters (including '\0')
		int lenUID = strlen(ev->UID);
		if (lenUID == 0 || lenUID >= 1000) {
			errorMsg("\t\tUID has invalid length: %d\n", lenUID);
			return INV_EVENT;
		}

		// validate creation and start DateTimes
		if ((err = validateDateTime(ev->creationDateTime) != OK)) {
			errorMsg("\t\tCreation DateTime invalid\n");
			if (err == INV_DT) {
				return INV_EVENT;
			}
			highestPriority = higherPriority(highestPriority, err);
		}
		if ((err = validateDateTime(ev->startDateTime)) != OK) {
			errorMsg("\t\tStart DateTime invalid\n");
			if (err == INV_DT) {
				return INV_EVENT;
			}
			highestPriority = higherPriority(highestPriority, err);
		}

		// validate event properties
		if (ev->properties == NULL) {
			errorMsg("\t\tProperties list is NULL\n");
			return INV_EVENT;
		}
		if ((err = validatePropertiesEv(ev->properties)) != OK) {
			errorMsg("\t\tProperties List encountered error\n");
			highestPriority = higherPriority(highestPriority, err);
		}

		// validate event alarms
		if ((err = validateAlarms(ev->alarms)) != OK) {
			errorMsg("\t\tAlarms List encountered error\n");
			highestPriority = higherPriority(highestPriority, err);
		}

		// a check to fail faster in the case where the highest priority error has already been reached
		if (highestPriority == INV_EVENT) {
			debugMsg("\t\tReturning INV_EVENT\n");
			return INV_EVENT;
		}
	}

	successMsg("\t\t-----END validateEvents()-----\n");
	return highestPriority;
}

/* Validates a list of alarms to determine whether each alarm conforms to the iCalendar
 * specification. Returns the highest priority error, or OK if every alarm in the list
 * conforms to the specification.
 *
 * Highest priority error for this function: INV_ALARM (Priority lvl 3/5)
 */
ICalErrorCode validateAlarms(List *alarms) {
	debugMsg("\t\t-----START validateAlarms()-----\n");
	if (alarms == NULL) {
		errorMsg("\t\t\tAlarm List passed was NULL\n");
		return INV_CAL;
	}

	Alarm *alm;
	AlarmActionType type;
	char *upper;
	ICalErrorCode err, highestPriority;
	highestPriority = OK;
	ListIterator iter = createIterator(alarms);

	while ((alm = (Alarm *)nextElement(&iter)) != NULL) {
		// validate action
		if (alm->action == NULL) {
			errorMsg("\t\t\tAlarm ACTION was NULL\n");
			return INV_ALARM;
		}

		// action can't be empty or longer than 200 characters (including '\0')
		int lenAction = strlen(alm->action);
		if (lenAction == 0 || lenAction >= 200) {
			errorMsg("\t\t\tAlarm ACTION has invalid length: %d\n", lenAction);
			return INV_ALARM;
		}

		// The type of action must be known when validating the properties of an alarm
		upper = strUpperCopy(alm->action);
		if (strstr(upper, "AUDIO") != NULL) {
			debugMsg("\t\t\tAlarm type is AUDIO\n");
			type = Audio;
		} else if (strstr(upper, "DISPLAY") != NULL) {
			debugMsg("\t\t\tAlarm type is DISPLAY\n");
			type = Display;
		} else if (strstr(upper, "EMAIL") != NULL) {
			debugMsg("\t\t\tAlarm type is EMAIL\n");
			type = Email;
		} else {
			errorMsg("\t\t\tAlarm type is invalid\n");
			return INV_ALARM;
		}

		// validate trigger
		if (alm->trigger == NULL) {
			errorMsg("\t\t\tAlarm TRIGGER is NULL\n");
			return INV_ALARM;
		}

		// trigger can't be an empty string
		if (strcmp("", alm->trigger) == 0) {
			errorMsg("\t\t\tAlarm TRIGGER is empty\n");
			return INV_ALARM;
		}

		// validate alarm properties
		if (alm->properties == NULL) {
			errorMsg("\t\t\tAlarm properties List is NULL");
			return INV_ALARM;
		}
		if ((err = validatePropertiesAl(alm->properties, type)) != OK) {
			errorMsg("\t\t\tAlarm Properties encountered an error\n");
			highestPriority = higherPriority(highestPriority, err);
		}

		// a check to fail faster in the case where the highest priority error has already been reached
		if (highestPriority == INV_ALARM) {
			debugMsg("\t\t\tReturning INV_ALARM\n");
			return INV_ALARM;
		}
	}
	
	successMsg("\t\t\t-----END validateAlarms()-----\n");

	return highestPriority;
}

/* Validates a list of properties to determine whether each property conforms to the iCalendar
 * specification with respect to the valid properties of the Calendar itself.
 * Returns the highest priority error, or OK if every property in the list conforms to the specification.
 *
 * For the purposes of this assignment, propDescr is valid as long as it is not NULL or empty.
 * Therefore, the only things that must be validated are whether the propName is valid for the
 * current scope, and occurs a valid number of times (i.e. VERSION occurs exactly once, etc.)
 *
 * Highest priority error for this function: INV_CAL (Priority lvl 5/5)
 */
ICalErrorCode validatePropertiesCal(List *properties) {
	if (properties == NULL) {
		return OTHER_ERROR;
	}

	bool calscale, method;
	calscale = method = false;
	Property *prop;
	ListIterator iter = createIterator(properties);

	debugMsg("\t-----START validatePropertiesCal()-----\n");

	// TODO maybe make an array of bool's, and just check if the index is true or false instead
	// of using big switch statements with if/else's

	while ((prop = (Property *)nextElement(&iter)) != NULL) {
		char *printProp = printProperty(prop);
		notifyMsg("\t\t\"%s\"\n", printProp);
		free(printProp);

		// validate that property description is not empty
		if (prop->propDescr == NULL || (prop->propDescr)[0] == '\0') {
			errorMsg("\t\tProperty description is NULL or empty\n");
			return INV_CAL;
		}

		switch (equalsOneOfStr(prop->propName, NUM_CALPROPNAMES, calPropNames)) {
			case -1:
				// the property name did not match any valid Calendar property names
				return INV_CAL;

			case 0:
				debugMsg("\t\tValidate CALSCALE\n");
				if (calscale) {
					errorMsg("\t\tDuplicate CALSCALE\n");
					return INV_CAL;
				}
				calscale = true;
				break;

			case 1:
				debugMsg("\t\tValidate METHOD\n");
				if (method) {
					errorMsg("\t\tDuplicate METHOD\n");
					return INV_CAL;
				}
				method = true;
				break;

			case 2:
			case 3:
				// This should never happen for a valid calendar: Calendar structs have a unique
				// variable to store the PRODID and it should never be in the property list.
				debugMsg("\t\tValidate PRODID or VERSION\n");
				errorMsg("\t\tDuplicate METHOD\n");
				return INV_CAL;
		}
	}

	successMsg("\t-----END validatePropertiesCal()-----\n");
	return OK;
}

/* Validates a list of properties to determine whether each property conforms to the iCalendar
 * specification with respect to the valid properties of an Event.
 * Returns the highest priority error, or OK if every property in the list conforms to the specification.
 *
 * For the purposes of this assignment, propDescr is valid as long as it is not NULL or empty.
 * Therefore, the only things that must be validated are whether the propName is valid for the
 * current scope, and occurs a valid number of times (i.e. UID occurs exactly once, etc.)
 *
 * Highest priority error for this function: INV_EVENT (Priority lvl 4/5)
 */
ICalErrorCode validatePropertiesEv(List *properties) {
	if (properties == NULL) {
		return OTHER_ERROR;
	}

	// TODO maybe make an array of bool's, and just check if the index is true or false instead
	// of using big switch statements with if/else's

	debugMsg("\t\t----START validatePropertiesEv()-----\n");
	// The following properties MUST NOT occur more than once:
	bool class, created,description, geo, last_mod, location, organizer, \
	     priority, seq, status, summary, transp, url, recurid, rrule;
	class = created = description = geo = last_mod = location = organizer = priority = seq = status = \
		summary = transp = url = recurid = rrule = false;
	// The 'duration' property and 'dtend' property can't show up together in the same event
	bool dtend, duration;
	dtend = duration = false;

	Property *prop;
	ListIterator iter = createIterator(properties);
	while ((prop = (Property *)nextElement(&iter)) != NULL) {
		char *printProp = printProperty(prop);
		notifyMsg("\t\t\t\"%s\"\n", printProp);
		free(printProp);

		// validate that property description is not empty
		if (prop->propDescr == NULL || (prop->propDescr)[0] == '\0') {
			errorMsg("\t\t\tProperty description is NULL or empty\n");
			return INV_EVENT;
		}

		switch (equalsOneOfStr(prop->propName, NUM_EVENTPROPNAMES, eventPropNames)) {
			case -1:
				// the property name did not match any valid Event property names
				return INV_EVENT;
				break;

			case 0:
				debugMsg("\t\t\tValidate ATTACH\n");
				break;

			case 1:
				debugMsg("\t\t\tValidate ATTENDEE\n");
				break;

			case 2:
				debugMsg("\t\t\tValidate CATEGORIES\n");
				break;

			case 3:
				debugMsg("\t\t\tValidate CLASS\n");
				if (class) {
					errorMsg("\t\t\tDuplicate CLASS\n");
					return INV_EVENT;
				}
				class = true;
				break;

			case 4:
				debugMsg("\t\t\tValidate COMMENT\n");
				break;

			case 5:
				debugMsg("\t\t\tValidate CONTACT\n");
				break;

			case 6:
				debugMsg("\t\t\tValidate CREATED\n");
				if (created) {
					errorMsg("\t\t\tDuplicate CREATED\n");
					return INV_EVENT;
				}
				created = true;
				break;

			case 7:
				debugMsg("\t\t\tValidate DESCRIPTION\n");
				if (description) {
					errorMsg("\t\t\tDuplicate DESCRIPTION\n");
					return INV_EVENT;
				}
				description = true;
				break;

			case 8:
				debugMsg("\t\t\tValidate DTEND\n");
				if (dtend || duration) {
					errorMsg("\t\t\tDuplicate DTEND, or DURATION is present\n");
					return INV_EVENT;
				}
				dtend = true;
				break;

			case 9:
				// This property is already accounted for in the Event structure definition.
				// If it showss up in the property List, then something has gone wrong
				// in createCalendar() as this error should have been caught there.
				debugMsg("\t\t\tValidate DTSTAMP\n");
				errorMsg("\t\t\tDTSTAMP found in property List\n");
				return INV_EVENT;

			case 10:
				// This property is already accounted for in the Event structure definition.
				// If it showss up in the property List, then something has gone wrong
				// in createCalendar() as this error should have been caught there.
				debugMsg("\t\t\tValidate DTSTART\n");
				errorMsg("\t\t\tDTSTART found in property List\n");
				return INV_EVENT;

			case 11:
				debugMsg("\t\t\tValidate DURATION\n");
				if (dtend || duration) {
					errorMsg("\t\t\tDuplicate DURATION, or DTEND is present\n");
					return INV_EVENT;
				}
				duration = true;
				break;

			case 12:
				debugMsg("\t\t\tValidate EXDATE\n");
				break;

			case 13:
				debugMsg("\t\t\tValidate GEO\n");
				if (geo) {
					errorMsg("\t\t\tDuplicate GEO\n");
					return INV_EVENT;
				}
				geo = true;
				break;

			case 14:
				debugMsg("\t\t\tValidate LAST-MOD\n");
				if (last_mod) {
					errorMsg("\t\t\tDuplicate LAST-MOD\n");
					return INV_EVENT;
				}
				last_mod = true;
				break;

			case 15:
				debugMsg("\t\t\tValidate LOCATION\n");
				if (location){
					errorMsg("\t\t\tDuplicate LOCATION\n");
					return INV_EVENT;
				}
				location = true;
				break;

			case 16:
				debugMsg("\t\t\tValidate ORGANIZER\n");
				if (organizer) {
					errorMsg("\t\t\tDuplicate ORGANIZER\n");
					return INV_EVENT;
				}
				organizer = true;
				break;

			case 17:
				debugMsg("\t\t\tValidate PRIORITY\n");
				if (priority) {
					errorMsg("\t\t\tDuplicate PRIORITY\n");
					return INV_EVENT;
				}
				priority = true;
				break;

			case 18:
				debugMsg("\t\t\tValidate RDATE\n");
				break;

			case 19:
				debugMsg("\t\t\tValidate RECURID\n");
				if (recurid) {
					errorMsg("\t\t\tDuplicate RECURID\n");
					return INV_EVENT;
				}
				recurid = true;
				break;

			case 20:
				debugMsg("\t\t\tValidate RELATED\n");
				break;

			case 21:
				debugMsg("\t\t\tValidate RESOURCES\n");
				break;

			case 22:
				debugMsg("\t\t\tValidate RRULE\n");
				if (rrule) {
					errorMsg("\t\t\tDuplicate RRULE\n");
					return INV_EVENT;
				}
				rrule = true;
				break;

			case 23:
				debugMsg("\t\t\tValidate RSTATUS\n");
				break;

			case 24:
				debugMsg("\t\t\tValidate SEQ\n");
				if (seq) {
					errorMsg("\t\t\tDuplicate SEQ\n");
					return INV_EVENT;
				}
				seq = true;
				break;

			case 25:
				debugMsg("\t\t\tValidate STATUS\n");
				if (status) {
					errorMsg("\t\t\tDuplicate STATUS\n");
					return INV_EVENT;
				}
				status = true;
				break;

			case 26:
				debugMsg("\t\t\tValidate SUMMARY\n");
				if (summary) {
					errorMsg("\t\t\tDuplicate SUMMARY\n");
					return INV_EVENT;
				}
				summary = true;
				break;

			case 27:
				debugMsg("\t\t\tValidate TRANSP\n");
				if (transp) {
					errorMsg("\t\t\tDuplicate TRANSP\n");
					return INV_EVENT;
				}
				transp = true;
				break;

			case 28:
				debugMsg("\t\t\tValidate UID\n");
				// This property is already accounted for in the Event structure definition.
				// If it showss up in the property List, then something has gone wrong
				// in createCalendar() as this error should have been caught there.
				errorMsg("\t\t\tUID found in property List\n");
				return INV_EVENT;

			case 29:
				debugMsg("\t\t\tValidate URL\n");
				if (url) {
					errorMsg("\t\t\tDuplicate URL\n");
					return INV_EVENT;
				}
				url = true;
				break;
		}
	}

	successMsg("\t\t\t-----END validatePropertiesEv()-----\n");
	return OK;
}

/* Validates a list of properties to determine whether each property conforms to the iCalendar
 * specification with respect to the valid properties of an Alarm.
 * Returns the highest priority error, or OK if every property in the list conforms to the specification.
 *
 * For the purposes of this assignment, propDescr is valid as long as it is not NULL or empty.
 * Therefore, the only things that must be validated are whether the propName is valid for the
 * current scope, and occurs a valid number of times (i.e. ACTION occurs exactly once, etc.)
 *
 * Highest priority error for this function: INV_ALARM (Priority lvl 3/5)
 */
ICalErrorCode validatePropertiesAl(List *properties, AlarmActionType type) {
	if (properties == NULL) {
		return OTHER_ERROR;
	}

	// TODO maybe make an array of bool's, and just check if the index is true or false instead
	// of using big switch statements with if/else's

	debugMsg("\t\t\t-----START validtePropertiesAl()-----\n");
	// The following are required for DISPLAY alarms
	bool description = false;

	// The following are required for all alarm types as long as one of them is present
	// (i.e. they are optional, but if one is present than the other must be present as well)
	bool duration, repeat;
	duration = repeat = false;

	// The following is required for EMAIL alarms, and can occur more than once
	bool attendee = false;

	// Required for Email type alarms. Can't occur more than once.
	bool summary = false;

	// Misc. that can't be declared more than once
	bool attach = false;

	Property *prop;
	ListIterator iter = createIterator(properties);
	while ((prop = (Property *)nextElement(&iter)) != NULL) {
		char *printProp = printProperty(prop);
		notifyMsg("\t\t\t\t\"%s\"\n", printProp);
		free(printProp);

		// validate that property description is not empty
		if (prop->propDescr == NULL || (prop->propDescr)[0] == '\0') {
			errorMsg("\t\t\t\tProperty description is NULL or empty\n");
			return INV_ALARM;
		}

		switch (equalsOneOfStr(prop->propName, NUM_ALARMPROPNAMES, alarmPropNames)) {
			case -1:
				errorMsg("\t\t\t\tfound non-valid propName: \"%s\"\n", prop->propName);
				return INV_ALARM;

			case 0:
				// This property is already accounted for in the Alarm structure definition.
				// If it showss up in the property List, then something has gone wrong
				// in createCalendar() as this error should have been caught there.
				errorMsg("\t\t\t\tan extra ACTION wiggled through createCalendar()\n");
				return INV_ALARM;

			case 1:
				debugMsg("\t\t\t\tValidate ATTACH\n");
				if (type == Display) {
					errorMsg("\t\t\t\tfound ATTACH in Display type alarm\n");
					return INV_ALARM;
				} else if (type == Audio) {
					if (attach) {
						errorMsg("\t\t\t\tduplicate ATTACH in Audio type alarm\n");
						return INV_ALARM;
					}
					attach = true;
				}
				break;

			case 2:
				debugMsg("\t\t\t\tValidate ATTENDEE\n");
				if (type != Email) {
					errorMsg("\t\t\t\tfound ATTENDEE in non Email type alarm\n");
					return INV_ALARM;
				}
				attendee = true;
				break;

			case 3:
				debugMsg("\t\t\t\tValidate DESCRIPTION\n");
				if (type != Display) {
					errorMsg("\t\t\t\tfound DESCRIPTION in non Display type alarm\n");
					return INV_ALARM;
				}

				if (description) {
					errorMsg("\t\t\t\tduplicate DESCRIPTION in Display type alarm\n");
					return INV_ALARM;
				}
				description = true;
				break;

			case 4:
				debugMsg("\t\t\t\tValidate DURATION\n");
				if (duration) {
					errorMsg("\t\t\t\tduplicate DURATION property found\n");
					return INV_ALARM;
				}
				duration = true;
				break;

			case 5:
				debugMsg("\t\t\t\tValidate REPEAT\n");
				if (repeat) {
					errorMsg("\t\t\t\tduplicate REPEAT property found\n");
					return INV_ALARM;
				}
				repeat = true;
				break;

			case 6:
				debugMsg("\t\t\t\tValidate SUMMARY\n");
				if (type != Email) {
					errorMsg("\t\t\t\tfound SUMMARY in non Email type alarm\n");
					return INV_ALARM;
				}

				if (summary) {
					errorMsg("\t\t\t\tduplicate SUMMARY in Email type alarm\n");
					return INV_ALARM;
				}
				summary = true;
				break;

			case 7:
				// This property is already accounted for in the Alarm structure definition.
				// If it showss up in the property List, then something has gone wrong
				// in createCalendar() as this error should have been caught there.
				debugMsg("\t\t\t\tValidate TRIGGER\n");
				return INV_ALARM;
				break;
		}
	}

	// duration and repeat properties must either be both present, or neither are present
	if (duration != repeat) {
		errorMsg("\t\t\t\tduration != repeat (%d and %d respectively)\n", \
			   duration, repeat);
		return INV_ALARM;
	}

	// Email type alarms need at least 1 attendee
	if (type == Email && !attendee) {
		errorMsg("\t\t\t\tEmail type alarm lacks at least 1 attendee\n");
		return INV_ALARM;
	}

	successMsg("\t\t\t\t-----END validatePropertiesAl()-----\n");
	return OK;
}

/* Validates a single DateTime to determine whether it conforms to the iCalendar
 * specification. Returns the highest priority error, or OK if the DateTime
 * conforms to the specification.
 *
 * Highest priority error for this function: INV_DT (Priority lvl 1/5)
 */
ICalErrorCode validateDateTime(DateTime dt) {
	debugMsg("\t\t-----START validateDateTime()-----\n");
	debugMsg("\t\t\tDate: %s, Time: %s, UTC? %s\n", dt.date, dt.time, (dt.UTC) ? "Yes" : "No");

	int lenDate = strlen(dt.date);
	int lenTime = strlen(dt.time);

	debugMsg("\t\t\tlenDate = %d, lenTime = %d\n", lenDate, lenTime);

	// date must be of the form YYYYMMDD = 8 characters
	// time must be of the form HHMMSS = 6 characters
	if (lenDate != 8 || lenTime != 6) {
		errorMsg("\t\t\tEither lenDate != 8 or lenTime != 6 (or both)\n");
		return INV_DT;
	}

	// check if the date contains only numbers
	for (int i = 0; i < lenDate; i++) {
		if (!isdigit((dt.date)[i])) {
			errorMsg("\t\t\tdate contained non-number character\n");
			return INV_DT;
		}
	}

	// check if the time contains only numbers
	for (int i = 0; i < lenTime; i++) {
		if (!isdigit((dt.time)[i])) {
			errorMsg("\t\t\ttime contained non-number character\n");
			return INV_DT;
		}
	}

	successMsg("\t\t\t-----END validateDateTime()-----\n");
	return OK;
}

bool propNamesEqual(const void *first, const void *second) {
	Property *p1 = (Property *)first;
	Property *p2 = (Property *)second;

	return strcmp(p1->propName, p2->propName) == 0;
}

