/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 27, 2019     *
 *                                  *
 *  Assignment 2, CIS*2750          *
 *  main.c                          *
 ************************************/

#include "CalendarParser.h"
#include "CalendarHelper.h"
#include "LinkedListAPI.h"
#include "Parsing.h"
#include "Initialize.h"
#include "Debug.h"

#define NUM_FILES 89

int main() {
    const char readPath[] = "/home/joseph/cis2750/a02/TEST/";
    const char writePath[] = "/home/joseph/cis2750/a02/WriteTest/";
	const char *files[NUM_FILES] = {"AIDS/FAMINE.ics",
                                "AIDS/LOSS.ics",
                                "AIDS/PESTILENCE.ics",
                                "AIDS/PLAGUE.ics",
                                "AIDS/WAR.ics",
                                "DUP_PRODID/DupPRODID1.ics",
                                "DUP_PRODID/DupPRODID2.ics",
                                "DUP_PRODID/DupPRODID3.ics",
                                "DUP_VER/DupVersion1.ics",
                                "DUP_VER/DupVersion2.ics",
                                "DUP_VER/DupVersion3.ics",
                                "INV_ALARM/invOpeningTag2.ics",
                                "INV_ALARM/MissingAction.ics",
                                "INV_ALARM/MissingClosingTag.ics",
                                "INV_ALARM/MissingClosingTag2.ics",
                                "INV_ALARM/MissingClosingTag3.ics",
                                "INV_ALARM/MissingTrigger.ics",
                                "INV_ALARM/NullAction.ics",
                                "INV_ALARM/NullTrigger.ics",
                                "INV_CAL/BadLine.ics",
                                "INV_CAL/blankLine.ics",
                                "INV_CAL/FUKU.ics",
                                "INV_CAL/INV_CAL-wrongendtag.ics",
                                "INV_CAL/invClosingTag1.ics",
                                "INV_CAL/invClosingTag3.ics",
                                "INV_CAL/invOpeningTag3.ics",
                                "INV_CAL/MissingEndCalendar.ics",
                                "INV_CAL/MissingEvent.ics",
                                "INV_CAL/MissingEvent2.ics",
                                "INV_CAL/MissingOpeningTag.ics",
                                "INV_CAL/MissingOpeningTag2.ics",
                                "INV_CAL/MissingOpeningTag3.ics",
                                "INV_CAL/MissingPRODID.ics",
                                "INV_CAL/MissingPRODID2.ics",
                                "INV_CAL/MissingVER.ics",
                                "INV_CAL/MissingVER2.ics",
                                "INV_DT/InvalidCreationDate.ics",
                                "INV_DT/InvalidStartDate.ics",
                                "INV_DT/MalformedCreationDate.ics",
                                "INV_DT/MalformedStartDate.ics",
                                "INV_EVENT/invClosingTag2.ics",
                                "INV_EVENT/INV_EVENT-blankUID.ics",
                                "INV_EVENT/INV_EVENT-DTENDandDURA.ics",
                                "INV_EVENT/INV_EVENT-dupURL.ics",
                                "INV_EVENT/invOpeningTag1.ics",
                                "INV_EVENT/MalformedUID.ics",
                                "INV_EVENT/MissingClosingTag.ics",
                                "INV_EVENT/MissingClosingTag2.ics",
                                "INV_EVENT/MissingClosingTag3.ics",
                                "INV_EVENT/MissingCreationDate.ics",
                                "INV_EVENT/MissingStartDate.ics",
                                "INV_EVENT/MissingUID.ics",
                                "INV_EVENT/MissingUID2.ics",
                                "INV_FILE/BadFileName.txt",
                                "INV_FILE/BadFileName2.ical",
                                "INV_FILE/BadFileName3.icss",
                                "INV_FILE/EmptyString.ics",
                                "INV_FILE/invalidEndings.ics",
                                "INV_FILE/InvalidLineEndings.ics",
                                "INV_FILE/INV_FILE-missingCRLF.ics",
                                "INV_FILE/MisspellllllThisName.ics",
                                "INV_PRODID/NullPRODID1.ics",
                                "INV_PRODID/NullPRODID2.ics",
                                "INV_PRODID/NullPRODID3.ics",
                                "INV_VER/INV_VERSION1.ics",
                                "INV_VER/INV_VERSION2.ics",
                                "INV_VER/MalformedVersion1.ics",
                                "INV_VER/MalformedVersion2.ics",
                                "INV_VER/NullVersion.ics",
                                "OK/cis2170.ics",
                                "OK/complexUnfolding.ics",
                                "OK/EBOLA.ics",
                                "OK/fuck_you.ics",
                                "OK/fuckyou.ics",
                                "OK/googleCalendarTest.ics",
                                "OK/invite.ics",
                                "OK/mLineProp1.ics",
                                "OK/NestedMedley.ics",
                                "OK/newCalendar.ics",
                                "OK/properties.ics",
                                "OK/testCalEvtPropAlm.ics",
                                "OK/testCalEvtProp.ics",
                                "OK/testCalSimpleNoUTC.ics",
                                "OK/testCalSimpleUTCComments.ics",
                                "OK/ValidFileFolding.ics",
                                "OK/ValidFileSimple.ics",
                                "OK/VALID-LINEFOLDSCOMMENTS.ics",
                                "OK/VALID-LINEFOLDS.ics",
                                "OK/validation.ics"};

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
    char *printCal, *printErrorCode;

    // FIXME This is a list of files that give memory errors FIXME
    // None!
	


    Calendar *cal;
    int i = 0;
    int written = 0;
    ICalErrorCode createErr, validateErr, writeErr;
    int choice;
    char pathToFile[1000];

    for (i = 0; i < NUM_FILES; i++) {
        printf("%d - %s\n", i, files[i]);
    }

    printf("\nWhich calendar would you like to test? [0-%d, anything else for all]: ", NUM_FILES-1);
    scanf("%d", &choice);

    if (choice >= 0 && choice < NUM_FILES) {
		//
		// A specific calendar from the readPath has been chosen
		//
        strcpy(pathToFile, readPath);
        strcat(pathToFile, files[choice]);
        printf("\n\n\n==========================================================================================\n");
        printf("%s\n", pathToFile);
        printf("==========================================================================================\n");
        createErr = createCalendar(pathToFile, &cal);

        if (createErr != OK) {
			// Error occurred while reading calendar
            printErrorCode = printError(createErr);
            printf("\nAn error occurred: %s\n", printErrorCode);
            free(printErrorCode);
        } else {
			// Calendar was created successfully
			printf(BR_GREEN "\nCalendar created successfully!" RESET " Printing...\n");

            printCal = printCalendar(cal);
            printf("------------------------------------------------------------------------------------------\n");
            printf("\"%s\"\n", printCal);
            free(printCal);

			// Run the calendar through 'validateCalendar'
			validateErr = validateCalendar(cal);
			char *printValidateErr = printError(validateErr);
			printErrorCode = printError(createErr);
			printf("\nvalidateCalendar() return Code: %s\n", printValidateErr);
			free(printValidateErr);

			// Write the calendar object that was created
            strcpy(pathToFile, writePath);
			strcat(pathToFile, (files[choice]) + strcspn(files[choice], "/") + 1);
            printf("\nWriting Calendar to \"%s\"\n", pathToFile);
            if ((writeErr = writeCalendar(pathToFile, cal)) != OK) {
                printErrorCode = printError(writeErr);
                printf("\nAn error occurred: %s\n", printErrorCode);
                free(printErrorCode);
            } else {
                printf(BR_GREEN "\nWrote \"%s\" successfully!\n" RESET, pathToFile);
            }

			// Convert the calendar object into a JSON string
			printf("\nWriting Calendar and its Event List to JSON strings\n");
			char *calJSON = calendarToJSON(cal);
			char *evtListJSON = eventListToJSON(cal->events);

			printf("\nCalendar JSON: %s\n\nEvent List JSON: %s\n", calJSON, evtListJSON);
			free(calJSON);
			free(evtListJSON);

			// Delete the calendar
            deleteCalendar(cal);
        }
    } else {
		//
		// Every calendar from the readPath will execute sequentially
		//
        for (i = 0; i < NUM_FILES; i++) {
            strcpy(pathToFile, readPath);
            strcat(pathToFile, files[i]);
            printf("\n\n\n==========================================================================================\n");
            printf("Index %d - %s\n", i, pathToFile);
            printf("==========================================================================================\n");
            createErr = createCalendar(pathToFile, &cal);

            if (createErr != OK) {
				// Error occurred while reading calendar
                printErrorCode = printError(createErr);
                printf("\nAn error occurred: %s\n", printErrorCode);
                free(printErrorCode);
            } else {
				// Calendar was created successfully
				printf(BR_GREEN "\nCalendar created successfully!" RESET " Printing...\n");
                printCal = printCalendar(cal);
				printf("------------------------------------------------------------------------------------------\n");
				printf("\"%s\"\n", printCal);

                free(printCal);

				// Run the calendar through 'validateCalendar'
				validateErr = validateCalendar(cal);
				char *printValidateErr = printError(validateErr);
				printf("\nvalidateCalendar() return Code: %s\n", printValidateErr);
				free(printValidateErr);

				// Write the calendar object that was created
                strcpy(pathToFile, writePath);
                strcat(pathToFile, (files[i]) + strcspn(files[i], "/") + 1);
                printf("\nWriting Calendar to \"%s\"\n", pathToFile);
                if ((writeErr = writeCalendar(pathToFile, cal)) != OK) {
                    printErrorCode = printError(writeErr);
                    printf("\nAn error occurred: %s\n", printErrorCode);
                    free(printErrorCode);
                } else {
                    printf("\nWrote \"%s\" successfully\n", pathToFile);
                    written++;
                }   

				// Convert the calendar object into a JSON string
				printf("\nWriting Calendar and its Event List to JSON strings\n");
				char *calJSON = calendarToJSON(cal);
				char *evtListJSON = eventListToJSON(cal->events);

				printf("Calendar JSON: %s\nEvent List JSON: %s\n", calJSON, evtListJSON);
				free(calJSON);
				free(evtListJSON);

				// Delete the calendar
                deleteCalendar(cal);
            }
        }
        printf("\n\nRan through %d calendars.\nSuccessfully wrote %d of them.\n", i, written);
    }





	/*
	printf("\n----------DEBUG.H----------\n");
	debugMsg("This is a test of a debug where this string is the only parameter\n");
	errorMsg("This is a test of an error where this string is the only parameter\n");

	debugMsg("This is a test of a debug which has one (%d) parameter\n", 1);
	errorMsg("This is a test of an error which has one (%d) parameter\n", 1);

	debugMsg("This is a test of a dbeug which has %s (%d) paramters\n", "two", 2);
	errorMsg("This is a test of an error which has %s (%d) parameters\n", "two", 2);

	debugMsg("%s %s %s %c%d%c %s\n", "This is a", "test of a debug", "which has seven", '(', 7, ')', "parameters");
	errorMsg("%s %s %s %c%d%c %s\n", "This is a", "test of an error", "which has seven", '(', 7, ')', "parameters");
	*/




	/*
	printf("\n----------EQUALSONEOFSTR----------\n");
	const char *animals[] = {"dog", "cat", "elephant", "snail"};

	printf("Does \"dog\" equal one of\t\t[\"dog\", \"cat\", \"elephant\", \"snail\"]? : %d\n", equalsOneOfStr("dog", 4, animals));
	printf("Does \"cat\" equal one of\t\t[\"dog\", \"cat\", \"elephant\", \"snail\"]? : %d\n", equalsOneOfStr("cat", 4, animals));
	printf("Does \"elephant\" equal one of\t[\"dog\", \"cat\", \"elephant\", \"snail\"]? : %d\n", equalsOneOfStr("elephant", 4, animals));
	printf("Does \"snail\" equal one of\t[\"dog\", \"cat\", \"elephant\", \"snail\"]? : %d\n", equalsOneOfStr("snail", 4, animals));
	printf("Does \"monkey\" equal one of\t[\"dog\", \"cat\", \"elephant\", \"snail\"]? : %d\n", equalsOneOfStr("monkey", 4, animals));
	*/




	/*
	printf("\n----------VEQUALSONEOFSTR----------\n");
	printf("Does \"dog\" equal one of\t\t[\"dog\", \"cat\", \"elephant\", \"snail\"]? : %d\n", vequalsOneOfStr("dog", 4, "dog", "cat", "elephant", "snail"));
	printf("Does \"cat\" equal one of\t\t[\"dog\", \"cat\", \"elephant\", \"snail\"]? : %d\n", vequalsOneOfStr("cat", 4, "dog", "cat", "elephant", "snail"));
	printf("Does \"elephant\" equal one of\t[\"dog\", \"cat\", \"elephant\", \"snail\"]? : %d\n", vequalsOneOfStr("elephant", 4, "dog", "cat", "elephant", "snail"));
	printf("Does \"snail\" equal one of\t[\"dog\", \"cat\", \"elephant\", \"snail\"]? : %d\n", vequalsOneOfStr("snail", 4, "dog", "cat", "elephant", "snail"));
	printf("Does \"monkey\" equal one of\t[\"dog\", \"cat\", \"elephant\", \"snail\"]? : %d\n", vequalsOneOfStr("monkey", 4, "dog", "cat", "elephant", "snail"));
	*/
	




	/*
	printf("\n----------__TOJSON----------\n");
	Calendar *JSONcal;
	ICalErrorCode createJSONerr = createCalendar("/home/joseph/cis2750/a02/TEST/OK/json.ics", &JSONcal);

	if (createJSONerr == OK) {
		char *calJ = calendarToJSON(JSONcal);
		char *evtJ = eventListToJSON(JSONcal->events);
		char *dtJ = dtToJSON(((Event *)(JSONcal->events->head->data))->startDateTime);

		printf("\nCal JSON: %s\n\nEvent List JSON: %s\n\nDTSTART of first event in list JSON: %s\n", \
		       calJ, evtJ, dtJ);

		free(calJ);
		free(evtJ);
		free(dtJ);
	} else {
		printf("createCalendar() failed\n");
	}
	*/

    return EXIT_SUCCESS;
}

