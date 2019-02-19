/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 27, 2019     *
 *                                  *
 *  Assignment 2, CIS*2750          *
 *  CalendarParser.c                *
 ************************************/

#include "CalendarParser.h"
#include "CalendarHelper.h"
#include "LinkedListAPI.h"
#include "Parsing.h"
#include "Initialize.h"

/** Function to create a Calendar object based on the contents of an iCalendar file.
 *@pre File name cannot be an empty string or NULL.  File name must have the .ics extension.
       File represented by this name must exist and must be readable.
 *@post Either:
        A valid calendar has been created, its address was stored in the variable obj, and OK was returned
		or
		An error occurred, the calendar was not created, all temporary memory was freed, obj was set to NULL, and the
		appropriate error code was returned
 *@return the error code indicating success or the error encountered when parsing the calendar
 *@param fileName - a string containing the name of the iCalendar file
 *@param a double pointer to a Calendar struct that needs to be allocated
**/
ICalErrorCode createCalendar(char* fileName, Calendar** obj) {
    FILE *fin;
    ICalErrorCode error;
    bool version, prodID, method, beginCal, endCal, foundEvent;
    char *parse;
    version = prodID = method = beginCal = endCal = foundEvent = false;

    // Prof said not to check for obj being NULL, but you can't dereference a NULL pointer,
    // so I think he meant "don't worry if *obj = NULL, since it is being overwritten", and in
    // order to dereference it then the double pointer passed into the function can't be NULL.
    if (obj == NULL) {
        return OTHER_ERROR;
    }

    // filename can't be null or an empty string, and must end with the '.ics' extension
    if (fileName == NULL || strcmp(fileName, "") == 0 || !endsWith(fileName, ".ics")) {
        *obj = NULL;
        return INV_FILE;
    }

    fin = fopen(fileName, "r");

    // Check that file was found/opened correctly
    if (fin == NULL) {
        // On a failure, the obj argument is set to NULL and an error code is returned
        *obj = NULL;
        return INV_FILE;
    }

    // allocate memory for the Calendar and all its components
    if ((error = initializeCalendar(obj)) != OK) {
        return error;
    }

    char line[10000];
    while (!feof(fin)) {
        // readFold returns NULL when the raw line does not end with a \r\n sequence
        // (i.e. the file has invalid line endings)
        if ((error = readFold(line, 10000, fin)) != OK) {
            cleanup(obj, NULL, fin);
            return error;
        }

        parse = strUpperCopy(line);

        //fprintf(stdout, "DEBUG: in createCalendar: upper'd line = \"%s\"\n", parse);

        // Empty lines/lines containing just whitespace are NOT permitted
        // by the iCal specification.
        // (readFold function automatically trims whitespace)
        if (strlen(parse) == 0) {
            cleanup(obj, parse, fin);
            return INV_CAL;
        }
        
        if (startsWith(parse, ";")) {
            // lines starting with a semicolon (;) are comments, and
            // should be ignored
            free(parse);
            parse = NULL;
            continue;
        }

        // Check if the END:VCALENDAR has been hit. If it has, and there is still more file to be read,
        // then something has gone wrong.
        if (endCal) {
            cleanup(obj, parse, fin);
            return INV_CAL;
        }

        // The first non-commented line must be BEGIN:VCALENDAR
        if (!beginCal && !strcmp(parse, "BEGIN:VCALENDAR") == 0) {
            cleanup(obj, parse, fin);
            return INV_CAL;
        } else if (!beginCal) {
            beginCal = true;
            free(parse);
            continue;
        }
        

        // add properties, alarms, events, and other elements to the calendar
        if (startsWith(parse, "VERSION:")) {
            if (version) {
                cleanup(obj, parse, fin);
                return DUP_VER;
            }

            //fprintf(stdout, "DEBUG: in createCalendar: found VERSION line: \"%s\"\n", line);
            char *endptr;
            // +8 to start conversion after the 'VERSION:' part of the string
            (*obj)->version = strtof(line + 8, &endptr);

            if (strlen(line + 8) == 0 || line+8 == endptr || *endptr != '\0') {
                // VERSION property contains no data after the ':', or the data
                // could not be converted into a number
                cleanup(obj, parse, fin);
                return INV_VER;
            }

            //fprintf(stdout, "DEBUG: in createCalendar: set version to %f\n", (*obj)->version);
            version = true;
        } else if (startsWith(parse, "PRODID:")) {
            if (prodID) {
                cleanup(obj, parse, fin);
                return DUP_PRODID;
            }

            // PRODID: contains no information
            if (strlen(line + 7) == 0) {
                cleanup(obj, parse, fin);
                return INV_PRODID;
            }

            // +7 to only copy characters past 'PRODID:' part of the string
            //fprintf(stdout, "DEBUG: in createCalendar: found PRODID line: \"%s\"\n", line);
            strcpy((*obj)->prodID, line + 7);
            //fprintf(stdout, "DEBUG: in createCalendar: set product ID to\"%s\"\n", (*obj)->prodID);
            prodID = true;
        } else if (startsWith(parse, "METHOD:")) {
            if (method) {
                cleanup(obj, parse, fin);
                return INV_CAL;
            }

            // METHOD: contains no information
            if (strlen(line + 7) == 0) {
                cleanup(obj, parse, fin);
                return INV_CAL;
            }

            //fprintf(stdout, "DEBUG: in createCalendar: found METHOD line: \"%s\"\n", line);
            Property *methodProp;
            if ((error = initializeProperty(line, &methodProp)) != OK) {
                // something happened, and the property could not be created properly
                cleanup(obj, parse, fin);
                return INV_CAL;
            }

            insertFront((*obj)->properties, (void *)methodProp);
            method = true;
        } else if (strcmp(parse, "END:VCALENDAR") == 0) {
            endCal = true;
        } else if (strcmp(parse, "BEGIN:VCALENDAR") == 0) {
            // only 1 calendar allowed per file
            cleanup(obj, parse, fin);
            return INV_CAL;
        } else if (strcmp(parse, "BEGIN:VEVENT") == 0) {
            Event *event;
            if ((error = getEvent(fin, &event)) != OK) {
                // something happened, and the event could not be created properly
                cleanup(obj, parse, fin);
                return error;
            }
            foundEvent = true;

            insertFront((*obj)->events, (void *)event);
        } else if (strcmp(parse, "BEGIN:VALARM") == 0) {
            // there can't be an alarm for an entire calendar
            //fprintf(stdout, "ERROR: in createCalendar: found an alarm not in an event\n");
            cleanup(obj, parse, fin);
            return INV_ALARM;
        } else if (strcmp(parse, "END:VEVENT") == 0 || strcmp(parse, "END:VALARM") == 0) {
            // a duplicated END tag was found
            //fprintf(stdout, "Found a duplicated END tag: \"%s\"\n", line);
            cleanup(obj, parse, fin);
            return INV_CAL;
        } else {
            // All other BEGIN: clauses have been handled in their own 'else if' case.
            // If another one is hit, then it is an error.
            if (startsWith(parse, "BEGIN:")) {
                //fprintf(stdout, "ERROR: in createCalendar: Found an illegal BEGIN: \"%s\"\n", line);
                cleanup(obj, parse, fin);
                return INV_CAL;
            }

            //fprintf(stdout, "DEBUG: in createCalendar: found non-mandatory property: \"%s\"\n", line);
            Property *prop;
            if ((error = initializeProperty(line, &prop)) != OK) {
                // something happened, and the property could not be created properly
                cleanup(obj, parse, fin);
                return INV_CAL;
            }

            insertFront((*obj)->properties, (void *)prop);
        }

        free(parse);
        parse = NULL;
    }
    fclose(fin);

    // Calendars require a few mandatory elements. If one does not have
    // any of these properties/lines, it is invalid.
    if (!endCal || !foundEvent || !version || !prodID) {
        cleanup(obj, parse, NULL);
        return INV_CAL;
    }

    // the file has been parsed, mandatory properties have been found,
    // and the calendar is valid (or at least valid with respect to
    // the syntax of an iCalendar file)
    return OK;
}


/** Function to delete all calendar content and free all the memory.
 *@pre Calendar object exists, is not null, and has not been freed
 *@post Calendar object had been freed
 *@return none
 *@param obj - a pointer to a Calendar struct
**/
void deleteCalendar(Calendar* obj) {
    freeList(obj->events);
    freeList(obj->properties);
    free(obj);
}


/** Function to create a string representation of a Calendar object.
 *@pre Calendar object exists, is not null, and is valid
 *@post Calendar has not been modified in any way, and a string representing the Calendar contents has been created
 *@return a string contaning a humanly readable representation of a Calendar object
 *@param obj - a pointer to a Calendar struct
**/
char* printCalendar(const Calendar* obj) {
    if (obj == NULL) {
        return NULL;
    }

    char *eventListStr = toString(obj->events);
    char *propertyListStr = toString(obj->properties);

	int size = strlen(eventListStr) + strlen(propertyListStr) + 1000;
	char *toReturn = malloc(size);

    // check for malloc failing
    if (toReturn == NULL) {
        return NULL;
    }

    // A neat little function I found that allows for string creation using printf
    // format specifiers. Makes stringing information together in a string like this
    // much easier than using strcat() repeatedly!
    snprintf(toReturn, size, "Start CALENDAR: {VERSION=%.2f, PRODID=%s, Start EVENTS={%s\n} End EVENTS, Start PROPERTIES={%s\n} End PROPERTIES}, End CALENDAR", \
             obj->version, obj->prodID, eventListStr, propertyListStr);

    free(eventListStr);
    free(propertyListStr);

    return realloc(toReturn, strlen(toReturn) + 1);
}


/** Function to "convert" the ICalErrorCode into a humanly redabale string.
 *@return a string containing a humanly readable representation of the error code by indexing into
          the descr array using the error code enum value as an index
 *@param err - an error code
**/
char* printError(ICalErrorCode err) {
    // FIXME this doesn't do what the description specificly says;
    // no array named 'descr' is found anywhere in the assignment outline,
    // so I can't exactly index it.
    char *toReturn = malloc(200);
    
    switch ((int)err) {
        case OK:
            strcpy(toReturn, "OK: No errors");
            break;

        case INV_FILE:
            strcpy(toReturn, "Invalid file");
            break;

        case INV_CAL:
            strcpy(toReturn, "Invalid calendar");
            break;

        case INV_VER:
            strcpy(toReturn, "Invalid version");
            break;

        case DUP_VER:
            strcpy(toReturn, "Duplicate version");
            break;

        case INV_PRODID:
            strcpy(toReturn, "Invalid product ID");
            break;

        case DUP_PRODID:
            strcpy(toReturn, "Duplicate product ID");
            break;

        case INV_EVENT:
            strcpy(toReturn, "Invalid event");
            break;

        case INV_DT:
            strcpy(toReturn, "Invalid date-time");
            break;

        case INV_ALARM:
            strcpy(toReturn, "Invalid alarm");
            break;

        case WRITE_ERROR:
            strcpy(toReturn, "Write error");
            break;

        case OTHER_ERROR:
            strcpy(toReturn, "Other error");
            break;

        default:
            sprintf(toReturn, "Unknown error: Found error with value %d", err);
            break;
    }

    return realloc(toReturn, strlen(toReturn) + 1);
}


/** Function to writing a Calendar object into a file in iCalendar format.
 *@pre Calendar object exists, is not null, and is valid
 *@post Calendar has not been modified in any way, and a file representing the
        Calendar contents in iCalendar format has been created
 *@return the error code indicating success or the error encountered when parsing the calendar
 *@param obj - a pointer to a Calendar struct
 **/
ICalErrorCode writeCalendar(char* fileName, const Calendar* obj) {
    FILE *fout;

    if (fileName == NULL || obj == NULL) {
        return WRITE_ERROR;
    }

    if (strcmp(fileName, "") == 0 || !endsWith(fileName, ".ics")) {
        return WRITE_ERROR;
    }
    
    if ((fout = fopen(fileName, "w")) == NULL) {
        return WRITE_ERROR;
    }

    ICalErrorCode err;
    fprintf(fout, "BEGIN:VCALENDAR\r\n");
    fprintf(fout, "PRODID:%s\r\n", obj->prodID);
    fprintf(fout, "VERSION:%.1f\r\n", obj->version);
    if ((err = writeProperties(fout, obj->properties)) != OK) {
        return WRITE_ERROR;
    }
    if ((err = writeEvents(fout, obj->events)) != OK) {
        return WRITE_ERROR;
    }
    fprintf(fout, "END:VCALENDAR\r\n");

    return OK;
}


/** Function to validating an existing a Calendar object
 *@pre Calendar object exists and is not null
 *@post Calendar has not been modified in any way
 *@return the error code indicating success or the error encountered when validating the calendar
 *@param obj - a pointer to a Calendar struct
 **/
ICalErrorCode validateCalendar(const Calendar* obj) {
	if (obj == NULL) {
		return INV_CAL;
	}

	// check for NULL Calendar members
	if (obj->prodID == NULL || obj->events == NULL || obj->properties == NULL) {
		return INV_CAL;
	}

	// verify the version
	if (obj->version <= 0.0) {
		return INV_CAL;
	}

	// verify the product ID
	int lenID = strlen(obj->prodID);
	if (lenID <= 0 || lenID >= 1000) {
		return INV_CAL;
	}

	// 'highestPriority' is necessary due to one of the last paragraphs of Module 2:
	//	"If the struct contains multiple errors, the error code should correspond to the highest level
	//	of error code that you encounter. For example, if the argument to validateCalendar contains:
	//	- an invalid Calendar property, and
	//	- an invalid Alarm component inside an Event component
	//	you must return INV_CAL, not INV_ALARM."
	ICalErrorCode err, highestPriority;

	// initialize to a dummy value of OK to symbolize no errors have been encountered
	highestPriority = OK;

	// verify events
	if ((err = validateEvents(obj->events)) != OK) {
		highestPriority = err;
	}

	// verify calendar properties
	if ((err = validatePropertiesCal(obj->properties) != OK)) {
		// determine the priority of the new error
		highestPriority = higherPriority(highestPriority, err);
	}

	// Return the highest priority error. This variable is initialized to OK, so if no errors were encountered
	// then it returns OK; indicative of a valid calendar with no errors.
    return highestPriority;
}


/** Function to converting a DateTime into a JSON string
 *@pre N/A
 *@post DateTime has not been modified in any way
 *@return A string in JSON format
 *@param prop - a DateTime struct
 **/
char* dtToJSON(DateTime prop) {
	char *toReturn = malloc(1000);

	int written = snprintf(toReturn, 1000, "{\"date\":\"%s\",\"time\":\"%s\",\"isUTC\":%s}", prop.date, prop.time, \
	                       (prop.UTC) ? "true" : "false");

	return realloc(toReturn, written + 1);
}

/** Function to converting an Event into a JSON string
 *@pre Event is not NULL
 *@post Event has not been modified in any way
 *@return A string in JSON format
 *@param event - a pointer to an Event struct
 **/
char* eventToJSON(const Event* event) {
	char *toReturn;
	int written;

	if (event == NULL) {
		// In the case where an event is NULL, an empty JSON string is returned
		toReturn = malloc(3);
		strcpy(toReturn, "{}");
		written = 2;
	} else {
		char *startDT = dtToJSON(event->startDateTime);

		// Create a dummy property to find the "SUMMARY" property in 'event', if it exists
		Property *dummy = malloc(sizeof(Property));
		strcpy(dummy->propName, "SUMMARY");
		Property *summary = findElement(event->properties, propNamesEqual, dummy);

		// Allocate memory depending on whether a SUMMARY property needs to be written
		int size = (summary == NULL) ? 500 : strlen(summary->propDescr) + 500;
		toReturn = malloc(size);

		// Write the JSON in toReturn
		written = snprintf(toReturn, size, "{\"startDT\":%s,\"numProps\":%d,\"numAlarms\":%d,\"summary\":\"%s\"}", \
		                   startDT, getLength(event->properties), getLength(event->alarms), \
		                   // findElement returns NULL if the property could not be found in 'event',
		                   // in which case an empty string is written instead of the summary properties description
		                   (summary == NULL) ? "" : summary->propDescr);
	}

	return realloc(toReturn, written + 1);
}

/** Function to converting an Event list into a JSON string
 *@pre Event list is not NULL
 *@post Event list has not been modified in any way
 *@return A string in JSON format
 *@param eventList - a pointer to an Event list
 **/
char* eventListToJSON(const List* eventList) {
	char *toReturn, *tempEvJSON;
	int currentLength;

	if (eventList == NULL || getLength(eventList) == 0) {
		toReturn = malloc(3);
		strcpy(toReturn, "[]");
		return toReturn;
	}

	// Start by putting the initial bracket '[' in the JSON
	toReturn = malloc(2);
	strcpy(toReturn, "[");
	currentLength = 2;

	// Add all the event JSON's to 'toReturn'
	ListIterator iter = createIterator(eventList);
	Event *ev;
	while ((ev = (Event *)nextElement(&iter)) != NULL) {
		tempEvJSON = eventToJSON(ev);
		currentLength += strlen(tempEvJSON) + 1;
		toReturn = realloc(toReturn, currentLength);

		strcat(toReturn, tempEvJSON);
		strcat(toReturn, ",");

		free(tempEvJSON);
	}

	// There will always be a trailing comma at the very end of the string
	toReturn[currentLength-1] = ']';

	return toReturn;
}

/** Function to converting a Calendar into a JSON string
 *@pre Calendar is not NULL
 *@post Calendar has not been modified in any way
 *@return A string in JSON format
 *@param cal - a pointer to a Calendar struct
 **/
char* calendarToJSON(const Calendar* cal) {
	char *toReturn;
	int written;

	if (cal == NULL) {
		toReturn = malloc(3);
		strcpy(toReturn, "{}");
		return toReturn;
	}

	toReturn = malloc(2000);
	written = snprintf(toReturn, 2000, "{\"version\":%.1f,\"prodID\":\"%s\",\"numProps\":%d,\"numEvents\":%d}", \
	                   cal->version, cal->prodID, getLength(cal->properties) + 2, getLength(cal->events));

	return realloc(toReturn, written + 1);
}

/** Function to converting a JSON string into a Calendar struct
 *@pre JSON string is not NULL
 *@post String has not been modified in any way
 *@return A newly allocated and partially initialized Calendar struct
 *@param str - a pointer to a string
 **/
Calendar* JSONtoCalendar(const char* str) {
	if (str == NULL) {
		return NULL;
	}

	Calendar *toReturn;
	if (initializeCalendar(&toReturn) != OK) {
		return NULL;
	}

	// The JSON string 'str' is similar to the JSON string created by calendarToJSON(),
	// except it lacks the list info.
	if (sscanf(str, "{\"version\":%f,\"prodID\":\"%s\"}", &(toReturn->version), toReturn->prodID) < 2) {
		return NULL;
	}

	return toReturn;
}

/** Function to converting a JSON string into an Event struct
 *@pre JSON string is not NULL
 *@post String has not been modified in any way
 *@return A newly allocated and partially initialized Event struct
 *@param str - a pointer to a string
 **/
Event* JSONtoEvent(const char* str) {
	if (str == NULL) {
		return NULL;
	}

	Event *toReturn;
	if (initializeEvent(&toReturn) != OK) {
		return NULL;
	}

	// The JSON string 'str' contains only a "UID" field
	if (sscanf(str, "{\"UID\":\"%s\"}", toReturn->UID) < 1) {
		return NULL;
	}

	return toReturn;
}

/** Function to adding an Event struct to an ixisting Calendar struct
 *@pre arguments are not NULL
 *@post The new event has been added to the calendar's events list
 *@return N/A
 *@param cal - a Calendar struct
 *@param toBeAdded - an Event struct
 **/
void addEvent(Calendar* cal, Event* toBeAdded) {
	if (cal == NULL || toBeAdded == NULL) {
		return;
	}

	// The event must be specifically added to the END of the events list
	insertBack(cal->events, toBeAdded);
}

// ************* List helper functions - MUST be implemented ***************

/*
 */
void deleteEvent(void* toBeDeleted) {
    if (!toBeDeleted) {
        return;
    }

    Event *ev = (Event *)toBeDeleted;

    // DateTime objects don't need to be freed (they aren't
    // stored as pointers in an Event struct)
    freeList(ev->properties);
    freeList(ev->alarms);

    free(ev);
}

/*
 */
int compareEvents(const void* first, const void* second) {
    Event *e1 = (Event *)first;
    Event *e2 = (Event *)second;

    return strcmp(e1->UID, e2->UID);
}

/*
 */
char* printEvent(void* toBePrinted) {
    if (!toBePrinted) {
        return NULL;
    }

    Event *ev = (Event *)toBePrinted;

    // DateTime's and Lists have their own print functions
    char *createStr = printDate(&(ev->creationDateTime));
    char *startStr = printDate(&(ev->startDateTime));
    char *propsStr = toString(ev->properties);
    char *alarmsStr = toString(ev->alarms);

    int length = strlen(createStr) + strlen(startStr) + strlen(propsStr) + strlen(alarmsStr) + 200;
    char *toReturn = malloc(length);

    snprintf(toReturn, length, "Start EVENT {EventUID: \"%s\", EventCreate: \"%s\", EventStart: \"%s\", EVENT_PROPERTIES: {%s\n} End EVENT_PROPERTIES, Start EVENT_ALARMS: {%s\n} End EVENT_ALARMS} End EVENT", \
             ev->UID, createStr, startStr, propsStr, alarmsStr);

    // Free dynamically allocated print strings
    free(createStr);
    free(startStr);
    free(propsStr);
    free(alarmsStr);

    return realloc(toReturn, strlen(toReturn) + 1);
}


/*
 */
void deleteAlarm(void* toBeDeleted) {
    if (!toBeDeleted) {
        return;
    }

    Alarm *al = (Alarm *)toBeDeleted;

    free(al->trigger);
    freeList(al->properties);

    free(al);
}

/*
 * Compares the 'action' properties of two alarms.
 */
int compareAlarms(const void* first, const void* second) {
    Alarm *a1 = (Alarm *)first;
    Alarm *a2 = (Alarm *)second;

    return strcmp(a1->action, a2->action);
}

/*
 */
char* printAlarm(void* toBePrinted) {
    if (!toBePrinted) {
        return NULL;
    }

    Alarm *al = (Alarm *)toBePrinted;

    // Lists have their own print function
    char *props = toString(al->properties);

    int length = strlen(al->action) + strlen(al->trigger) + strlen(props) + 200;
    char *toReturn = malloc(length);

    snprintf(toReturn, length, "Start ALARM {AlarmAction: \"%s\", AlarmTrigger: \"%s\", Start ALARM_PROPERTIES: {%s\n} End ALARM_PROPERTIES} End ALARM", \
             al->action, al->trigger, props);

    // Free dynamically allocated print string
    free(props);

    return realloc(toReturn, strlen(toReturn) + 1);
}


/*
 */
void deleteProperty(void* toBeDeleted) {
    // Properties are alloc'd in one block and none of their
    // members needs to be freed in any special way, so no
    // type casting or calling other functions is necessary
    if (toBeDeleted) {
        free(toBeDeleted);
    }
}

/*
 * Compares the names of two properties, then the values if their names are the same.
 */
int compareProperties(const void* first, const void* second) {
    Property *p1 = (Property *)first;
    Property *p2 = (Property *)second;
	int toReturn;

    if ((toReturn = strcmp(p1->propName, p2->propName)) == 0) {
		toReturn = strcmp(p1->propDescr, p2->propDescr);
	}

	return toReturn;
}

/*
 */
char* printProperty(void* toBePrinted) {
    if (!toBePrinted) {
        return NULL;
    }

    Property *prop = (Property *)toBePrinted;

    int length = strlen(prop->propDescr) + 150;
    char *toReturn = malloc(length);

    snprintf(toReturn, length, "Start PROPERTY {PropName: \"%s\", PropDescr: \"%s\"} End PROPERTY", prop->propName, prop->propDescr);

    return realloc(toReturn, strlen(toReturn) + 1);
}


/*
 */
void deleteDate(void* toBeDeleted) {
    // DateTimes do not have to be dynamically allocated (no structures use DT pointers,
    // and DT's are always a set size), so I'm honestly not entirely sure why this function is even here.
    return;
}

/*
 * Compare dates, then times if the dates are the same, then UTC values
 * if the times are the same as well.
 */
int compareDates(const void* first, const void* second) {
    DateTime *dt1 = (DateTime *)first;
    DateTime *dt2 = (DateTime *)second;
    int cmp;

    // if dates are the same, then compare times instead
    if ((cmp = strcmp(dt1->date, dt2->date)) == 0) {
        // if times are also the same, then compare UTC instead
        if ((cmp = strcmp(dt1->time, dt2->time)) == 0) {
            return dt1->UTC - dt2->UTC;
        } // if times are not the same, then return the time comparison below
    } // if dates are not the same, then return the date comparison below

    return cmp;
}

/*
 */
char* printDate(void* toBePrinted) {
    if (!toBePrinted) {
        return NULL;
    }

    DateTime *dt = (DateTime *)toBePrinted;

    int length = 150;
    char *toReturn = malloc(length);

    snprintf(toReturn, length, "Start DATE_TIME {Date (YYYYMMDD): \"%s\", Time (HHMMSS): \"%s\", UTC?: %s} End DATE_TIME", \
             dt->date, dt->time, (dt->UTC) ? "Yes" : "No");

    return realloc(toReturn, strlen(toReturn) + 1);
}

