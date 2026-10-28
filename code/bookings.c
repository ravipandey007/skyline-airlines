#include "skyline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Find and return a booking by its ID. */
Booking *findBooking(int bookingID) {
    Booking *currentBooking = bookingHead;

    while (currentBooking != NULL) {
        if (currentBooking->bookingID == bookingID) {
            return currentBooking;
        }

        currentBooking = currentBooking->next;
    }

    return NULL;
}

/* Make a booking node and attach it to the end of the booking list. */
int appendBooking(int bookingID, const char *name,
                  int age, int flightNo) {
    Booking *newBooking;
    Booking *lastBooking;

    newBooking = (Booking *)malloc(sizeof(Booking));

    if (newBooking == NULL) {
        printf("Memory allocation failed.\n");
        return 0;
    }

    newBooking->bookingID = bookingID;
    newBooking->age = age;
    newBooking->flightNo = flightNo;

    snprintf(newBooking->passengerName,
             sizeof(newBooking->passengerName), "%s", name);

    newBooking->next = NULL;

    /* Add the new booking at the end of the singly linked list. */
    if (bookingHead == NULL) {
        bookingHead = newBooking;
    } else {
        lastBooking = bookingHead;

        while (lastBooking->next != NULL) {
            lastBooking = lastBooking->next;
        }

        lastBooking->next = newBooking;
    }

    return 1;
}

/* Print each booking in the linked list. */
void displayAllBookings(void) {
    Booking *currentBooking = bookingHead;

    if (currentBooking == NULL) {
        printf("\nNo confirmed bookings.\n");
        return;
    }

    printf("\n========== CONFIRMED BOOKINGS ==========\n");

    while (currentBooking != NULL) {
        printf("Booking ID: %d | Passenger: %s\n",
               currentBooking->bookingID,
               currentBooking->passengerName);

        printf("Age: %d | Flight: %d\n\n",
               currentBooking->age, currentBooking->flightNo);

        currentBooking = currentBooking->next;
    }
}

/* Show a booking; passengers must also enter the booking name. */
void viewBooking(int passengerMode) {
    int bookingID;
    int flightIndex;
    char passengerName[NAME_LEN];
    Booking *booking;

    bookingID = readInt("\nEnter booking ID: ");
    booking = findBooking(bookingID);

    if (booking == NULL) {
        printf("Booking not found.\n");
        return;
    }

    if (passengerMode) {
        readLine("Enter passenger name used for booking: ",
                 passengerName, sizeof(passengerName));

        if (strcmp(passengerName, booking->passengerName) != 0) {
            printf("Passenger name does not match.\n");
            return;
        }
    }

    flightIndex = findFlightIndex(booking->flightNo);

    printf("\n========== SKYLINE AIRLINES ==========\n");
    printf("             E-TICKET\n");
    printf("--------------------------------------\n");

    printf("Booking ID : %d\n", booking->bookingID);
    printf("Passenger  : %s\n", booking->passengerName);
    printf("Age        : %d\n", booking->age);
    printf("Flight     : %d\n", booking->flightNo);

    if (flightIndex != -1) {
        printf("Route      : %s to %s\n",
               flights[flightIndex].source,
               flights[flightIndex].destination);

        printf("Date       : %s\n", flights[flightIndex].date);
        printf("Time       : %s\n", flights[flightIndex].time);
        printf("Fare       : Rs. %.2f\n", flights[flightIndex].fare);
    }

    printf("======================================\n");
}

/* Add a passenger to the back of the waiting queue. */
void enqueueWaiting(const char *name, int age, int flightNo) {
    QueueNode *newPassenger;

    newPassenger = (QueueNode *)malloc(sizeof(QueueNode));

    if (newPassenger == NULL) {
        printf("Could not add passenger to waiting list.\n");
        return;
    }

    snprintf(newPassenger->name, sizeof(newPassenger->name), "%s", name);

    newPassenger->age = age;
    newPassenger->flightNo = flightNo;
    newPassenger->next = NULL;

    /* The first item is both the front and rear of an empty queue. */
    if (waitingList.rear == NULL) {
        waitingList.front = newPassenger;
        waitingList.rear = newPassenger;
    } else {
        waitingList.rear->next = newPassenger;
        waitingList.rear = newPassenger;
    }

    printf("\nThis flight is full.\n");
    printf("You have been added to the waiting list.\n");
}

/* Print passengers from the front to the back of the queue. */
void displayWaitingList(void) {
    QueueNode *currentPassenger = waitingList.front;
    int position = 1;

    if (currentPassenger == NULL) {
        printf("\nWaiting list is empty.\n");
        return;
    }

    printf("\n========== WAITING LIST ==========\n");

    while (currentPassenger != NULL) {
        printf("%d. %s | Age: %d | Flight: %d\n",
               position,
               currentPassenger->name,
               currentPassenger->age,
               currentPassenger->flightNo);

        position++;
        currentPassenger = currentPassenger->next;
    }
}

/* Book a seat, or add the passenger to the queue when the flight is full. */
void bookTicket(void) {
    char name[NAME_LEN];
    int passengerAge;
    int flightNumber;
    int flightIndex;
    int bookingID;

    readLine("\nPassenger full name: ", name, sizeof(name));

    if (name[0] == '\0') {
        printf("Name cannot be empty.\n");
        return;
    }

    passengerAge = readInt("Passenger age: ");

    if (passengerAge < 1 || passengerAge > 120) {
        printf("Enter an age between 1 and 120.\n");
        return;
    }

    displayFlights();

    flightNumber = readInt("\nEnter flight number to book: ");
    flightIndex = findFlightIndex(flightNumber);

    if (flightIndex == -1) {
        printf("Invalid flight number.\n");
        return;
    }

    if (flights[flightIndex].availableSeats == 0) {
        enqueueWaiting(name, passengerAge, flightNumber);
        return;
    }

    bookingID = nextBookingID++;

    if (!appendBooking(bookingID, name, passengerAge, flightNumber)) {
        nextBookingID--;
        return;
    }

    flights[flightIndex].availableSeats--;

    printf("\n");
    printf("================================================\n");
    printf("               SKYLINE AIRLINES\n");
    printf("                  E-TICKET\n");
    printf("================================================\n");

    printf("Booking ID      : %d\n", bookingID);
    printf("Passenger       : %s\n", name);
    printf("Age             : %d\n", passengerAge);
    printf("Flight          : %d\n", flightNumber);
    printf("Route           : %s to %s\n",
           flights[flightIndex].source,
           flights[flightIndex].destination);
    printf("Travel date     : %s\n", flights[flightIndex].date);
    printf("Departure time  : %s\n", flights[flightIndex].time);
    printf("Ticket fare     : Rs. %.2f\n", flights[flightIndex].fare);
    printf("Seats remaining : %d\n", flights[flightIndex].availableSeats);

    printf("================================================\n");
    printf("Keep your booking ID for future reference.\n");

}

/* Move the first waiting passenger for this flight into confirmed bookings. */
void promoteWaitingPassenger(int flightNo) {
    QueueNode *currentPassenger = waitingList.front;
    QueueNode *previousPassenger = NULL;
    int flightIndex = findFlightIndex(flightNo);
    int bookingID;

    if (flightIndex == -1 || flights[flightIndex].availableSeats <= 0) {
        return;
    }

    /* Find the earliest queued passenger for this flight. */
    while (currentPassenger != NULL &&
           currentPassenger->flightNo != flightNo) {
        previousPassenger = currentPassenger;
        currentPassenger = currentPassenger->next;
    }

    if (currentPassenger == NULL) {
        return;
    }

    /* Confirm the booking before removing the passenger from the queue. */
    bookingID = nextBookingID;
    if (!appendBooking(bookingID, currentPassenger->name,
                       currentPassenger->age, flightNo)) {
        return;
    }

    /* Unlink the passenger while keeping the queue's front and rear correct. */
    if (previousPassenger == NULL) {
        waitingList.front = currentPassenger->next;
    } else {
        previousPassenger->next = currentPassenger->next;
    }

    if (waitingList.rear == currentPassenger) {
        waitingList.rear = previousPassenger;
    }

    nextBookingID++;
    flights[flightIndex].availableSeats--;

    printf("\nWaiting-list passenger confirmed!\n");
    printf("Passenger: %s\n", currentPassenger->name);
    printf("New booking ID: %d\n", bookingID);

    free(currentPassenger);
}

/* Cancel a booking and offer its seat to the next waiting passenger. */
void cancelTicket(int passengerMode) {
    int bookingID;
    int flightIndex;
    int flightNo;
    Booking *currentBooking = bookingHead;
    Booking *previousBooking = NULL;
    char passengerName[NAME_LEN];

    bookingID = readInt("\nEnter booking ID to cancel: ");

    while (currentBooking != NULL &&
           currentBooking->bookingID != bookingID) {
        previousBooking = currentBooking;
        currentBooking = currentBooking->next;
    }

    if (currentBooking == NULL) {
        printf("Booking not found or already cancelled.\n");
        return;
    }

    if (passengerMode) {
        readLine("Enter passenger name used for booking: ",
                 passengerName, sizeof(passengerName));

        if (strcmp(passengerName, currentBooking->passengerName) != 0) {
            printf("Passenger name does not match.\n");
            printf("Cancellation denied.\n");
            return;
        }
    }

    /* Remove this booking node from the linked list. */
    if (previousBooking == NULL) {
        bookingHead = currentBooking->next;
    } else {
        previousBooking->next = currentBooking->next;
    }

    flightNo = currentBooking->flightNo;
    flightIndex = findFlightIndex(flightNo);

    if (flightIndex != -1 &&
        flights[flightIndex].availableSeats <
            flights[flightIndex].totalSeats) {
        flights[flightIndex].availableSeats++;
    }

    printf("Booking %d cancelled successfully.\n",
           currentBooking->bookingID);

    free(currentBooking);

    /* If a seat becomes available, check the waiting list. */
    promoteWaitingPassenger(flightNo);
}
