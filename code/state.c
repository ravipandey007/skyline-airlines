#include "skyline.h"
#include <stdlib.h>

Flight flights[MAX_FLIGHTS];
int flightCount = 0;
int nextBookingID = 1001;
Booking *bookingHead = NULL;
Queue waitingList = {NULL, NULL};
StackNode *historyTop = NULL;
TreeNode *flightRoot = NULL;

void freeAllMemory(void) {
    Booking *b;
    QueueNode *q;
    StackNode *s;

    while (bookingHead != NULL) {
        b = bookingHead;
        bookingHead = bookingHead->next;
        free(b);
    }

    while (waitingList.front != NULL) {
        q = waitingList.front;
        waitingList.front = waitingList.front->next;
        free(q);
    }

    while (historyTop != NULL) {
        s = historyTop;
        historyTop = historyTop->next;
        free(s);
    }

    freeTree(flightRoot);
}
