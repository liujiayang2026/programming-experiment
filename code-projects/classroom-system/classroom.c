#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include.h"

Classroom classrooms[TOTAL_CLASSROOMS];
Record records[MAX_RECORDS];
int recordCount = 0;

static void formatTimeValue(time_t value, char* buffer, size_t size) {
    struct tm* timeInfo;

    if (value == 0) {
        snprintf(buffer, size, "N/A");
        return;
    }

    timeInfo = localtime(&value);
    if (timeInfo == NULL) {
        snprintf(buffer, size, "N/A");
        return;
    }

    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeInfo);
}

static void displaySingleRecord(const Record* record) {
    char borrowTime[50];
    char returnTime[50];
    char reserveStart[50];
    char reserveEnd[50];

    formatTimeValue(record->borrowTime, borrowTime, sizeof(borrowTime));
    formatTimeValue(record->returnTime, returnTime, sizeof(returnTime));
    formatTimeValue(record->reserveStartTime, reserveStart, sizeof(reserveStart));
    formatTimeValue(record->reserveEndTime, reserveEnd, sizeof(reserveEnd));

    if (record->isReservation) {
        printf("\t\t  Record Type  : Reservation\n");
        printf("\t\t  Room Name    : %s\n", record->classroom);
        printf("\t\t  User         : %s\n", record->borrower);
        printf("\t\t  Student ID   : %s\n", record->student_id);
        printf("\t\t  Start Time   : %s\n", reserveStart);
        printf("\t\t  End Time     : %s\n", reserveEnd);
        if (record->isReturned) {
            printf("\t\t  Status       : Returned\n");
        } else {
            time_t now = time(NULL);
            if (now < record->reserveStartTime) {
                printf("\t\t  Status       : Upcoming\n");
            } else if (now > record->reserveEndTime) {
                printf("\t\t  Status       : Expired\n");
            } else {
                printf("\t\t  Status       : In Progress\n");
            }
        }
    } else {
        printf("\t\t  Record Type  : Borrowing\n");
        printf("\t\t  Room Name    : %s\n", record->classroom);
        printf("\t\t  Borrower     : %s\n", record->borrower);
        printf("\t\t  Student ID   : %s\n", record->student_id);
        printf("\t\t  Borrow Time  : %s\n", borrowTime);
        printf("\t\t  Status       : %s\n", record->isReturned ? "Returned" : "Not Returned");
        printf("\t\t  Return Time  : %s\n", returnTime);
    }
    printf("\t\t---------------------------------------------------\n");
}

void printSectionTitle(const char* title) {
    printf("\n");
    printf("\t\t   --------------------------------\n");
    printf("\t\t| %-32s |\n", title);
    printf("\t\t   --------------------------------\n");
}

void printMessageBox(const char* message) {
    printf("\n");
    printf("\t\t   --------------------------------\n");
    printf("\t\t| %-32s |\n", message);
    printf("\t\t   --------------------------------\n");
}

void pauseScreen() {
    char buffer[8];
    printf("\n\t\tPress Enter to continue...");
    fgets(buffer, sizeof(buffer), stdin);
}

void initializeClassrooms() {
    int i;
    for (i = 0; i < TOTAL_CLASSROOMS; i++) {
        classrooms[i].id = i + 1;
        sprintf(classrooms[i].name, "Classroom %d", i + 1);
        classrooms[i].isOccupied = 0;
        strcpy(classrooms[i].borrower, "");
        strcpy(classrooms[i].student_id, "");
    }
}

void displayAllClassrooms() {
    int i;
    printf("\n\t\tCurrent classroom status:\n");
    printf("\t\t-------------------------------------------------------------\n");
    for (i = 0; i < TOTAL_CLASSROOMS; i++) {
        if (classrooms[i].isOccupied) {
            printf("\t\t%-12s Status: In Use   Borrower: %-10s ID: %-12s\n",
                   classrooms[i].name,
                   classrooms[i].borrower,
                   classrooms[i].student_id);
        } else {
            printf("\t\t%-12s Status: Available\n", classrooms[i].name);
        }
    }
    printf("\t\t-------------------------------------------------------------\n");
}

int getIntInput(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    char extra;

    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printMessageBox("Input failed. Try again.");
            continue;
        }
        if (sscanf(buffer, "%d %c", &value, &extra) != 1) {
            printMessageBox("Please enter a valid number.");
            continue;
        }
        if (value < min || value > max) {
            printf("\t\tOut of range. Enter a number between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

void getStringInput(const char* prompt, char* buffer, int maxLen) {
    while (1) {
        size_t len;
        printf("%s", prompt);
        if (fgets(buffer, maxLen, stdin) == NULL) {
            printMessageBox("Input failed. Try again.");
            continue;
        }
        len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }
        }
        if (strlen(buffer) == 0) {
            printMessageBox("Input cannot be empty.");
            continue;
        }
        return;
    }
}

int isValidClassroomNumber(int num) {
    return num >= 1 && num <= TOTAL_CLASSROOMS;
}

char* getClassroomNameByNumber(int num) {
    if (isValidClassroomNumber(num)) {
        return classrooms[num - 1].name;
    }
    return NULL;
}

char* findClassroomName(const char* input) {
    int number;
    int i;
    char extra;

    if (sscanf(input, "%d %c", &number, &extra) == 1) {
        return getClassroomNameByNumber(number);
    }

    for (i = 0; i < TOTAL_CLASSROOMS; i++) {
        if (strcmp(classrooms[i].name, input) == 0) {
            return classrooms[i].name;
        }
    }

    return NULL;
}

int parseDateTimeInput(const char* input, time_t* result) {
    int month;
    int day;
    int hour;
    struct tm timeInfo;
    time_t now = time(NULL);
    struct tm* current = localtime(&now);

    if (sscanf(input, "%d-%d-%d", &month, &day, &hour) != 3) {
        return 0;
    }

    memset(&timeInfo, 0, sizeof(timeInfo));
    timeInfo.tm_year = current->tm_year; // Use current year
    timeInfo.tm_mon = month - 1;
    timeInfo.tm_mday = day;
    timeInfo.tm_hour = hour;
    timeInfo.tm_min = 0; // Default to 0 minutes
    timeInfo.tm_sec = 0; // Default to 0 seconds
    timeInfo.tm_isdst = -1;

    *result = mktime(&timeInfo);
    return *result != (time_t)-1;
}

int isRoomReservedAtTime(const char* classroom, time_t checkTime) {
    int i;
    for (i = 0; i < recordCount; i++) {
        if (records[i].isReservation && strcmp(records[i].classroom, classroom) == 0) {
            if (checkTime >= records[i].reserveStartTime && checkTime <= records[i].reserveEndTime) {
                return 1;
            }
        }
    }
    return 0;
}

int hasReservationConflict(const char* classroom, time_t startTime, time_t endTime) {
    int i;
    for (i = 0; i < recordCount; i++) {
        if (records[i].isReservation && strcmp(records[i].classroom, classroom) == 0) {
            if (!(endTime < records[i].reserveStartTime || startTime > records[i].reserveEndTime)) {
                return 1;
            }
        }
    }
    return 0;
}

void addRecord(const char* classroom, const char* borrower, const char* student_id) {
    int found = 0;
    int i;
    char borrowTime[50];

    for (i = 0; i < TOTAL_CLASSROOMS; i++) {
        if (strcmp(classrooms[i].name, classroom) == 0) {
            if (classrooms[i].isOccupied) {
                printMessageBox("This room is already occupied.");
                return;
            }
            classrooms[i].isOccupied = 1;
            strcpy(classrooms[i].borrower, borrower);
            strcpy(classrooms[i].student_id, student_id);
            found = 1;
            break;
        }
    }

    if (!found) {
        printMessageBox("Room not found.");
        return;
    }

    if (recordCount >= MAX_RECORDS) {
        printMessageBox("Record limit reached.");
        return;
    }

    strcpy(records[recordCount].classroom, classroom);
    strcpy(records[recordCount].borrower, borrower);
    strcpy(records[recordCount].student_id, student_id);
    records[recordCount].isReturned = 0;
    records[recordCount].isReservation = 0;
    records[recordCount].borrowTime = time(NULL);
    records[recordCount].returnTime = 0;
    records[recordCount].reserveStartTime = 0;
    records[recordCount].reserveEndTime = 0;
    formatTimeValue(records[recordCount].borrowTime, borrowTime, sizeof(borrowTime));
    recordCount++;

    printf("\n\t\tBorrow successful: Room=%s User=%s ID=%s Time=%s\n",
           classroom,
           borrower,
           student_id,
           borrowTime);
}

void addReservationRecord(const char* classroom, const char* borrower, const char* student_id, time_t startTime, time_t endTime) {
    if (recordCount >= MAX_RECORDS) {
        printMessageBox("Record limit reached.");
        return;
    }

    strcpy(records[recordCount].classroom, classroom);
    strcpy(records[recordCount].borrower, borrower);
    strcpy(records[recordCount].student_id, student_id);
    records[recordCount].isReturned = 0;
    records[recordCount].isReservation = 1;
    records[recordCount].borrowTime = 0;
    records[recordCount].returnTime = 0;
    records[recordCount].reserveStartTime = startTime;
    records[recordCount].reserveEndTime = endTime;
    recordCount++;
}

void returnClassroomRecord(const char* classroom) {
    int found = 0;
    int i;
    char returnTime[50];

    for (i = 0; i < TOTAL_CLASSROOMS; i++) {
        if (strcmp(classrooms[i].name, classroom) == 0) {
            if (!classrooms[i].isOccupied) {
                printMessageBox("This room is not occupied.");
                return;
            }
            classrooms[i].isOccupied = 0;
            strcpy(classrooms[i].borrower, "");
            strcpy(classrooms[i].student_id, "");
            found = 1;
            break;
        }
    }

    if (!found) {
        printMessageBox("Room not found.");
        return;
    }

    for (i = recordCount - 1; i >= 0; i--) {
        if (!records[i].isReservation && strcmp(records[i].classroom, classroom) == 0 && !records[i].isReturned) {
            records[i].isReturned = 1;
            records[i].returnTime = time(NULL);
            formatTimeValue(records[i].returnTime, returnTime, sizeof(returnTime));
            printf("\n\t\tReturn successful: Room=%s Return Time=%s\n", classroom, returnTime);
            return;
        }
    }

    printMessageBox("No active borrowing record found.");
}

void displayAllRecords() {
    int i;

    if (recordCount == 0) {
        printMessageBox("No records found.");
        return;
    }

    printf("\n\t\t---------------- Record List ----------------\n");
    for (i = 0; i < recordCount; i++) {
        printf("\t\tRecord %d\n", i + 1);
        displaySingleRecord(&records[i]);
    }
}

void displayRecordsByClassroom(const char* classroom) {
    int i;
    int found = 0;
    char* classroomName;

    classroomName = findClassroomName(classroom);
    if (classroomName == NULL) {
        printMessageBox("Invalid or unknown room.");
        return;
    }

    printf("\n\t\t---------------- Room Query Result ----------------\n");
    for (i = 0; i < recordCount; i++) {
        if (strcmp(records[i].classroom, classroomName) == 0) {
            displaySingleRecord(&records[i]);
            found = 1;
        }
    }

    if (!found) {
        printMessageBox("No history found for this room.");
    }
}

void displayRecordsByBorrower(const char* borrower) {
    int i;
    int found = 0;

    printf("\n\t\t---------------- User Query Result ----------------\n");
    for (i = 0; i < recordCount; i++) {
        if (strcmp(records[i].borrower, borrower) == 0) {
            displaySingleRecord(&records[i]);
            found = 1;
        }
    }

    if (!found) {
        printMessageBox("No record found for this user.");
    }
}

void displayRecordsByStudentId(const char* studentId) {
    int i;
    int found = 0;

    printf("\n\t\t-------------- Student ID Query Result --------------\n");
    for (i = 0; i < recordCount; i++) {
        if (strcmp(records[i].student_id, studentId) == 0) {
            displaySingleRecord(&records[i]);
            found = 1;
        }
    }

    if (!found) {
        printMessageBox("No record found for this student ID.");
    }
}

