#ifndef SKYLINE_H
#define SKYLINE_H

#define MAX_FLIGHTS 50
#define NAME_LEN 50
#define CITY_LEN 30
#define OWNER_ID "SKYADMIN2026"

/* One flight stored in the fixed-size flight array. */
typedef struct {
    int flightNo;
    char source[CITY_LEN];
    char destination[CITY_LEN];
    char date[15];
    char time[10];
    int totalSeats;
    int availableSeats;
    float fare;
} Flight;

/* Confirmed bookings are kept in a singly linked list. */
typedef struct Booking {
    int bookingID;
    char passengerName[NAME_LEN];
    int age;
    int flightNo;
    struct Booking *next;
} Booking;

/* Waiting passengers use a FIFO queue: add at rear, remove from front. */
typedef struct QueueNode {
    char name[NAME_LEN];
    int age;
    int flightNo;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;

/* Actions use a stack, so the newest action is shown first. */
typedef struct StackNode {
    char action[150];
    struct StackNode *next;
} StackNode;

/* The BST stores flight numbers to support ordered flight searches. */
typedef struct TreeNode {
    int flightNo;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

extern Flight flights[MAX_FLIGHTS];
extern int flightCount;
extern int nextBookingID;
extern Booking *bookingHead;
extern Queue waitingList;
extern StackNode *historyTop;
extern TreeNode *flightRoot;

/* Read input safely so names and cities can contain spaces. */
void readLine(const char *prompt, char *buffer, int size);
int readInt(const char *prompt);
float readFloat(const char *prompt);
void pauseScreen(void);

/* Flight list and flight-number search tree. */
int findFlightIndex(int flightNo);
void addSampleFlights(void);
void displayFlights(void);
void searchFlightByNumber(void);
void searchFlightsByRoute(void);
TreeNode *insertTree(TreeNode *root, int number);
TreeNode *searchTree(TreeNode *root, int number);
void freeTree(TreeNode *root);
void rebuildFlightTree(void);
void searchFlightBST(void);
void addFlight(void);
void displayReport(void);

/* Confirmed bookings and passengers waiting for a seat. */
Booking *findBooking(int id);
int appendBooking(int id, const char *name, int age, int flightNo);
void displayAllBookings(void);
void viewBooking(int passengerMode);
void enqueueWaiting(const char *name, int age, int flightNo);
void displayWaitingList(void);
void pushHistory(const char *action);
void displayActionHistory(void);
void bookTicket(void);
void promoteWaitingPassenger(int flightNo);
void cancelTicket(int passengerMode);

/* Read and write saved flight and booking records. */
void saveFlights(void);
int loadFlights(void);
void saveBookings(void);
void loadBookings(void);
void saveAll(void);

/* Console menus and cleanup. */
void passengerMenu(void);
int ownerLogin(void);
void ownerMenu(void);
void freeAllMemory(void);

#endif
