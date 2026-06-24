#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "filehandler.h"
#include "course_ops.h"
using namespace std;

bool markAttendance(string courseCode, string semester, string date);
double getAttendancePct(string roll, string courseCode);
vector<vector<string> > getShortageList(string courseCode, string semester);
bool undoLastSession();
void printDailySheet(string courseCode, string semester, string date);

#endif
