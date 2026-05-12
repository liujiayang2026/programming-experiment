#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include.h"

void borrowClassroom() {
    int classroomNum;
    char borrower[50];
    char student_id[20];
    char* classroomName;

    printSectionTitle("Borrow Room");
    displayAllClassrooms();
    classroomNum = getIntInput("\n\t\tEnter room number (1-10): ", 1, TOTAL_CLASSROOMS);
    classroomName = getClassroomNameByNumber(classroomNum);

    if (isRoomReservedAtTime(classroomName, time(NULL))) {
        printMessageBox("This room is reserved now and cannot be borrowed.");
        pauseScreen();
        return;
    }

    getStringInput("\t\tEnter borrower name: ", borrower, sizeof(borrower));
    getStringInput("\t\tEnter student ID: ", student_id, sizeof(student_id));

    addRecord(classroomName, borrower, student_id);
    displayAllClassrooms();
    pauseScreen();
}

void returnClassroom() {
    int classroomNum;
    char* classroomName;

    printSectionTitle("Return Room");
    displayAllClassrooms();
    classroomNum = getIntInput("\n\t\tEnter room number to return (1-10): ", 1, TOTAL_CLASSROOMS);
    classroomName = getClassroomNameByNumber(classroomNum);

    returnClassroomRecord(classroomName);
    displayAllClassrooms();
    pauseScreen();
}

void displayRecords() {
    printSectionTitle("All Borrowing Records");
    displayAllClassrooms();
    displayAllRecords();
    pauseScreen();
}

void reserveClassroom() {
    int classroomNum;
    char borrower[50];
    char studentId[20];
    char startBuffer[50];
    char endBuffer[50];
    char* classroomName;
    time_t startTime;
    time_t endTime;
    time_t now;
    struct tm* timeInfo;

    printSectionTitle("Reserve Room");
    displayAllClassrooms();
    classroomNum = getIntInput("\n\t\tEnter room number to reserve (1-10): ", 1, TOTAL_CLASSROOMS);
    classroomName = getClassroomNameByNumber(classroomNum);

    getStringInput("\t\tEnter reservation start time (MM-DD-HH, 24-hour): ", startBuffer, sizeof(startBuffer));
    if (!parseDateTimeInput(startBuffer, &startTime)) {
        printMessageBox("Invalid start time format. Use MM-DD-HH (e.g. 05-08-14 for May 8th 14:00)");
        pauseScreen();
        return;
    }

    getStringInput("\t\tEnter reservation end time (MM-DD-HH, 24-hour): ", endBuffer, sizeof(endBuffer));
    if (!parseDateTimeInput(endBuffer, &endTime)) {
        printMessageBox("Invalid end time format. Use MM-DD-HH (e.g. 05-08-16 for May 8th 16:00)");
        pauseScreen();
        return;
    }

    if (endTime <= startTime) {
        printMessageBox("End time must be later than start time.");
        pauseScreen();
        return;
    }

    now = time(NULL);
    if (startTime <= now) {
        printMessageBox("Start time must be in the future.");
        pauseScreen();
        return;
    }

    getStringInput("\t\tEnter reserver name: ", borrower, sizeof(borrower));
    getStringInput("\t\tEnter student ID: ", studentId, sizeof(studentId));

    if (hasReservationConflict(classroomName, startTime, endTime)) {
        printMessageBox("Reservation conflict detected for this room.");
        pauseScreen();
        return;
    }

    addReservationRecord(classroomName, borrower, studentId, startTime, endTime);
    printf("\n\t\tReservation created successfully.\n");
    printf("\t\tRoom       : %s\n", classroomName);
    printf("\t\tUser       : %s\n", borrower);
    printf("\t\tStudent ID : %s\n", studentId);
    timeInfo = localtime(&startTime);
    printf("\t\tStart Time : %02d-%02d-%02d\n",
           timeInfo->tm_mon + 1, timeInfo->tm_mday, timeInfo->tm_hour);
    timeInfo = localtime(&endTime);
    printf("\t\tEnd Time   : %02d-%02d-%02d\n",
           timeInfo->tm_mon + 1, timeInfo->tm_mday, timeInfo->tm_hour);
    pauseScreen();
}
