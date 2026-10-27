#include "skyline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================= BOOKING LINKED LIST ================= */

Booking *findBooking(int id) {
    Booking *current = bookingHead;

    while (current != NULL) {
        if (current->bookingID == id) {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

int appendBooking(int id, const char *name,
                  int age, int flightNo) {
    Booking *node;
    Booking *current;

    node = (Booking *)malloc(sizeof(Booking));

    if (node == NULL) {
        printf("Memory allocation failed.\n");
        return 0;
    }

    node->bookingID = id;
    node->age = age;
    node->flightNo = flightNo;

    snprintf(node->passengerName,
             sizeof(node->passengerName), "%s", name);

    node->next = NULL;

    /* Add the new booking at the end of the singly linked list. */
    if (bookingHead == NULL) {
        bookingHead = node;
    } else {
        current = bookingHead;

        while (current->next != NULL) {
            current = current->next;
        }

        current->next = node;
    }

    return 1;
}

void displayAllBookings(void) {
    Booking *current = bookingHead;

    if (current == NULL) {
        printf("\nNo confirmed bookings.\n");
        return;
    }

    printf("\n========== CONFIRMED BOOKINGS ==========\n");

    while (current != NULL) {
        printf("Booking ID: %d | Passenger: %s\n",
               current->bookingID,
               current->passengerName);

        printf("Age: %d | Flight: %d\n\n",
               current->age, current->flightNo);

        current = current->next;
    }
}

void viewBooking(int passengerMode) {
    int id, index;
    char name[NAME_LEN];
    Booking *booking;

    id = readInt("\nEnter booking ID: ");
    booking = findBooking(id);

    if (booking == NULL) {
        printf("Booking not found.\n");
        return;
    }

    if (passengerMode) {
        readLine("Enter passenger name used for booking: ",
                 name, sizeof(name));

        if (strcmp(name, booking->passengerName) != 0) {
            printf("Passenger name does not match.\n");
            return;
        }
    }

    index = findFlightIndex(booking->flightNo);

    printf("\n========== SKYLINE AIRLINES ==========\n");
    printf("             E-TICKET\n");
    printf("--------------------------------------\n");

    printf("Booking ID : %d\n", booking->bookingID);
    printf("Passenger  : %s\n", booking->passengerName);
    printf("Age        : %d\n", booking->age);
    printf("Flight     : %d\n", booking->flightNo);

    if (index != -1) {
        printf("Route      : %s to %s\n",
               flights[index].source,
               flights[index].destination);

        printf("Date       : %s\n", flights[index].date);
        printf("Time       : %s\n", flights[index].time);
        printf("Fare       : Rs. %.2f\n", flights[index].fare);
    }

    printf("======================================\n");
}

/* ================= WAITING QUEUE ================= */

void enqueueWaiting(const char *name, int age, int flightNo) {
    QueueNode *node;

    node = (QueueNode *)malloc(sizeof(QueueNode));

    if (node == NULL) {
        printf("Could not add passenger to waiting list.\n");
        return;
    }

    snprintf(node->name, sizeof(node->name), "%s", name);

    node->age = age;
    node->flightNo = flightNo;
    node->next = NULL;

    /* The first item is both the front and rear of an empty queue. */
    if (waitingList.rear == NULL) {
        waitingList.front = node;
        waitingList.rear = node;
    } else {
        waitingList.rear->next = node;
        waitingList.rear = node;
    }

    printf("\nThis flight is full.\n");
    printf("You have been added to the waiting list.\n");
}

void displayWaitingList(void) {
    QueueNode *current = waitingList.front;
    int position = 1;

    if (current == NULL) {
        printf("\nWaiting list is empty.\n");
        return;
    }

    printf("\n========== WAITING LIST ==========\n");

    while (current != NULL) {
        printf("%d. %s | Age: %d | Flight: %d\n",
               position,
               current->name,
               current->age,
               current->flightNo);

        position++;
        current = current->next;
    }
}

/* ================= ACTION HISTORY STACK ================= */

void pushHistory(const char *action) {
    StackNode *node;

    node = (StackNode *)malloc(sizeof(StackNode));

    if (node == NULL) {
        return;
    }

    snprintf(node->action, sizeof(node->action), "%s", action);

    /* Push onto the top so history is naturally newest-first (LIFO). */
    node->next = historyTop;
    historyTop = node;
}

void displayActionHistory(void) {
    StackNode *current = historyTop;

    if (current == NULL) {
        printf("\nNo actions recorded in this session.\n");
        return;
    }

    printf("\n========== RECENT ACTIONS ==========\n");

    while (current != NULL) {
        printf("- %s\n", current->action);
        current = current->next;
    }
}

/* ================= TICKET BOOKING ================= */

void bookTicket(void) {
    char name[NAME_LEN];
    char action[150];
    int age, number, index, id;

    readLine("\nPassenger full name: ", name, sizeof(name));

    if (name[0] == '\0') {
        printf("Name cannot be empty.\n");
        return;
    }

    age = readInt("Passenger age: ");

    if (age < 1 || age > 120) {
        printf("Enter an age between 1 and 120.\n");
        return;
    }

    displayFlights();

    number = readInt("\nEnter flight number to book: ");
    index = findFlightIndex(number);

    if (index == -1) {
        printf("Invalid flight number.\n");
        return;
    }

    if (flights[index].availableSeats == 0) {
        enqueueWaiting(name, age, number);
        return;
    }

    id = nextBookingID++;

    if (!appendBooking(id, name, age, number)) {
        nextBookingID--;
        return;
    }

    flights[index].availableSeats--;

    printf("\n");
    printf("================================================\n");
    printf("               SKYLINE AIRLINES\n");
    printf("                  E-TICKET\n");
    printf("================================================\n");

    printf("Booking ID      : %d\n", id);
    printf("Passenger       : %s\n", name);
    printf("Age             : %d\n", age);
    printf("Flight          : %d\n", number);
    printf("Route           : %s to %s\n",
           flights[index].source,
           flights[index].destination);
    printf("Travel date     : %s\n", flights[index].date);
    printf("Departure time  : %s\n", flights[index].time);
    printf("Ticket fare     : Rs. %.2f\n", flights[index].fare);
    printf("Seats remaining : %d\n", flights[index].availableSeats);

    printf("================================================\n");
    printf("Keep your booking ID for future reference.\n");

    snprintf(action, sizeof(action),
             "Booking %d created for %s on flight %d",
             id, name, number);

    pushHistory(action);
}

/* ================= WAITING-LIST PROMOTION ================= */

void promoteWaitingPassenger(int flightNo) {
    QueueNode *current = waitingList.front;
    QueueNode *previous = NULL;
    int index = findFlightIndex(flightNo);

    if (index == -1 || flights[index].availableSeats <= 0) {
        return;
    }

    /* Find the earliest queued passenger for this flight. */
    while (current != NULL && current->flightNo != flightNo) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return;
    }

    /* Unlink the passenger while keeping the queue's front and rear correct. */
    if (previous == NULL) {
        waitingList.front = current->next;
    } else {
        previous->next = current->next;
    }

    if (waitingList.rear == current) {
        waitingList.rear = previous;
    }

    {
        int id = nextBookingID++;
        char action[150];

        if (appendBooking(id, current->name, current->age, flightNo)) {
            flights[index].availableSeats--;

            printf("\nWaiting-list passenger confirmed!\n");
            printf("Passenger: %s\n", current->name);
            printf("New booking ID: %d\n", id);

            snprintf(action, sizeof(action),
                     "Waiting passenger %s confirmed on flight %d",
                     current->name, flightNo);

            pushHistory(action);
        } else {
            nextBookingID--;
        }
    }

    free(current);
}

/* ================= CANCEL TICKET ================= */

void cancelTicket(int passengerMode) {
    int id, index, flightNo;
    Booking *current = bookingHead;
    Booking *previous = NULL;
    char name[NAME_LEN];
    char action[150];

    id = readInt("\nEnter booking ID to cancel: ");

    while (current != NULL && current->bookingID != id) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Booking not found or already cancelled.\n");
        return;
    }

    if (passengerMode) {
        readLine("Enter passenger name used for booking: ",
                 name, sizeof(name));

        if (strcmp(name, current->passengerName) != 0) {
            printf("Passenger name does not match.\n");
            printf("Cancellation denied.\n");
            return;
        }
    }

    /* Remove this booking node from the linked list. */
    if (previous == NULL) {
        bookingHead = current->next;
    } else {
        previous->next = current->next;
    }

    flightNo = current->flightNo;
    index = findFlightIndex(flightNo);

    if (index != -1 &&
        flights[index].availableSeats < flights[index].totalSeats) {
        flights[index].availableSeats++;
    }

    snprintf(action, sizeof(action),
             "Booking %d for %s cancelled",
             current->bookingID, current->passengerName);

    printf("Booking %d cancelled successfully.\n",
           current->bookingID);

    free(current);
    pushHistory(action);

    /* If a seat becomes available, check the waiting list. */
    promoteWaitingPassenger(flightNo);
}
