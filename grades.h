#ifndef GRADES_H
#define GRADES_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "filehandler.h"
#include "student_ops.h"
#include "attendance.h"
using namespace std;

struct Stats
{
    double highest;
    double lowest;
    double mean;
    double median;
};

bool enterMarks(string roll, string courseCode, string semester, double quizzes[], int quizCount, double assignments[], int assignmentCount, double mid, double finalExam);
double bestThreeOfFive(double marks[], int n);
double computeWeightedTotal(double quizAvg, double assignmentAvg, double mid, double finalExam);
string getLetterGrade(double total);
double computeGPA(string roll, string semester);
Stats computeClassState(string courseCode, string semester);
bool applyAttendancePenalty(string roll, string courseCode, string semester);
double gradePoint(string grade);

#endif
