#include "skyline.h"
#include <stdio.h>

/* ================= MAIN FUNCTION ================= */

int main(void) {
    int choice;

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
        printf("\n");
        printf("====================================================\n");
        printf("                 SKYLINE AIRLINES\n");
        printf("             AIRLINE MANAGEMENT SYSTEM\n");
        printf("====================================================\n");
        printf("             Welcome! Fly with comfort.\n");
        printf("----------------------------------------------------\n");
        printf("1. Passenger Portal\n");
        printf("2. Airline Owner / Admin Portal\n");
        printf("0. Exit\n");
        printf("----------------------------------------------------\n");

        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1:
                passengerMenu();
                break;

            case 2:
                ownerMenu();
                break;

            case 0:
                saveAll();
                printf("\nThank you for choosing Skyline Airlines.\n");
                printf("Have a safe journey!\n");
                break;

            default:
                printf("Please choose 1, 2, or 0.\n");
        }

    } while (choice != 0);

    freeAllMemory();

    return 0;
}
