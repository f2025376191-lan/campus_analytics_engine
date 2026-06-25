#ifndef FEE_TRACKER_H
#define FEE_TRACKER_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "filehandler.h"
#include "student_ops.h"
using namespace std;

struct FeeDefaulter
{
    string roll;
    string name;
    string semester;
    double outstanding;
    int weeksOverdue;
};

bool recordPayment(string roll, string semester, double amount, string paidDate);
double computeLateFine(string roll, string semester);
int daysBetween(string firstDate, string secondDate);
void generateReceipt(string roll, string semester);
vector<FeeDefaulter> getDefaulters(string currentDate);

#endif
