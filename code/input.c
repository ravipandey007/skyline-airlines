#include "skyline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================= INPUT FUNCTIONS ================= */

void readLine(const char *prompt, char *buffer, int size) {
    int ch;
    size_t len;

    printf("%s", prompt);

    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        /* Discard the rest of an overlong line so it won't affect the next prompt. */
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }
}

int readInt(const char *prompt) {
    char input[100];
    char extra;
    int value;

    while (1) {
        readLine(prompt, input, sizeof(input));

        /* The extra character check rejects input such as "12abc". */
        if (sscanf(input, "%d %c", &value, &extra) == 1) {
            return value;
        }

        printf("Invalid input. Enter a whole number.\n");
    }
}

float readFloat(const char *prompt) {
    char input[100];
    char extra;
    float value;

    while (1) {
        readLine(prompt, input, sizeof(input));

        if (sscanf(input, "%f %c", &value, &extra) == 1
            && value >= 0) {
            return value;
        }

        printf("Enter a valid non-negative amount.\n");
    }
}

void pauseScreen(void) {
    char input[8];
    readLine("\nPress Enter to continue...", input, sizeof(input));
}
