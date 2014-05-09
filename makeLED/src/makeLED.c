/*
 ============================================================================
 Name        : makeLED.c
 Author      : Derek Molloy
 Edited By   : mr-augustine
 Description : Simple On-board LED flashing program

 This program uses USR LED 0 and can be executed in five ways:
 	 makeLED on
 	 makeLED off
 	 makeLED flash (flash at 100ms intervals - on 50ms/off 50ms)
 	 makeLED heartbeat (return the LED to its original purpose)
 	 makeLED status (get the trigger status)

 The makeLED program was originally created by Derek Molloy in C++ and was
 converted to a C language implementation. For more details visit:
 http://derekmolloy.ie/beaglebone-controlling-the-on-board-leds-using-c/
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LED0_PATH "/sys/class/leds/beaglebone:green:usr0"

void removeTrigger(void);

int main(int argc, char * argv[]) {
	if (argc != 2) {
		printf("Usage: makeLed <command>\n");
		printf("commands: {on, off, flash, status}\n");
		printf("e.g., makeLed on\n");
		return EXIT_FAILURE;
	}

	FILE * ledFileP = NULL;
	char userCommand[10];

	strcpy(userCommand, argv[1]);

	printf("Starting the LED flash program\n");
	printf("The LED Path is: %s\n", LED0_PATH);

	// for each user command, the program reads or writes data to a file in
	// the virtual file system
	if (strcmp(userCommand, "on") == 0) {
		removeTrigger();

		if ((ledFileP = fopen(LED0_PATH "/brightness","w")) != NULL) {
			fprintf(ledFileP, "1");
			fclose(ledFileP);
		} else {
			printf("Unable to open LED:brightness file\n");
			return EXIT_FAILURE;
		}
	} else if (strcmp(userCommand, "off") == 0) {
		removeTrigger();

		if ((ledFileP = fopen(LED0_PATH "/brightness","w")) != NULL) {
			fprintf(ledFileP, "0");
			fclose(ledFileP);
		} else {
			printf("Unable to open LED:brightness file\n");
			return EXIT_FAILURE;
		}
	} else if (strcmp(userCommand, "flash") == 0) {
		/* 1 - setup the time*/
		if ((ledFileP = fopen(LED0_PATH "/trigger","w")) != NULL) {
			fprintf(ledFileP, "timer");
			fclose(ledFileP);
		} else {
			printf("Unable to open LED:trigger file\n");
			return EXIT_FAILURE;
		}

		/* 2 - set the LED-on duration */
		if ((ledFileP = fopen(LED0_PATH "/delay_on","w")) != NULL) {
			fprintf(ledFileP, "50");
			fclose(ledFileP);
		} else {
			printf("Unable to open LED:delay_on file\n");
			return EXIT_FAILURE;
		}

		/* 3 - set the LED-off duration */
		if ((ledFileP = fopen(LED0_PATH "/delay_off","w")) != NULL) {
			fprintf(ledFileP, "50");
			fclose(ledFileP);
		} else {
			printf("Unable to open LED:delay_off file\n");
			return EXIT_FAILURE;
		}
	} else if (strcmp(userCommand, "heartbeat") == 0) {
		if ((ledFileP = fopen(LED0_PATH "/trigger","w")) != NULL) {
			fprintf(ledFileP, "heartbeat");
			fclose(ledFileP);
		} else {
			printf("Unable to open LED:trigger file\n");
			return EXIT_FAILURE;
		}
	}
	else if (strcmp(userCommand, "status") == 0) {
		if ((ledFileP = fopen(LED0_PATH "/trigger","r")) != NULL) {
			char line[256];
			while(fscanf(ledFileP,"%s", line) == 1) {
				printf("%s ", line);
			}
			printf("\n");

			fclose(ledFileP);
		} else {
			printf("Unable to open LED:trigger file\n");
			return EXIT_FAILURE;
		}
	} else {
		printf("Invalid command\n");
	}

	printf("Finished the LED flash program\n");

	return 0;
}

void removeTrigger(void) {
	FILE * ledFileP;

	// remove the trigger from the LED
	if ((ledFileP = fopen(LED0_PATH "/trigger","w")) != NULL) {
		fprintf(ledFileP, "none");
		fclose(ledFileP);
	} else {
		printf("Unable to open LED:trigger file\n");
	}

	return;
}
