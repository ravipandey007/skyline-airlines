@echo off
setlocal
cd /d "%~dp0"

if not exist build mkdir build

gcc -std=c99 -Wall -Wextra -pedantic -Icode code\main.c code\state.c code\input.c code\flights.c code\bookings.c code\storage.c code\menus.c -o build\skyline-airlines.exe
if errorlevel 1 exit /b %errorlevel%

build\skyline-airlines.exe
