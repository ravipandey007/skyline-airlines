#include "skyline.h"
#include <stdio.h>

/* Start the program, load saved data, and show the main menu. */
int main(void) {
    int menuChoice;

    printf("\nLoading Skyline Airlines...\n");

    /* Load saved flights, or start with sample flights on the first run. */
    if (!loadFlights()) {
        addSampleFlights();
        saveFlights();
    }

    /* Build the in-memory booking list and flight search tree. */
    loadBookings();
    rebuildFlightTree();

    printf("System ready.\n");

    do {
        printf("\n=== SKYLINE AIRLINES ===\n");
        printf("1. Passenger Portal\n");
        printf("2. Airline Owner / Admin Portal\n");
        printf("0. Exit\n");

        menuChoice = readInt("Enter your choice: ");

        switch (menuChoice) {
            case 1:
                passengerMenu();
                break;

            case 2:
                ownerMenu();
                break;

            case 0:
                saveAll();
                printf("Thank you for choosing Skyline Airlines.\n");
                break;

            default:
                printf("Invalid choice. Choose 1, 2, or 0.\n");
        }

    } while (menuChoice != 0);

    freeAllMemory();

    return 0;
}
