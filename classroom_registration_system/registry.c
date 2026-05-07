#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include.h"

void borrowClassroom() {
    displayAllClassrooms();
    int classroomNum;
    char borrower[50];
    char student_id[20];
    char* classroomName;
    
    printf("\nEnter classroom number (1-10): ");
    classroomNum = getIntInput("", 1, 10);
    classroomName = getClassroomNameByNumber(classroomNum);
    
    getStringInput("Enter your name: ", borrower, sizeof(borrower));
    getStringInput("Enter your student ID: ", student_id, sizeof(student_id));
    
    addRecord(classroomName, borrower, student_id);
    displayAllClassrooms();
}

void returnClassroom() {
    displayAllClassrooms();
    int classroomNum;
    char* classroomName;
    
    printf("\nEnter classroom number to return (1-10): ");
    classroomNum = getIntInput("", 1, 10);
    classroomName = getClassroomNameByNumber(classroomNum);
    
    returnClassroomRecord(classroomName);
    displayAllClassrooms();
}

void displayRecords() {
    displayAllClassrooms();
    displayAllRecords();
}
