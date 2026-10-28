@echo off
setlocal
REM Run the compiler from the project folder.
cd /d "%~dp0"

REM Make the output folder the first time the app is built.
if not exist build mkdir build

REM Compile all C files using the C99 standard and show compiler warnings.
gcc -std=c99 -Wall -Wextra -pedantic -Icode code\main.c code\state.c code\input.c code\flights.c code\bookings.c code\storage.c code\menus.c -o build\skyline-airlines.exe

REM Do not try to run the program if compilation failed.
if errorlevel 1 exit /b %errorlevel%

REM Start the compiled application.
build\skyline-airlines.exe
