#ifndef COURSE_OPS_H
#define COURSE_OPS_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "filehandler.h"
#include "student_ops.h"
using namespace std;

enum EnrollResult
{
    ENROLL_SUCCESS,
    ENROLL_STUDENT_INACTIVE,
    ENROLL_COURSE_NOT_FOUND,
    ENROLL_SEATS_FULL,
    ENROLL_ALREADY_EXISTS,
    ENROLL_CREDIT_OVERLOAD,
    ENROLL_PREREQ_FAIL
};

EnrollResult enrollStudent(string roll, string courseCode, string semester);
bool dropCourse(string roll, string courseCode, string semester);
int getCreditLoad(string roll, string semester);
bool checkPrerequisite(string roll, string courseCode);
vector<vector<string> > listEnrolledStudents(string courseCode, string semester);
string enrollResultMessage(EnrollResult result);
bool courseExists(string courseCode);
vector<string> getCourseRow(string courseCode);
int getCourseCredits(string courseCode);
void printCourseTable();
void printEnrolledTable(string courseCode, string semester);

#endif
