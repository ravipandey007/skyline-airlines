#include "skyline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Write all flights to the pipe-separated flights.txt file. */
void saveFlights(void) {
    FILE *fp = fopen("flights.txt", "w");
    int i;

    if (fp == NULL) {
        printf("Could not save flights.txt\n");
        return;
    }

    /* Save each array item as one pipe-separated text record. */
    for (i = 0; i < flightCount; i++) {
        fprintf(fp, "%d|%s|%s|%s|%s|%d|%d|%.2f\n",
                flights[i].flightNo,
                flights[i].source,
                flights[i].destination,
                flights[i].date,
                flights[i].time,
                flights[i].totalSeats,
                flights[i].availableSeats,
                flights[i].fare);
    }

    fclose(fp);
}

/* Load saved flights; return 1 when at least one flight was read. */
int loadFlights(void) {
    FILE *fp = fopen("flights.txt", "r");
    Flight f;

    if (fp == NULL) {
        return 0;
    }

    while (flightCount < MAX_FLIGHTS &&
        fscanf(fp, "%d|%29[^|]|%29[^|]|%14[^|]|%9[^|]|%d|%d|%f\n",
               &f.flightNo, f.source, f.destination,
               f.date, f.time, &f.totalSeats,
               &f.availableSeats, &f.fare) == 8) {

        flights[flightCount++] = f;
    }

    fclose(fp);
    return flightCount > 0;
}

/* Write each confirmed booking to bookings.txt. */
void saveBookings(void) {
    FILE *fp = fopen("bookings.txt", "w");
    Booking *current = bookingHead;

    if (fp == NULL) {
        printf("Could not save bookings.txt\n");
        return;
    }

    while (current != NULL) {
        fprintf(fp, "%d|%s|%d|%d\n",
                current->bookingID,
                current->passengerName,
                current->age,
                current->flightNo);

        current = current->next;
    }

    fclose(fp);
}

/* Load bookings into the linked list and find the next unused booking ID. */
void loadBookings(void) {
    FILE *fp = fopen("bookings.txt", "r");
    int id, age, number;
    char name[NAME_LEN];

    if (fp == NULL) {
        return;
    }

    /* Read saved records and rebuild the in-memory booking linked list. */
    while (fscanf(fp, "%d|%49[^|]|%d|%d\n",
                  &id, name, &age, &number) == 4) {

        if (findFlightIndex(number) != -1 &&
            appendBooking(id, name, age, number)) {

            if (id >= nextBookingID) {
                nextBookingID = id + 1;
            }
        }
    }

    fclose(fp);
}

/* Save both kinds of permanent data. */
void saveAll(void) {
    saveFlights();
    saveBookings();
}
