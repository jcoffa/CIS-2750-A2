/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 27, 2019     *
 *                                  *
 *  Assignment 2, CIS*2750          *
 *  CalendarHelper.c                *
 ************************************/

#include "CalendarHelper.h"

ICalErrorCode writeProperties(FILE *fout, const List *props) {
    if (fout == NULL || props == NULL) {
        return WRITE_ERROR;
    }

    if (getLength(props) == 0) {
        return OK;
    }

    ICalErrorCode err;
    Property *toWrite;
    ListIterator iter = createIterator(props);
    while ((toWrite = (Property *)nextElement(iter)) != NULL) {
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

ICalErrorCode writeEvents(FILE *fout, const List *events) {
    if (fout == NULL || events == NULL) {
        return WRITE_ERROR;
    }

    if (getLength(events) == 0) {
        return OK;
    }

    ICalErrorCode err;
    Event *toWrite;
    ListIterator iter = createIterator(events);
    while ((toWrite = (Event *)nextElement(iter)) != NULL) {
        fprintf(fout, "BEGIN:VEVENT\r\n");
        fprintf(fout, "UID:%s\r\n", toWrite->UID);
        if ((err = writeDateTime(fout, toWrite->creationDateTime)) != OK) {
            return err;
        }
        if ((err = writeDateTime(fout, toWrite->startDateTime)) != OK) {
            return err;
        }
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

ICalErrorCode writeAlarms(FILE *fout, const List *alarms) {
    if (fout == NULL || alarms == NULL) {
        return WRITE_ERROR;
    }

    if (getLength(alarms) == 0) {
        return OK;
    }

    ICalErrorCode err;
    Alarm *toWrite;
    ListIterator iter = createIterator(alarms);
    while ((toWrite = (Alarm *)nextElement(iter)) != NULL) {
        fprintf(fout, "BEGIN:VALARM\r\n");
        fprintf(fout, "ACTION:%s\r\n", toWrite->action);
        fprintf(fout, "TRGIGER:%s\r\n", toWrite->trigger);
        if ((err = writeProperties(fout, alarms->properties)) != OK) {
            return err;
        }
        fprintf(fout, "END:VALARM\r\n");
    }

    return OK;
}

ICalErrorCode writeDateTime(FILE *fout, DateTime dt) {
    if (fout == NULL) {
        return WRITE_ERROR;
    }

    fprintf(fout, "%sT%s", dt.date, dt.time);
    fprintf(fout, "%s", (dt.UTC) ? "Z\r\n" : "\r\n");

    return OK;
}

