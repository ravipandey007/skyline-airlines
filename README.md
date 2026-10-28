# Skyline Airlines

A C99 airline-management system that runs in the Windows terminal. It lets
passengers browse flights and manage bookings, while an admin can manage
flights and review airline activity.

## Features

- View flights and search by flight number or route.
- Book tickets, view e-tickets, and cancel bookings.
- Join a flight waiting list when all seats are taken. When a booking is
  cancelled, the earliest waiting passenger for that flight is promoted.
- Add flights through the admin menu.
- Review confirmed bookings and the waiting list.
- Save flights and confirmed bookings between runs.

## Data structures

This project demonstrates common data structures in C:

| Structure | Used for |
| --- | --- |
| Array | Stores up to 50 flight records |
| Singly linked list | Stores confirmed bookings |
| FIFO queue | Keeps waiting passengers in arrival order |
| Binary search tree | Searches flights by flight number |

## Code explanation

See [`CODE_EXPLANATION.pdf`](CODE_EXPLANATION.pdf) for a beginner-friendly
walkthrough of the program flow, data structures, every source file, and every
function.

## Build and run

**Requirements:** Windows and GCC (for example, MinGW) available on `PATH`.

Double-click `run.bat`, or open a terminal in the project folder and run:

```bat
run.bat
```

The script compiles the C source files with C99 warnings enabled and starts
`build\skyline-airlines.exe`.

## Using the application

1. Choose **Passenger Portal** to browse flights, book a ticket, view a
   booking, or cancel it.
2. Choose **Airline Owner / Admin Portal** to manage flights and view
   bookings and the waiting list.
3. Keep the booking ID shown after booking; it is needed to view or cancel a
   booking.

The admin ID for this demo is `SKYADMIN2026`. It is stored in
`code\skyline.h`. This simple ID check is for demonstration purposes and is
not secure authentication.

## Project layout

```text
code\
  bookings.c   Booking list and waiting queue
  flights.c    Flight records and binary search tree
  input.c      Console input helpers
  main.c       Program entry point
  menus.c      Passenger and admin menus
  skyline.h    Shared types, constants, and declarations
  state.c      Shared application state and cleanup
  storage.c    Loading and saving records
run.bat        Build and run the console application
```

## Saved data

The program reads and writes `flights.txt` and `bookings.txt` in the project
folder. If there are no saved flights, it starts with sample flights.
Generated data files and build output are excluded from Git.

The waiting list is kept in memory and reset when the program exits.
