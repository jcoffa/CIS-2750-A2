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
		if ((err = validatePropertiesEv(ev->properties)) != OK) {
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
	AlarmActionType type;
	char *upper;
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

		// The type of action must be known when validating the properties of an alarm
		upper = strUpperCopy(alm->action);
		if (strstr(upper, "AUDIO") != NULL) {
			type = Audio;
		} else if (strstr(upper, "DISPLAY") != NULL) {
			type = Display;
		} else if (strstr(upper, "EMAIL") != NULL) {
			type = Email;
		} else {
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
		if ((err = validatePropertiesAl(alm->properties, type)) != OK) {
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

	printf("-----STARTED validatePropertiesCal()-----\n");

	// TODO maybe make an array of bool's, and just check if the index is true or false instead
	// of using big switch statements with if/else's

	while ((prop = (Property *)nextElement(&iter)) != NULL) {
		char *printProp = printProperty(prop);
		printf("\t\"%s\"5\n", printProp);
		free(printProp);

		switch (equalsOneOfStr(prop->propName, NUM_CALPROPNAMES, calPropNames)) {
			case -1:
				// the property name did not match any valid Calendar property names
				return INV_CAL;

			case 0:
				printf("\tValidate CALSCALE\n");
				if (calscale) {
					return INV_CAL;
				}
				calscale = true;
				break;

			case 1:
				printf("\tValidate METHOD\n");
				if (method) {
					return INV_CAL;
				}
				method = true;
				break;

			case 2:
			case 3:
				// This should never happen for a valid calendar: Calendar structs have a unique
				// variable to store the PRODID and it should never be in the property list.
				printf("\tValidate PRODID or VERSION\n");
				return INV_CAL;
		}
	}

	printf("-----END validatePropertiesCal()-----\n");
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

	printf("\t----START validatePropertiesEv()-----\n");
	// The following properties MUST NOT occur more than once:
	bool class, created,description, geo, last_mod, location, organizer, \
	     priority, seq, status, summary, transp, url, recurid, rrule;
	// The 'duration' property and 'dtend' property can't show up together in the same event
	bool dtend, duration;
	Property *prop;
	ListIterator iter = createIterator(properties);
	while ((prop = (Property *)nextElement(&iter)) != NULL) {
		char *printProp = printProperty(prop);
		printf("\t\t\"%s\"\n", printProp);
		free(printProp);

		switch (equalsOneOfStr(prop->propName, NUM_EVENTPROPNAMES, eventPropNames)) {
			case -1:
				// the property name did not match any valid Event property names
				return INV_EVENT;
				break;

			case 0:
				printf("\t\tValidate ATTACH\n");
				break;

			case 1:
				printf("\t\tValidate ATTENDEE\n");
				break;

			case 2:
				printf("\t\tValidate CATEGORIES\n");
				break;

			case 3:
				printf("\t\tValidate CLASS\n");
				if (class) {
					return INV_EVENT;
				}
				class = true;
				break;

			case 4:
				printf("\t\tValidate COMMENT\n");
				break;

			case 5:
				printf("\t\tValidate CONTACT\n");
				break;

			case 6:
				printf("\t\tValidate CREATED\n");
				if (created) {
					return INV_EVENT;
				}
				created = true;
				break;

			case 7:
				printf("\t\tValidate DESCRIPTION\n");
				if (description) {
					return INV_EVENT;
				}
				description = true;
				break;

			case 8:
				printf("\t\tValidate DTEND\n");
				if (dtend || duration) {
					return INV_EVENT;
				}
				dtend = true;
				break;

			case 9:
				// This property is already accounted for in the Event structure definition.
				// If it showss up in the property List, then something has gone wrong
				// in createCalendar() as this error should have been caught there.
				printf("\t\tValidate DTSTAMP\n");
				return INV_EVENT;
				break;

			case 10:
				// This property is already accounted for in the Event structure definition.
				// If it showss up in the property List, then something has gone wrong
				// in createCalendar() as this error should have been caught there.
				printf("\t\tValidate DTSTART\n");
				return INV_EVENT;

			case 11:
				printf("\t\tValidate DURATION\n");
				if (dtend || duration) {
					return INV_EVENT;
				}
				duration = true;
				break;

			case 12:
				printf("\t\tValidate EXDATE\n");
				break;

			case 13:
				printf("\t\tValidate GEO\n");
				if (geo) {
					return INV_EVENT;
				}
				geo = true;
				break;

			case 14:
				printf("\t\tValidate LAST-MOD\n");
				if (last_mod) {
					return INV_EVENT;
				}
				last_mod = true;
				break;

			case 15:
				printf("\t\tValidate LOCATION\n");
				if (location){
					return INV_EVENT;
				}
				location = true;
				break;

			case 16:
				printf("\t\tValidate ORGANIZER\n");
				if (organizer) {
					return INV_EVENT;
				}
				organizer = true;
				break;

			case 17:
				printf("\t\tValidate PRIORITY\n");
				if (priority) {
					return INV_EVENT;
				}
				priority = true;
				break;

			case 18:
				printf("\t\tValidate RDATE\n");
				break;

			case 19:
				printf("\t\tValidate RECURID\n");
				if (recurid) {
					return INV_EVENT;
				}
				recurid = true;
				break;

			case 20:
				printf("\t\tValidate RELATED\n");
				break;

			case 21:
				printf("\t\tValidate RESOURCES\n");
				break;

			case 22:
				printf("\t\tValidate RRULE\n");
				if (rrule) {
					return INV_EVENT;
				}
				rrule = true;
				break;

			case 23:
				printf("\t\tValidate RSTATUS\n");
				break;

			case 24:
				printf("\t\tValidate SEQ\n");
				if (seq) {
					return INV_EVENT;
				}
				seq = true;
				break;

			case 25:
				printf("\t\tValidate STATUS\n");
				if (status) {
					return INV_EVENT;
				}
				status = true;
				break;

			case 26:
				printf("\t\tValidate SUMMARY\n");
				if (summary) {
					return INV_EVENT;
				}
				summary = true;
				break;

			case 27:
				printf("\t\tValidate TRANSP\n");
				if (transp) {
					return INV_EVENT;
				}
				transp = true;
				break;

			case 28:
				printf("\t\tValidate UID\n");
				// This property is already accounted for in the Event structure definition.
				// If it showss up in the property List, then something has gone wrong
				// in createCalendar() as this error should have been caught there.
				return INV_EVENT;

			case 29:
				printf("\t\tValidate URL\n");
				if (url) {
					return INV_EVENT;
				}
				url = true;
				break;
		}
	}

	printf("\t-----END validatePropertiesEv()-----\n");
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

	printf("\t\t-----START validtePropertiesAl()-----\n");
	// The following are required for DISPLAY alarms
	bool description;

	// The following are required for all alarm types as long as one of them is present
	// (i.e. they are optional, but if one is present than the other must be present as well)
	bool duration, repeat;

	// The following is required for EMAIL alarms, and can occur more than once
	bool attendee;

	// Required for Email type alarms. Can't occur more than once.
	bool summary;

	// Misc. that can't be declared more than once
	bool attach;

	Property *prop;
	ListIterator iter = createIterator(properties);
	while ((prop = (Property *)nextElement(&iter)) != NULL) {
		char *printProp = printProperty(prop);
		printf("\t\t\t\"%s\"\n", printProp);
		free(printProp);

		switch (equalsOneOfStr(prop->propName, NUM_ALARMPROPNAMES, alarmPropNames)) {
			case -1:
				printf("\t\t\tERROR in validatePropertiesAl: found non-valid propName: \"%s\"\n", prop->propName);
				return INV_ALARM;
				break;

			case 0:
				// This property is already accounted for in the Alarm structure definition.
				// If it showss up in the property List, then something has gone wrong
				// in createCalendar() as this error should have been caught there.

				printf("\t\t\tERROR in validatePropertiesAl: an extra ACTION wiggled through createCalendar()\n");
				return INV_ALARM;
				break;

			case 1:
				printf("\t\t\tValidate ATTACH\n");
				if (type == Display) {
					printf("\t\t\tERROR in validatePropertiesAl: found ATTACH in Display type alarm\n");
					return INV_ALARM;
				} else if (type == Audio) {
					if (attach) {
						printf("\t\t\tERROR in validatePropertiesAl: duplicate ATTACH in Audio type alarm\n");
						return INV_ALARM;
					}
					attach = true;
				}
				break;

			case 2:
				printf("\t\t\tValidate ATTENDEE\n");
				if (type != Email) {
					printf("\t\t\tERROR in validatePropertiesAl: found ATTENDEE in non Email type alarm\n");
					return INV_ALARM;
				}
				attendee = true;
				break;

			case 3:
				printf("\t\t\tValidate DESCRIPTION\n");
				if (type != Display) {
					printf("\t\t\tERROR in validatePropertiesAl: found DESCRIPTION in non Display type alarm\n");
					return INV_ALARM;
				}

				if (description) {
					printf("\t\t\tERROR in validatePropertiesAl: duplicate DESCRIPTION in Display type alarm\n");
					return INV_ALARM;
				}
				description = true;
				break;

			case 4:
				printf("\t\t\tValidate DURATION\n");
				if (duration) {
					printf("\t\t\tERROR in validatePropertiesAl: duplicate DURATION property found\n");
					return INV_ALARM;
				}
				duration = true;
				break;

			case 5:
				printf("\t\t\tValidate REPEAT\n");
				if (repeat) {
					printf("\t\t\tERROR in validatePropertiesAl: duplicate REPEAT property found\n");
					return INV_ALARM;
				}
				repeat = true;
				break;

			case 6:
				printf("\t\t\tValidate SUMMARY\n");
				if (type != Email) {
					printf("\t\t\tERROR in validatePropertiesAl: found SUMMARY in non Email type alarm\n");
					return INV_ALARM;
				}

				if (summary) {
					printf("\t\t\tERROR in validatePropertiesAl: duplicate SUMMARY in Email type alarm\n");
					return INV_ALARM;
				}
				summary = true;
				break;

			case 7:
				// This property is already accounted for in the Alarm structure definition.
				// If it showss up in the property List, then something has gone wrong
				// in createCalendar() as this error should have been caught there.
				printf("\t\t\tValidate TRIGGER\n");
				return INV_ALARM;
				break;
		}

		// duration and repeat properties must either be both present, or neither are present
		if (duration != repeat) {
			printf("\t\t\tERROR in validatePropertiesAl: duration != repeat (%d and %d respectively)\n", \
			       duration, repeat);
			return INV_ALARM;
		}

		// Email type alarms need at least 1 attendee
		if (type == Email && !attendee) {
			printf("\t\t\tERROR in validatePropertiesAl: Email type alarm lacks at least 1 attendee\n");
			return INV_ALARM;
		}
	}

	printf("\t\t-----END validatePropertiesAl()-----\n");
	return OK;
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

