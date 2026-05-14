#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include.h"

void displayMenu() {
    printf("\n");
    printf("\t\t   --------------------------------\n");
    printf("\t\t|      Classroom Management        |\n");
    printf("\t\t|                                  |\n");
    printf("\t\t|  1.Borrow Room   2.Return Room   |\n");
    printf("\t\t|                                  |\n");
    printf("\t\t|  3.View Records  4.Query Room    |\n");
    printf("\t\t|                                  |\n");
    printf("\t\t|  5.Query User    6.Query ID      |\n");
    printf("\t\t|                                  |\n");
    printf("\t\t|  7.Reserve Room  8.Exit System   |\n");
    printf("\t\t   --------------------------------\n");
}

int main() {
    int choice;
    char classroom[50];
    char borrower[50];
    char studentId[20];

    initializeClassrooms();

    while (1) {
        displayMenu();
        choice = getIntInput("\t\tEnter menu number (1-8): ", 1, 8);

        switch (choice) {
            case 1:
                borrowClassroom();
                break;
            case 2:
                returnClassroom();
                break;
            case 3:
                displayRecords();
                break;
            case 4:
                printSectionTitle("Query by Room");
                getStringInput("\t\tEnter room number or room name: ", classroom, sizeof(classroom));
                displayRecordsByClassroom(classroom);
                pauseScreen();
                break;
            case 5:
                printSectionTitle("Query by User");
                getStringInput("\t\tEnter borrower name: ", borrower, sizeof(borrower));
                displayRecordsByBorrower(borrower);
                pauseScreen();
                break;
            case 6:
                printSectionTitle("Query by Student ID");
                getStringInput("\t\tEnter student ID: ", studentId, sizeof(studentId));
                displayRecordsByStudentId(studentId);
                pauseScreen();
                break;
            case 7:
                reserveClassroom();
                break;
            case 8:
                printMessageBox("System exited. Goodbye.");
                return 0;
            default:
                printMessageBox("Invalid input. Try again.");
        }
    }

    return 0;
}
