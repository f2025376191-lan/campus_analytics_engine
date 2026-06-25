#ifndef REPORTS_H
#define REPORTS_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include "grades.h"
#include "attendance.h"
#include "fee_tracker.h"
using namespace std;

void printMeritList();
void printAttendanceDefaulters(string courseCode, string semester);
void printFeeDefaulters(string currentDate);
void printSemesterResult(string semester);
void printDepartmentSummary();
void exportReportToFile(int reportChoice, string filename, string semester, string courseCode, string currentDate);

#endif
