#include "skyline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Return the array position of a flight, or -1 if it does not exist. */
int findFlightIndex(int flightNo) {
    int i;

    /* Flights are stored in an array, so check each item until it matches. */
    for (i = 0; i < flightCount; i++) {
        if (flights[i].flightNo == flightNo) {
            return i;
        }
    }

    return -1;
}

/* Add the example flights used when no saved flight file exists. */
void addSampleFlights(void) {
    Flight sample[] = {
        {101, "Delhi", "Mumbai", "2026-11-10",
         "09:30", 20, 20, 4000},

        {102, "Delhi", "Bengaluru", "2026-11-11",
         "12:15", 25, 25, 5200},

        {103, "Mumbai", "Goa", "2026-11-12",
         "16:00", 15, 15, 2800},

        {104, "Kolkata", "Delhi", "2026-11-13",
         "07:45", 18, 18, 4500},

        {105, "Chennai", "Hyderabad", "2026-11-14",
         "18:20", 22, 22, 3100}
    };

    int i;
    flightCount = (int)(sizeof(sample) / sizeof(sample[0]));

    for (i = 0; i < flightCount; i++) {
        flights[i] = sample[i];
    }
}

/* Print every flight in the array. */
void displayFlights(void) {
    int i;

    if (flightCount == 0) {
        printf("\nNo flights available.\n");
        return;
    }

    printf("\n================ AVAILABLE FLIGHTS ================\n");
    printf("%-8s %-13s %-13s %-12s %-8s %-8s %-10s\n",
           "Flight", "From", "To", "Date",
           "Time", "Seats", "Fare");

    printf("---------------------------------------------------"
           "-----------------------------\n");

    for (i = 0; i < flightCount; i++) {
        printf("%-8d %-13s %-13s %-12s %-8s %-8d Rs.%.2f\n",
               flights[i].flightNo,
               flights[i].source,
               flights[i].destination,
               flights[i].date,
               flights[i].time,
               flights[i].availableSeats,
               flights[i].fare);
    }
}

/* Ask for a flight number, then show its details. */
void searchFlightByNumber(void) {
    int number;
    int i;

    number = readInt("\nEnter flight number: ");
    i = findFlightIndex(number);

    if (i == -1) {
        printf("Flight not found.\n");
        return;
    }

    printf("\nFlight Details\n");
    printf("----------------------------------\n");
    printf("Flight number   : %d\n", flights[i].flightNo);
    printf("Route           : %s to %s\n",
           flights[i].source, flights[i].destination);
    printf("Date            : %s\n", flights[i].date);
    printf("Departure time  : %s\n", flights[i].time);
    printf("Available seats : %d\n", flights[i].availableSeats);
    printf("Ticket fare     : Rs. %.2f\n", flights[i].fare);
}

/* Show flights whose start and destination cities both match. */
void searchFlightsByRoute(void) {
    char source[CITY_LEN];
    char destination[CITY_LEN];
    int i, found = 0;

    readLine("\nDeparture city: ", source, sizeof(source));
    readLine("Destination city: ",
             destination, sizeof(destination));

    for (i = 0; i < flightCount; i++) {
        if (strcmp(flights[i].source, source) == 0 &&
            strcmp(flights[i].destination, destination) == 0) {

            printf("\nFlight %d | %s to %s\n",
                   flights[i].flightNo,
                   flights[i].source,
                   flights[i].destination);

            printf("Date: %s | Time: %s\n",
                   flights[i].date, flights[i].time);

            printf("Seats: %d | Fare: Rs. %.2f\n",
                   flights[i].availableSeats,
                   flights[i].fare);

            found = 1;
        }
    }

    if (!found) {
        printf("No matching route found.\n");
        printf("City names are case-sensitive.\n");
    }
}

/* Insert a flight number into its correct place in the BST. */
TreeNode *insertTree(TreeNode *root, int number) {
    TreeNode *node;

    /* An empty link is where this flight number belongs in the BST. */
    if (root == NULL) {
        node = (TreeNode *)malloc(sizeof(TreeNode));

        if (node == NULL) {
            return NULL;
        }

        node->flightNo = number;
        node->left = NULL;
        node->right = NULL;

        return node;
    }

    /* Smaller flight numbers go left; larger numbers go right. */
    if (number < root->flightNo) {
        root->left = insertTree(root->left, number);
    } else if (number > root->flightNo) {
        root->right = insertTree(root->right, number);
    }

    return root;
}

/* Find a flight number by following the BST's left or right links. */
TreeNode *searchTree(TreeNode *root, int number) {
    /* Each comparison chooses one branch, avoiding a full array scan. */
    if (root == NULL || root->flightNo == number) {
        return root;
    }

    if (number < root->flightNo) {
        return searchTree(root->left, number);
    }

    return searchTree(root->right, number);
}

/* Free this tree's children before freeing the parent node. */
void freeTree(TreeNode *root) {
    if (root == NULL) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

/* Recreate the BST from the flight array. */
void rebuildFlightTree(void) {
    int i;

    /* Rebuild after flight changes so the tree matches the flight array. */
    freeTree(flightRoot);
    flightRoot = NULL;

    for (i = 0; i < flightCount; i++) {
        flightRoot = insertTree(flightRoot, flights[i].flightNo);
    }
}

/* Search the BST, then print details from the flight array. */
void searchFlightBST(void) {
    int number, index;
    TreeNode *result;

    number = readInt("\nEnter flight number for BST search: ");

    result = searchTree(flightRoot, number);

    if (result == NULL) {
        printf("Flight not found in the BST.\n");
        return;
    }

    index = findFlightIndex(number);

    if (index != -1) {
        printf("Flight found using BST!\n");
        printf("%d: %s to %s | Fare: Rs. %.2f\n",
               flights[index].flightNo,
               flights[index].source,
               flights[index].destination,
               flights[index].fare);
    }
}

/* Ask for flight details and add a new flight to the array and BST. */
void addFlight(void) {
    Flight f;

    if (flightCount >= MAX_FLIGHTS) {
        printf("Maximum flight capacity reached.\n");
        return;
    }

    f.flightNo = readInt("New flight number: ");

    if (findFlightIndex(f.flightNo) != -1) {
        printf("That flight number already exists.\n");
        return;
    }

    readLine("Source city: ", f.source, sizeof(f.source));
    readLine("Destination city: ",
             f.destination, sizeof(f.destination));
    readLine("Date (YYYY-MM-DD): ", f.date, sizeof(f.date));
    readLine("Departure time (HH:MM): ", f.time, sizeof(f.time));

    f.totalSeats = readInt("Total seats: ");

    if (f.totalSeats <= 0) {
        printf("Total seats must be greater than zero.\n");
        return;
    }

    f.availableSeats = f.totalSeats;
    f.fare = readFloat("Ticket fare (Rs.): ");

    flights[flightCount++] = f;

    rebuildFlightTree();

    printf("Flight added successfully.\n");
}

/* Add up seats and bookings to print a simple admin report. */
void displayReport(void) {
    int i, total = 0, available = 0, count = 0;
    int index;
    float revenue = 0;
    Booking *current = bookingHead;

    for (i = 0; i < flightCount; i++) {
        total += flights[i].totalSeats;
        available += flights[i].availableSeats;
    }

    /* Walk the booking linked list to count bookings and estimate revenue. */
    while (current != NULL) {
        index = findFlightIndex(current->flightNo);

        if (index != -1) {
            revenue += flights[index].fare;
        }

        count++;
        current = current->next;
    }

    printf("\n========== AIRLINE SUMMARY REPORT ==========\n");
    printf("Total flights           : %d\n", flightCount);
    printf("Total seat capacity     : %d\n", total);
    printf("Available seats         : %d\n", available);
    printf("Occupied seats          : %d\n", total - available);
    printf("Confirmed bookings      : %d\n", count);
    printf("Estimated booking value : Rs. %.2f\n", revenue);
    printf("============================================\n");
}
