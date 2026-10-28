#include "skyline.h"
#include <stdio.h>

/* Write all flights to the pipe-separated flights.txt file. */
void saveFlights(void) {
    FILE *file = fopen("flights.txt", "w");
    int flightIndex;

    if (file == NULL) {
        printf("Could not save flights.txt\n");
        return;
    }

    /* Save each array item as one pipe-separated text record. */
    for (flightIndex = 0; flightIndex < flightCount; flightIndex++) {
        fprintf(file, "%d|%s|%s|%s|%s|%d|%d|%.2f\n",
                flights[flightIndex].flightNo,
                flights[flightIndex].source,
                flights[flightIndex].destination,
                flights[flightIndex].date,
                flights[flightIndex].time,
                flights[flightIndex].totalSeats,
                flights[flightIndex].availableSeats,
                flights[flightIndex].fare);
    }

    fclose(file);
}

/* Load saved flights; return 1 when at least one flight was read. */
int loadFlights(void) {
    FILE *file = fopen("flights.txt", "r");
    Flight flight;

    if (file == NULL) {
        return 0;
    }

    /* Each %[^|] reads text up to the next pipe separator. */
    while (flightCount < MAX_FLIGHTS &&
        fscanf(file, "%d|%29[^|]|%29[^|]|%14[^|]|%9[^|]|%d|%d|%f\n",
               &flight.flightNo, flight.source, flight.destination,
               flight.date, flight.time, &flight.totalSeats,
               &flight.availableSeats, &flight.fare) == 8) {

        flights[flightCount] = flight;
        flightCount++;
    }

    fclose(file);
    return flightCount > 0;
}

/* Write each confirmed booking to bookings.txt. */
void saveBookings(void) {
    FILE *file = fopen("bookings.txt", "w");
    Booking *currentBooking = bookingHead;

    if (file == NULL) {
        printf("Could not save bookings.txt\n");
        return;
    }

    while (currentBooking != NULL) {
        fprintf(file, "%d|%s|%d|%d\n",
                currentBooking->bookingID,
                currentBooking->passengerName,
                currentBooking->age,
                currentBooking->flightNo);

        currentBooking = currentBooking->next;
    }

    fclose(file);
}

/* Load bookings into the linked list and find the next unused booking ID. */
void loadBookings(void) {
    FILE *file = fopen("bookings.txt", "r");
    int bookingID;
    int passengerAge;
    int flightNo;
    char passengerName[NAME_LEN];

    if (file == NULL) {
        return;
    }

    /* Read saved records and rebuild the in-memory booking linked list. */
    while (fscanf(file, "%d|%49[^|]|%d|%d\n",
                  &bookingID, passengerName,
                  &passengerAge, &flightNo) == 4) {

        if (findFlightIndex(flightNo) != -1 &&
            appendBooking(bookingID, passengerName,
                          passengerAge, flightNo)) {

            if (bookingID >= nextBookingID) {
                nextBookingID = bookingID + 1;
            }
        }
    }

    fclose(file);
}

/* Save both kinds of permanent data. */
void saveAll(void) {
    saveFlights();
    saveBookings();
}
