#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include.h"

void borrowClassroom();
void returnClassroom();
void displayRecords();

void displayMenu() {
    printf("\nMenu:\n");
    printf("1. Borrow Classroom\n");
    printf("2. Return Classroom\n");
    printf("3. View Borrowing Records\n");
    printf("4. Exit\n");
}

int main() {
    int choice;
    initializeClassrooms();
    while(1) {
        displayMenu();
        choice = getIntInput("Enter your choice (1-4): ", 1, 4);
        switch(choice) {
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
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
