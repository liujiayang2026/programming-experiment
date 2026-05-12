#ifndef INCLUDE_H
#define INCLUDE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_RECORDS 100
#define TOTAL_CLASSROOMS 10

typedef struct {
    int id;
    char name[50];
    int isOccupied;
    char borrower[50];
    char student_id[20];
} Classroom;

typedef struct {
    char classroom[50];
    char borrower[50];
    char student_id[20];
    int isReturned;
    int isReservation;
    time_t borrowTime;
    time_t returnTime;
    time_t reserveStartTime;
    time_t reserveEndTime;
} Record;

extern Classroom classrooms[TOTAL_CLASSROOMS];
extern Record records[MAX_RECORDS];
extern int recordCount;

void initializeClassrooms();
void displayMenu();
void displayAllClassrooms();
void addRecord(const char* classroom, const char* borrower, const char* student_id);
void returnClassroomRecord(const char* classroom);
void addReservationRecord(const char* classroom, const char* borrower, const char* student_id, time_t startTime, time_t endTime);
void displayAllRecords();
void displayRecordsByClassroom(const char* classroom);
void displayRecordsByBorrower(const char* borrower);
void displayRecordsByStudentId(const char* studentId);
void reserveClassroom();
void borrowClassroom();
void returnClassroom();
void displayRecords();
void printSectionTitle(const char* title);
void printMessageBox(const char* message);
void pauseScreen();
int getIntInput(const char* prompt, int min, int max);
void getStringInput(const char* prompt, char* buffer, int maxLen);
int isValidClassroomNumber(int num);
char* getClassroomNameByNumber(int num);
char* findClassroomName(const char* input);
int parseDateTimeInput(const char* input, time_t* result);
int isRoomReservedAtTime(const char* classroom, time_t checkTime);
int hasReservationConflict(const char* classroom, time_t startTime, time_t endTime);

#endif
