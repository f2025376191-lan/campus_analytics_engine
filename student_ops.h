#ifndef STUDENT_OPS_H
#define STUDENT_OPS_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "filehandler.h"
using namespace std;

bool validateRoll(string roll);
bool addStudent(string roll, string name, string dept, double cgpa);
vector<string> searchByRoll(string roll);
vector<vector<string> > searchByName(string namePart);
bool updateStudent(string roll, int fieldIndex, string newValue);
bool softDelete(string roll);
vector<vector<string> > listActiveStudents();
bool isActiveStudent(string roll);
void printStudentRow(vector<string> row);
void printStudentTable(vector<vector<string> > rows);
void searchAsYouType();

#endif
