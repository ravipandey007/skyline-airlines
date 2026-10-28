#include "skyline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Return the array position of a flight, or -1 if it does not exist. */
int findFlightIndex(int flightNo) {
    int flightIndex;

    /* Flights are stored in an array, so check each item until it matches. */
    for (flightIndex = 0; flightIndex < flightCount; flightIndex++) {
        if (flights[flightIndex].flightNo == flightNo) {
            return flightIndex;
        }
    }

    return -1;
}

/* Add the example flights used when no saved flight file exists. */
void addSampleFlights(void) {
    /* Each row lists: number, route, date, time, seats, available, fare. */
    Flight sample[] = {
        {101, "Delhi", "Mumbai", "2026-11-10", "09:30", 20, 20, 4000},
        {102, "Delhi", "Bengaluru", "2026-11-11", "12:15", 25, 25, 5200},
        {103, "Mumbai", "Goa", "2026-11-12", "16:00", 15, 15, 2800},
        {104, "Kolkata", "Delhi", "2026-11-13", "07:45", 18, 18, 4500},
        {105, "Chennai", "Hyderabad", "2026-11-14", "18:20", 22, 22, 3100}
    };

    int flightIndex;
    flightCount = (int)(sizeof(sample) / sizeof(sample[0]));

    for (flightIndex = 0; flightIndex < flightCount; flightIndex++) {
        flights[flightIndex] = sample[flightIndex];
    }
}

/* Print every flight in the array. */
void displayFlights(void) {
    int flightIndex;

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

    for (flightIndex = 0; flightIndex < flightCount; flightIndex++) {
        printf("%-8d %-13s %-13s %-12s %-8s %-8d Rs.%.2f\n",
               flights[flightIndex].flightNo,
               flights[flightIndex].source,
               flights[flightIndex].destination,
               flights[flightIndex].date,
               flights[flightIndex].time,
               flights[flightIndex].availableSeats,
               flights[flightIndex].fare);
    }
}

/* Ask for a flight number, then show its details. */
void searchFlightByNumber(void) {
    int flightNumber;
    int flightIndex;

    flightNumber = readInt("\nEnter flight number: ");
    flightIndex = findFlightIndex(flightNumber);

    if (flightIndex == -1) {
        printf("Flight not found.\n");
        return;
    }

    printf("\nFlight Details\n");
    printf("----------------------------------\n");
    printf("Flight number   : %d\n", flights[flightIndex].flightNo);
    printf("Route           : %s to %s\n",
           flights[flightIndex].source, flights[flightIndex].destination);
    printf("Date            : %s\n", flights[flightIndex].date);
    printf("Departure time  : %s\n", flights[flightIndex].time);
    printf("Available seats : %d\n", flights[flightIndex].availableSeats);
    printf("Ticket fare     : Rs. %.2f\n", flights[flightIndex].fare);
}

/* Show flights whose start and destination cities both match. */
void searchFlightsByRoute(void) {
    char source[CITY_LEN];
    char destination[CITY_LEN];
    int flightIndex;
    int found = 0;

    readLine("\nDeparture city: ", source, sizeof(source));
    readLine("Destination city: ",
             destination, sizeof(destination));

    for (flightIndex = 0; flightIndex < flightCount; flightIndex++) {
        if (strcmp(flights[flightIndex].source, source) == 0 &&
            strcmp(flights[flightIndex].destination, destination) == 0) {

            printf("\nFlight %d | %s to %s\n",
                   flights[flightIndex].flightNo,
                   flights[flightIndex].source,
                   flights[flightIndex].destination);

            printf("Date: %s | Time: %s\n",
                   flights[flightIndex].date, flights[flightIndex].time);

            printf("Seats: %d | Fare: Rs. %.2f\n",
                   flights[flightIndex].availableSeats,
                   flights[flightIndex].fare);

            found = 1;
        }
    }

    if (!found) {
        printf("No matching route found.\n");
        printf("City names are case-sensitive.\n");
    }
}

/* Insert a flight number into its correct place in the BST. */
TreeNode *insertTree(TreeNode *root, int flightNumber) {
    /* An empty link is where this flight number belongs in the BST. */
    if (root == NULL) {
        TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));

        if (newNode == NULL) {
            return NULL;
        }

        newNode->flightNo = flightNumber;
        newNode->left = NULL;
        newNode->right = NULL;

        return newNode;
    }

    /* Smaller flight numbers go left; larger numbers go right. */
    if (flightNumber < root->flightNo) {
        root->left = insertTree(root->left, flightNumber);
    } else if (flightNumber > root->flightNo) {
        root->right = insertTree(root->right, flightNumber);
    }

    return root;
}

/* Find a flight number by following the BST's left or right links. */
TreeNode *searchTree(TreeNode *root, int flightNumber) {
    /* Each comparison chooses one branch, avoiding a full array scan. */
    if (root == NULL || root->flightNo == flightNumber) {
        return root;
    }

    if (flightNumber < root->flightNo) {
        return searchTree(root->left, flightNumber);
    }

    return searchTree(root->right, flightNumber);
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
    int flightIndex;

    /* Rebuild after flight changes so the tree matches the flight array. */
    freeTree(flightRoot);
    flightRoot = NULL;

    for (flightIndex = 0; flightIndex < flightCount; flightIndex++) {
        flightRoot = insertTree(flightRoot, flights[flightIndex].flightNo);
    }
}

/* Search the BST, then print details from the flight array. */
void searchFlightBST(void) {
    int flightNumber;
    int flightIndex;
    TreeNode *matchingNode;

    flightNumber = readInt("\nEnter flight number for BST search: ");

    matchingNode = searchTree(flightRoot, flightNumber);

    if (matchingNode == NULL) {
        printf("Flight not found in the BST.\n");
        return;
    }

    flightIndex = findFlightIndex(flightNumber);

    if (flightIndex != -1) {
        printf("Flight found using BST!\n");
        printf("%d: %s to %s | Fare: Rs. %.2f\n",
               flights[flightIndex].flightNo,
               flights[flightIndex].source,
               flights[flightIndex].destination,
               flights[flightIndex].fare);
    }
}

/* Ask for flight details and add a new flight to the array and BST. */
void addFlight(void) {
    Flight newFlight;

    if (flightCount >= MAX_FLIGHTS) {
        printf("Maximum flight capacity reached.\n");
        return;
    }

    newFlight.flightNo = readInt("New flight number: ");

    if (findFlightIndex(newFlight.flightNo) != -1) {
        printf("That flight number already exists.\n");
        return;
    }

    readLine("Source city: ", newFlight.source, sizeof(newFlight.source));
    readLine("Destination city: ",
             newFlight.destination, sizeof(newFlight.destination));
    readLine("Date (YYYY-MM-DD): ", newFlight.date, sizeof(newFlight.date));
    readLine("Departure time (HH:MM): ", newFlight.time, sizeof(newFlight.time));

    newFlight.totalSeats = readInt("Total seats: ");

    if (newFlight.totalSeats <= 0) {
        printf("Total seats must be greater than zero.\n");
        return;
    }

    newFlight.availableSeats = newFlight.totalSeats;
    newFlight.fare = readFloat("Ticket fare (Rs.): ");

    flights[flightCount++] = newFlight;

    rebuildFlightTree();

    printf("Flight added successfully.\n");
}
