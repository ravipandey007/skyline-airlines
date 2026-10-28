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

/* The BST stores flight numbers to support ordered flight searches. */
typedef struct TreeNode {
    int flightNo;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

/* One shared copy of each list, counter, and tree root. */
extern Flight flights[MAX_FLIGHTS];
extern int flightCount;
extern int nextBookingID;
extern Booking *bookingHead;
extern Queue waitingList;
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
TreeNode *insertTree(TreeNode *root, int flightNumber);
TreeNode *searchTree(TreeNode *root, int flightNumber);
void freeTree(TreeNode *root);
void rebuildFlightTree(void);
void searchFlightBST(void);
void addFlight(void);

/* Confirmed bookings and passengers waiting for a seat. */
Booking *findBooking(int bookingID);
int appendBooking(int bookingID, const char *name, int age, int flightNo);
void displayAllBookings(void);
void viewBooking(int passengerMode);
void enqueueWaiting(const char *name, int age, int flightNo);
void displayWaitingList(void);
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
