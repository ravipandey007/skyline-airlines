#include "skyline.h"
#include <stdio.h>
#include <string.h>

/* Read a whole line, including spaces, and remove its final newline. */
void readLine(const char *prompt, char *buffer, int size) {
    int extraCharacter;
    size_t lineLength;

    printf("%s", prompt);

    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    lineLength = strlen(buffer);

    if (lineLength > 0 && buffer[lineLength - 1] == '\n') {
        buffer[lineLength - 1] = '\0';
    } else {
        /* Discard extra characters so the next prompt starts on a new line. */
        while ((extraCharacter = getchar()) != '\n' &&
               extraCharacter != EOF) {
        }
    }
}

/* Keep asking until the user enters one whole number. */
int readInt(const char *prompt) {
    char inputLine[100];
    char extraCharacter;
    int value;

    while (1) {
        readLine(prompt, inputLine, sizeof(inputLine));

        /* End the input loop cleanly if input is closed, such as Ctrl+Z. */
        if (feof(stdin)) {
            return 0;
        }

        /* The extra character check rejects input such as "12abc". */
        if (sscanf(inputLine, "%d %c", &value, &extraCharacter) == 1) {
            return value;
        }

        printf("Invalid input. Enter a whole number.\n");
    }
}

/* Keep asking until the user enters a non-negative number. */
float readFloat(const char *prompt) {
    char inputLine[100];
    char extraCharacter;
    float value;

    while (1) {
        readLine(prompt, inputLine, sizeof(inputLine));

        if (feof(stdin)) {
            return 0;
        }

        if (sscanf(inputLine, "%f %c", &value, &extraCharacter) == 1
            && value >= 0) {
            return value;
        }

        printf("Enter a valid non-negative amount.\n");
    }
}

/* Wait for Enter before showing the next menu. */
void pauseScreen(void) {
    char input[8];
    readLine("\nPress Enter to continue...", input, sizeof(input));
}
