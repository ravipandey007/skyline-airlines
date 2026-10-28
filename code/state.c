#include "skyline.h"
#include <stdlib.h>

/* All shared program data is defined here and declared in skyline.h. */
Flight flights[MAX_FLIGHTS];
int flightCount = 0;
int nextBookingID = 1001;
Booking *bookingHead = NULL;
Queue waitingList = {NULL, NULL};
TreeNode *flightRoot = NULL;

/* Release every dynamically allocated node before the program exits. */
void freeAllMemory(void) {
    Booking *booking;
    QueueNode *passenger;

    while (bookingHead != NULL) {
        booking = bookingHead;
        bookingHead = bookingHead->next;
        free(booking);
    }

    while (waitingList.front != NULL) {
        passenger = waitingList.front;
        waitingList.front = waitingList.front->next;
        free(passenger);
    }

    /* freeTree visits and releases each node in the search tree. */
    freeTree(flightRoot);
}
