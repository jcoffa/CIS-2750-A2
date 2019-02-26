#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "LinkedListAPI.h"
#include "CalendarParser.h"
#include "CalendarHelper.h"

#define NUMTEST 19
#define TESTBUFF 100

#define BR_RED		"\x1b[91m"
#define BR_GREEN	"\x1b[92m"
#define CYAN		"\x1b[36m"
#define RESET		"\x1b[0m"

int main(void)
{
	unsigned int totalMatches = 0;
	const char readPath[] = "/home/joseph/cis2750/a02/TEST/OK/";
	const char writePath[] = "/home/joseph/cis2750/a02/WriteTest/";
    char fileName[TESTBUFF + TESTBUFF];

    char files[NUMTEST][TESTBUFF] =
    {
        "BORIS.ics",
        "EBOLA.ics",
        "escapedNewline.ics",
        "fuck_you.ics",
        "fuckyou.ics",
        "JJNestedMedleySuperBlank.ics",
        "lotsOfLines.ics",
        "mLineProp1.ics",
        "NestedMedley.ics",
        "nocomment.ics",
        "testCalEvtPropAlm.ics",
        "testCalEvtProp.ics",
        "testCalSimpleNoUTC.ics",
        "testCalSimpleUTCComments.ics",
        "ValidFileFolding.ics",
        "ValidFileSimple.ics",
        "VALID-LINEFOLDSCOMMENTS.ics",
        "VALID-LINEFOLDS.ics",
        "writeCalDemo.ics"
    };

    for (int i = 0; i < NUMTEST; i++)
    {

		printf("\n================================================================================\n\n");
        strcpy(fileName, readPath);
        strcat(fileName, files[i]);

		printf("Creating calendar from " CYAN "%s\n\n" RESET, fileName);

        Calendar *calendar;
        /* Calendar allocated */
        ICalErrorCode input = createCalendar(fileName, &calendar);
        /* Error allocated */
        char *firstError = printError(input);

        if (input != OK)
        {
            printf("\nCould not create" CYAN "%s\n" RESET, fileName);
            printf(BR_RED "Error: %s\n" RESET, firstError);
            free(firstError);
            continue;
        } else {
			printf(BR_GREEN "\nRead in " CYAN "\"%s\"" BR_GREEN " successfully!\n" RESET, fileName);
			char *pCal = printCalendar(calendar);
			printf("%s\n\n", pCal);
		}

        char outFile[TESTBUFF + TESTBUFF];
        strcpy(outFile, writePath);
        strcat(outFile, files[i]);

		printf("Writing calendar to " CYAN "%s\n\n" RESET, outFile);

        ICalErrorCode output = writeCalendar(outFile, calendar);
        /* Error allocated */
        char *writeError = printError(output);
        if (output != OK)
        {
            printf("\nCould not write Calendar " CYAN "%s\n" RESET, outFile);
            printf(BR_RED "Error: %s\n" RESET, writeError);
            deleteCalendar(calendar);
            free(firstError);
            free(writeError);
            continue;
        }

		printf("\nSuccessfully wrote the calendar to " CYAN "%s\n" RESET, outFile);

		/*
		FILE *fin = fopen(outFile, "r");
		char line[10000];
		while (fgets(line, 10000, fin)) {
			printf("%s", line);
		}
		printf("\n");
		*/

		printf("Creating the same calendar as before, this time using the written file\n\n");

        Calendar *outputCalendar;
        /* Calendar allocated */
        ICalErrorCode outputInput = createCalendar(outFile, &outputCalendar);
        /* Error allocated */
        char *secondError = printError(outputInput);
        
        if (outputInput != OK)
        {
            printf("\nCould not re-create original calendar using the written file " CYAN "%s\n" RESET, outFile);
            printf(BR_RED "Error: %s\n" RESET, secondError);
            deleteCalendar(calendar);
            free(writeError);
            free(firstError);
            free(secondError);
            continue;
        }

        /* Printstring allocated */
        char *firstString = printCalendar(calendar);
        /* Printstring allocated */
        char *secondString = printCalendar(outputCalendar);

        if (strcmp(firstString, secondString) != 0)
        {
            printf(BR_RED "NO MATCH : %s\n" RESET, fileName);
        }
        else
        {
            printf(BR_GREEN "MATCH : %s\n" RESET, fileName);
			totalMatches++;
        }

        free(writeError);
        free(firstError);
        free(secondError);
        deleteCalendar(calendar);
        deleteCalendar(outputCalendar);

        free(firstString);
        free(secondString);
    }

	printf("\n\nTotal score: %d/%d matches\n", totalMatches, NUMTEST);
    return 0;
}
