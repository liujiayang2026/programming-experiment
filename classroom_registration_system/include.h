#ifndef INCLUDE_H
#define INCLUDE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
} Record;

extern Classroom classrooms[TOTAL_CLASSROOMS];
extern Record records[MAX_RECORDS];
extern int recordCount;

void initializeClassrooms();
void displayAllClassrooms();
void addRecord(const char* classroom, const char* borrower, const char* student_id);
void returnClassroomRecord(const char* classroom);
void displayAllRecords();
void borrowClassroom();
void returnClassroom();
void displayRecords();
int getIntInput(const char* prompt, int min, int max);
void getStringInput(const char* prompt, char* buffer, int maxLen);
int isValidClassroomNumber(int num);
char* getClassroomNameByNumber(int num);

#endif // INCLUDE_H
