#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include.h"

Classroom classrooms[TOTAL_CLASSROOMS];
Record records[MAX_RECORDS];
int recordCount = 0;

void initializeClassrooms() {
    for (int i = 0; i < TOTAL_CLASSROOMS; i++) {
        classrooms[i].id = i + 1;
        sprintf(classrooms[i].name, "Classroom %d", i + 1);
        classrooms[i].isOccupied = 0;
        strcpy(classrooms[i].borrower, "");
        strcpy(classrooms[i].student_id, "");
    }
}

void displayAllClassrooms() {
    printf("\n--- Classroom Status ---\n");
    for (int i = 0; i < TOTAL_CLASSROOMS; i++) {
        if (classrooms[i].isOccupied) {
            printf("%s - Occupied by %s (ID: %s)\n", 
                   classrooms[i].name, 
                   classrooms[i].borrower, 
                   classrooms[i].student_id);
        } else {
            printf("%s - Available\n", classrooms[i].name);
        }
    }
}

int getIntInput(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &value) == 1) {
                if (value >= min && value <= max) {
                    return value;
                } else {
                    printf("Error: Please enter a number between %d and %d.\n", min, max);
                }
            } else {
                printf("Error: Invalid input. Please enter a number.\n");
            }
        }
    }
}

void getStringInput(const char* prompt, char* buffer, int maxLen) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, maxLen, stdin) != NULL) {
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }
            if (strlen(buffer) > 0) {
                return;
            } else {
                printf("Error: Input cannot be empty. Please try again.\n");
            }
        }
    }
}

int isValidClassroomNumber(int num) {
    return (num >= 1 && num <= TOTAL_CLASSROOMS);
}

char* getClassroomNameByNumber(int num) {
    if (isValidClassroomNumber(num)) {
        return classrooms[num - 1].name;
    }
    return NULL;
}

void addRecord(const char* classroom, const char* borrower, const char* student_id) {
    int found = 0;
    for (int i = 0; i < TOTAL_CLASSROOMS; i++) {
        if (strcmp(classrooms[i].name, classroom) == 0) {
            if (classrooms[i].isOccupied) {
                printf("Error: %s is already occupied!\n", classroom);
                return;
            }
            classrooms[i].isOccupied = 1;
            strcpy(classrooms[i].borrower, borrower);
            strcpy(classrooms[i].student_id, student_id);
            found = 1;
            break;
        }
    }
    
    if (found) {
        if (recordCount < MAX_RECORDS) {
            strcpy(records[recordCount].classroom, classroom);
            strcpy(records[recordCount].borrower, borrower);
            strcpy(records[recordCount].student_id, student_id);
            records[recordCount].isReturned = 0;
            recordCount++;
            printf("Record added: %s borrowed by %s (ID: %s)\n", classroom, borrower, student_id);
        } else {
            printf("Record limit reached. Cannot add more records.\n");
        }
    } else {
        printf("Error: Classroom %s not found!\n", classroom);
    }
}

void returnClassroomRecord(const char* classroom) {
    int found = 0;
    for (int i = 0; i < TOTAL_CLASSROOMS; i++) {
        if (strcmp(classrooms[i].name, classroom) == 0) {
            if (!classrooms[i].isOccupied) {
                printf("Error: %s is not occupied!\n", classroom);
                return;
            }
            classrooms[i].isOccupied = 0;
            strcpy(classrooms[i].borrower, "");
            strcpy(classrooms[i].student_id, "");
            found = 1;
            break;
        }
    }
    
    if (found) {
        for (int i = recordCount - 1; i >= 0; i--) {
            if (strcmp(records[i].classroom, classroom) == 0 && !records[i].isReturned) {
                records[i].isReturned = 1;
                break;
            }
        }
        printf("Returned: %s\n", classroom);
    } else {
        printf("Error: Classroom %s not found!\n", classroom);
    }
}

void displayAllRecords() {
    printf("\n--- Borrowing Records ---\n");
    for (int i = 0; i < recordCount; i++) {
        if (records[i].isReturned) {
            printf("%d. %s borrowed by %s (ID: %s) - Returned\n", 
                   i + 1, 
                   records[i].classroom, 
                   records[i].borrower, 
                   records[i].student_id);
        } else {
            printf("%d. %s borrowed by %s (ID: %s) - Not Returned\n", 
                   i + 1, 
                   records[i].classroom, 
                   records[i].borrower, 
                   records[i].student_id);
        }
    }
    if (recordCount == 0) {
        printf("No records found.\n");
    }
}
