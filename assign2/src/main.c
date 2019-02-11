/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 27, 2019     *
 *                                  *
 *  Assignment 2, CIS*2750          *
 *  main.c                          *
 ************************************/

#include "CalendarParser.h"
#include "LinkedListAPI.h"
#include "Parsing.h"
#include "Initialize.h"

#define NUM_FILES 88

int main() {

    /*
    printf("----------WHITESPACE TRIMMING----------\n");
    char s1[] = "   This has spaces.   ";
    char s2[] = "\tThis has a tab at the front, and also\there and after this\t";
    char s3[] = "           This has a mix of\tboth\t!    ";

    printf("\"%s\" -> \"%s\"\n", s1, trimWhitespace(s1));
    printf("\"%s\" -> \"%s\"\n", s2, trimWhitespace(s2));
    printf("\"%s\" -> \"%s\"\n", s3, trimWhitespace(s3));
    */





    /*
    printf("\n----------CONCATENATION----------\n");
    char concatenated[200] = "";
    concat(concatenated, "This is the start of a concatenated string."); //  43
    printf("\"%s\"\n", concatenated);

    concat(concatenated, "But what if I add more to it?");  // 29
    printf("\"%s\"\n", concatenated);

    concat(concatenated, " You fool! You must add in the space yourself!"); // 46
    printf("\"%s\"\n", concatenated);
    */





    /*
    printf("\n----------UNFOLDING----------\n");
    char fold1[] = "This line ha\r\n s some folds in i\r\n t!\r\n";
    char fold2[] = "This line doesn't have any whatsoever.";
    char fold3[] = "This one has\r\n  a fold immediately preceding a space! Tricky...\r\n";
    char fold4[] = "Thi\r\n s one has lots\r\n  of folds \r\n in it!\r\n";

    printf("FOLDED:\n\"%s\"\n", fold1);
    unfold(fold1);
    printf("UNFOLDED: \"%s\"\n", fold1);
    printf("~~~~~~~~~~\n");
   
    printf("FOLDED:\n\"%s\"\n", fold2);
    unfold(fold2);
    printf("UNFOLDED: \"%s\"\n", fold2);
    printf("~~~~~~~~~~\n");

    printf("FOLDED:\n\"%s\"\n", fold3);
    unfold(fold3);
    printf("UNFOLDED: \"%s\"\n", fold3);
    printf("~~~~~~~~~~\n");

    printf("FOLDED:\n\"%s\"\n", fold4);
    unfold(fold4);
    printf("UNFOLDED: \"%s\"\n", fold4);
    */





    /*
    printf("\n----------UNFOLDING AN ICAL PROPERTY ACROSS MULTIPLE LINES----------\n");
    FILE *desc1 = fopen("/home/joseph/cis2750/a01/testFiles/t1.ics", "r");
    FILE *desc2 = fopen("/home/joseph/cis2750/a01/testFiles/t2.ics", "r");
    FILE *desc3 = fopen("/home/joseph/cis2750/a01/testFiles/t3.ics", "r");
    FILE *desc4 = fopen("/home/joseph/cis2750/a01/testFiles/t4.ics", "r");
    char buffer[80];
    char toPrint[200] = "";

    printf("~~~~~~~~~~ NEW FILE ~~~~~~~~~\n");
    if (desc1) {
        while (fgets(buffer, 79, desc1)) {
            printf("%s", buffer);
            concat(toPrint, buffer);
        }
        printf("CONCATENATED: %s\n", toPrint);
        unfold(toPrint);
        printf("UNFOLDED: \"%s\"\n", toPrint);
        strcpy(toPrint, "");
        fclose(desc1);
    } else {
        printf("Opening desc1 failed\n");
    }


    printf("\n~~~~~~~~~~ NEW FILE ~~~~~~~~~\n");
    if (desc2) {
        while (fgets(buffer, 79, desc2)) {
            printf("%s", buffer);
            concat(toPrint, buffer);
        }
        printf("CONCATENATED: %s\n", toPrint);
        unfold(toPrint);
        printf("UNFOLDED: \"%s\"\n", toPrint);
        strcpy(toPrint, "");
        fclose(desc2);
    } else {
        printf("Opening desc2 failed\n");
    }


    printf("\n~~~~~~~~~~ NEW FILE ~~~~~~~~~\n");
    if (desc3) {
        while (fgets(buffer, 79, desc3)) {
            printf("%s", buffer);
            concat(toPrint, buffer);
        }
        printf("CONCATENATED: %s\n", toPrint);
        unfold(toPrint);
        printf("UNFOLDED: \"%s\"\n", toPrint);
        strcpy(toPrint, "");
        fclose(desc3);
    } else {
        printf("Opening desc3 failed\n");
    }
    */


    /*
    printf("\n~~~~~~~~~~ NEW FILE ~~~~~~~~~\n");
    if (desc4) {
        while (fgets(buffer, 79, desc4)) {
            printf("%s", buffer);
            concat(toPrint, buffer);
        }
        printf("CONCATENATED: %s\n", toPrint);
        unfold(toPrint);
        printf("UNFOLDED: \"%s\"\n", toPrint);
        strcpy(toPrint, "");
        fclose(desc4);
    } else {
        printf("Opening desc4 failed\n");
    }
    */
    



    /*
    printf("\n----------CREATE CALENDAR----------\n");
    Calendar *newCalendar;
    ICalErrorCode error = createCalendar("/home/joseph/cis2750/a01/testFiles/newCalendar.ics", &newCalendar);
    if (error == OK) {
        char *humanReadable = printCalendar(newCalendar);
        printf("\nCalendar created from file 'newCalendar.ics':\"%s\"\n", humanReadable);
        free(humanReadable);

        deleteCalendar(newCalendar);
    } else {
        char *errorStr = printError(error);
        printf("Something went wrong when reading \"newCalendar.ics\": %s\n", errorStr);
        free(errorStr);
    }
    */
    




    /*
    printf("\n----------STARTS WITH----------\n");
    printf("\"DESCRIPTION:Hi!\" and \"DESCRIPTION:\" = %d\n", startsWith("DESCRIPTION:Hi!", "DESCRIPTION:"));
    printf("\"PRODID:-//Google,...\" and \"PRODID:\" = %d\n", startsWith("PRODID:-//Google,...", "PRODID:"));
    printf("\"Nope\" and \"VERSION:\" = %d\n", startsWith("Nope", "VERSION:"));
    printf("\"Not this one either\" and \"VERSION:\" = %d\n", startsWith("Not this one either", "VERSION:"));
    printf("\"VERSION=2.0\" and \"VERSION:\" = %d\n", startsWith("VERSION=2.0", "VERSION:"));
    printf("\"VERSION=2.0\" and \"\" = %d\n", startsWith("VERSION=2.0", ""));
    */





    /*
    printf("\n----------ENDS WITH----------\n");
    printf("\"This is a test\" and \"t\" = %d\n", endsWith("This is a test", "t"));
    printf("\"This is a test\" and \"test\" = %d\n", endsWith("This is a test", "test"));
    printf("\"This is a test\" and \"q\" = %d\n", endsWith("This is a test", "q"));
    printf("\"This is a test\" and \"But this one is much much longer\" = %d\n", endsWith("This is a test", "But this one is much much longer"));
    printf("\"This is a test\" and \"\" = %d\n", endsWith("This is a test", ""));
    */





    /*
    printf("\n----------TOUPPER FUNCTIONS----------\n");
    char randCase[] = "ThiS CaSE is RandoM. HOPEFUllY thE FuncTioN WorKS ThIS TImE!";
    printf("Before: \"%s\"\n", randCase);
    strUpper(randCase);
    printf("After: \"%s\"\n", randCase);

    char randCase2[] = "But WhaT If we MalloC() the StriNg insTEAD?";
    char *returnedRandCase = strUpperCopy(randCase2);
    printf("\nrandCase2: \"%s\"\nreturnedRandCase: \"%s\"", randCase2, returnedRandCase);
    free(returnedRandCase);
    */





    /*
    printf("\n\n\n----------FILE PARSE UNFOLDING----------\n");
    FILE *testUnfold = fopen("/home/joseph/cis2750/a01/testFiles/unfolding.ics", "r");
    char unfolded[2000];
    while (!feof(testUnfold)) {
        readFold(unfolded, 2000, testUnfold);
        printf("Unfolded line: \"%s\"\n", unfolded);
    }

    printf("\n");

    FILE *multLineProp = fopen("/home/joseph/cis2750/a01/testFiles/mLineProp1.ics", "r");
    while (!feof(multLineProp)) {
        readFold(unfolded, 2000 ,multLineProp);
        printf("Unfolded line: \"%s\"\n", unfolded);
    }

    printf("\n");

    FILE *testUnfold2 = fopen("/home/joseph/cis2750/a01/testFiles/newCalendar.ics", "r");
    while (!feof(testUnfold2)) {
        readFold(unfolded, 2000, testUnfold2);
        printf("Unfolded line: \"%s\"\n", unfolded);
    }

    fclose(testUnfold);
    fclose(multLineProp);
    fclose(testUnfold2);
    */





    /*
    printf("\n\n\n----------PROPERTY CREATION----------\n");
    char testProperty1[] = "DESCRIPTION:This is a test\r\n";
    Property *testProp = newProperty(testProperty1);
    char *printProp1 = printProperty(testProp);
    printf("Property: \"%s\"\n", printProp1);

    char testProperty2[] = "METHOD:Testmethod";
    Property *testProp2 = newProperty(testProperty2);
    char *printProp2 = printProperty(testProp2);
    printf("Property: \"%s\"\n", printProp2);

    free(printProp1);
    free(printProp2);

    free(testProp);
    free(testProp2);
    */





    /*
    printf("\n\n\n----------INITIALIZE.C TESTING----------\n");
    printf("----------DATE TIME----------\n");
    char dtD1[] = "DTSTART:19990305T150722";
    DateTime dt1;
    initializeDateTime(dtD1, &dt1);
    printf("\n");
    char dtD2[] = "DTSTART:20180122T003045Z";
    DateTime dt2;
    initializeDateTime(dtD2, &dt2);
    char dtD3[] = "DTSTART:20101231T235959";
    DateTime dt3;
    printf("\n");
    initializeDateTime(dtD3, &dt3);
    char dtD4[] = "DTAAAAASTARTLOL";
    DateTime dt4;
    initializeDateTime(dtD4, &dt4);

    char *printDT = printDate((void *)&dt1);
    printf("dt1: \"%s\"\n", printDT);
    free(printDT);
    printDT = printDate((void *)&dt2);
    printf("dt2: \"%s\"\n", printDT);
    free(printDT);
    printDT = printDate((void *)&dt3);
    printf("dt3: \"%s\"\n", printDT);
    free(printDT);
    */





    /*
    printf("\n----------PROPERTY----------\n");
    char pd1[] = "ORGANIZER;CN=Joseph Coffa:mailto:example@webmail.ca";
    char pd2[] = "CALSCALE:Gregorian";
    char pd3[] = "DESCRIPTION:This is a sample description";
    char pd4[] = "PRODID:-//Google Inc//Google Calendar 70.9054//EN";
    Property *prop1 = initializeProperty(pd1);
    Property *prop2 = initializeProperty(pd2);
    Property *prop3 = initializeProperty(pd3);
    Property *prop4 = initializeProperty(pd4);

    char *printP = printProperty((void *)prop1);
    printf("prop1: \"%s\"\n", printP);
    free(printP);
    printP = printProperty((void *)prop2);
    printf("prop2: \"%s\"\n", printP);
    free(printP);
    printP = printProperty((void *)prop3);
    printf("prop3: \"%s\"\n", printP);
    free(printP);
    printP = printProperty((void *)prop4);
    printf("prop4: \"%s\"\n", printP);
    free(printP);
    */





    /*
    printf("\n----------ALARM----------\n");
    char action[] = "This is a test action";
    char trigger[] = "This is a test trigger";
    Alarm *alm = initializeAlarm();
    insertFront(alm->properties, (void *)prop1);
    insertFront(alm->properties, (void *)prop2);
    insertFront(alm->properties, (void *)prop3);
    insertFront(alm->properties, (void *)prop4);

    strcpy(alm->action, action);
    alm->trigger = malloc(strlen(trigger) + 1);
    strcpy(alm->trigger, trigger);

    char *printA = printAlarm((void *)alm);
    printf("Alarm with 4 properties: \"%s\"\n", printA);
    free(printA);
    */





    /*
    printf("\n----------EVENT----------\n");
    char uid[] = "This is a test UID";
    Event *event = initializeEvent();
    event->creationDateTime = dt1;
    event->startDateTime = dt2;
    strcpy(event->UID, uid);
    insertFront(event->alarms, alm);
    
    char *printE = printEvent((void *)event);
    printf("Event with 2 date times and 1 alarm (with 4 properties): \"%s\"\n", printE);
    free(printE);
    */





    /*
    printf("\n----------CALENDAR----------\n");
    Calendar *calendar = initializeCalendar();
    insertFront(calendar->events, event);

    char *printC = printCalendar((void *)calendar);
    printf("Calendar with 1 event (with 2 date times and 1 alarm (with 4 properties)): \"%s\"\n", printC);
    free(printC);

    // This should delete all of the stuff (hopefully?)
    deleteCalendar(calendar);
    */





    printf("\n\n\n----------CALENDAR CREATION----------\n");
    char *printCal, *printErr;

    // FIXME This is a list of files that give memory errors FIXME
    // None!
    char *files[NUM_FILES+1] = {"/home/joseph/cis2750/a01/TEST/AIDS/FAMINE.ics",
                                "/home/joseph/cis2750/a01/TEST/AIDS/LOSS.ics",
                                "/home/joseph/cis2750/a01/TEST/AIDS/PESTILENCE.ics",
                                "/home/joseph/cis2750/a01/TEST/AIDS/PLAGUE.ics",
                                "/home/joseph/cis2750/a01/TEST/AIDS/WAR.ics",
                                "/home/joseph/cis2750/a01/TEST/DUP_PRODID/DupPRODID1.ics",
                                "/home/joseph/cis2750/a01/TEST/DUP_PRODID/DupPRODID2.ics",
                                "/home/joseph/cis2750/a01/TEST/DUP_PRODID/DupPRODID3.ics",
                                "/home/joseph/cis2750/a01/TEST/DUP_VER/DupVersion1.ics",
                                "/home/joseph/cis2750/a01/TEST/DUP_VER/DupVersion2.ics",
                                "/home/joseph/cis2750/a01/TEST/DUP_VER/DupVersion3.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_ALARM/invOpeningTag2.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_ALARM/MissingAction.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_ALARM/MissingClosingTag.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_ALARM/MissingClosingTag2.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_ALARM/MissingClosingTag3.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_ALARM/MissingTrigger.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_ALARM/NullAction.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_ALARM/NullTrigger.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/BadLine.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/blankLine.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/FUKU.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/INV_CAL-wrongendtag.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/invClosingTag1.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/invClosingTag3.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/invOpeningTag3.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/MissingEndCalendar.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/MissingEvent.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/MissingEvent2.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/MissingOpeningTag.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/MissingOpeningTag2.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/MissingOpeningTag3.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/MissingPRODID.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/MissingPRODID2.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/MissingVER.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_CAL/MissingVER2.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_DT/InvalidCreationDate.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_DT/InvalidStartDate.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_DT/MalformedCreationDate.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_DT/MalformedStartDate.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_EVENT/invClosingTag2.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_EVENT/INV_EVENT-blankUID.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_EVENT/INV_EVENT-DTENDandDURA.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_EVENT/INV_EVENT-dupURL.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_EVENT/invOpeningTag1.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_EVENT/MalformedUID.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_EVENT/MissingClosingTag.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_EVENT/MissingClosingTag2.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_EVENT/MissingClosingTag3.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_EVENT/MissingCreationDate.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_EVENT/MissingStartDate.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_EVENT/MissingUID.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_EVENT/MissingUID2.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_FILE/BadFileName.txt",
                                "/home/joseph/cis2750/a01/TEST/INV_FILE/BadFileName2.ical",
                                "/home/joseph/cis2750/a01/TEST/INV_FILE/BadFileName3.icss",
                                "/home/joseph/cis2750/a01/TEST/INV_FILE/EmptyString.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_FILE/invalidEndings.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_FILE/InvalidLineEndings.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_FILE/INV_FILE-missingCRLF.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_FILE/MisspellllllThisName.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_PRODID/NullPRODID1.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_PRODID/NullPRODID2.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_PRODID/NullPRODID3.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_VER/INV_VERSION1.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_VER/INV_VERSION2.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_VER/MalformedVersion1.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_VER/MalformedVersion2.ics",
                                "/home/joseph/cis2750/a01/TEST/INV_VER/NullVersion.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/cis2170.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/complexUnfolding.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/EBOLA.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/fuck_you.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/fuckyou.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/googleCalendarTest.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/invite.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/mLineProp1.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/NestedMedley.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/newCalendar.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/properties.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/testCalEvtPropAlm.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/testCalEvtProp.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/testCalSimpleNoUTC.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/testCalSimpleUTCComments.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/ValidFileFolding.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/ValidFileSimple.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/VALID-LINEFOLDSCOMMENTS.ics",
                                "/home/joseph/cis2750/a01/TEST/OK/VALID-LINEFOLDS.ics",
                                NULL};
    Calendar *cal;
    int i = 0;
    ICalErrorCode err;
    int choice;

    printf("Which calendar would you like to test? [0-%d, anything else for all]: ", NUM_FILES-1);
    scanf("%d", &choice);

    if (choice >= 0 && choice < NUM_FILES) {
        printf("\n\n\n==========================================================================================\n");
        printf("%s\n", files[choice]);
        printf("==========================================================================================\n");
        err = createCalendar(files[choice], &cal);

        if (err != OK) {
            printErr = printError(err);
            printf("\nAn error occurred: %s\n", printErr);
            free(printErr);
        } else {
            printCal = printCalendar(cal);
            printf("\nPrinting Calendar from %s:\n", files[choice]);
            printf("------------------------------------------------------------------------------------------\n");
            printf("\"%s\"\n", printCal);
            free(printCal);
            deleteCalendar(cal);
        }
    } else {
        while (files[i] != NULL) {
            printf("\n\n\n==========================================================================================\n");
            printf("Index %d - %s\n", i, files[i]);
            printf("==========================================================================================\n");
            err = createCalendar(files[i], &cal);

            if (err != OK) {
                printErr = printError(err);
                printf("\nAn error occurred: %s\n", printErr);
                free(printErr);
            } else {
                printCal = printCalendar(cal);
                printf("\nPrinting Calendar from %s:\n\"%s\"\n", files[i], printCal);
                free(printCal);
                deleteCalendar(cal);
            }

            i++;
        }
        printf("\n\nRan through %d calendars.\n", i);
    }





    return EXIT_SUCCESS;
}

