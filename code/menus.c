#include "skyline.h"
#include <stdio.h>
#include <string.h>

/* Show the passenger menu and run the selected action. */
void passengerMenu(void) {
    int menuChoice;

    do {
        printf("\n--- Passenger Portal ---\n");
        printf("1. View available flights\n");
        printf("2. Search flights by route\n");
        printf("3. Search flight by number\n");
        printf("4. Book a ticket\n");
        printf("5. View my booking\n");
        printf("6. Cancel my booking\n");
        printf("0. Back to main menu\n");

        menuChoice = readInt("Enter your choice: ");

        switch (menuChoice) {
            case 1:
                displayFlights();
                break;

            case 2:
                searchFlightsByRoute();
                break;

            case 3:
                searchFlightByNumber();
                break;

            case 4:
                bookTicket();
                saveAll();
                break;

            case 5:
                viewBooking(1);
                break;

            case 6:
                cancelTicket(1);
                saveAll();
                break;

            case 0:
                printf("Returning to main menu...\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }

        if (menuChoice != 0) {
            pauseScreen();
        }

    } while (menuChoice != 0);
}

/* Ask for the admin ID; allow up to three attempts. */
int ownerLogin(void) {
    char enteredID[60];
    int loginAttempt;

    printf("\n--- Owner / Admin Login ---\n");

    for (loginAttempt = 1; loginAttempt <= 3; loginAttempt++) {
        readLine("Enter owner ID: ", enteredID, sizeof(enteredID));

        if (strcmp(enteredID, OWNER_ID) == 0) {
            printf("\nAccess granted. Welcome, Administrator!\n");
            return 1;
        }

        printf("Incorrect ID. Attempts remaining: %d\n",
               3 - loginAttempt);
    }

    printf("Access denied. Returning to main menu.\n");
    return 0;
}

/* Check the admin ID, then show the admin menu. */
void ownerMenu(void) {
    int menuChoice;

    if (!ownerLogin()) {
        return;
    }

    do {
        printf("\n--- Admin Panel ---\n");
        printf("1. View all flights\n");
        printf("2. Add a new flight\n");
        printf("3. Search flight by number\n");
        printf("4. Search flight using BST\n");
        printf("5. View all confirmed bookings\n");
        printf("6. Search booking by ID\n");
        printf("7. Cancel any booking\n");
        printf("8. View waiting list\n");
        printf("9. Save all records\n");
        printf("0. Logout\n");

        menuChoice = readInt("Enter your choice: ");

        switch (menuChoice) {
            case 1:
                displayFlights();
                break;

            case 2:
                addFlight();
                saveFlights();
                break;

            case 3:
                searchFlightByNumber();
                break;

            case 4:
                searchFlightBST();
                break;

            case 5:
                displayAllBookings();
                break;

            case 6:
                viewBooking(0);
                break;

            case 7:
                cancelTicket(0);
                saveAll();
                break;

            case 8:
                displayWaitingList();
                break;

            case 9:
                saveAll();
                printf("All records saved successfully.\n");
                break;

            case 0:
                saveAll();
                printf("Admin logged out successfully.\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }

        if (menuChoice != 0) {
            pauseScreen();
        }

    } while (menuChoice != 0);
}
