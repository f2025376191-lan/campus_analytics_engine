#include <iostream>
#include <string>
#include "filehandler.h"
#include "student_ops.h"
#include "course_ops.h"
#include "attendance.h"
#include "grades.h"
#include "fee_tracker.h"
#include "reports.h"
using namespace std;

void waitForEnter()
{
    cout << "\nPress Enter to continue...";
    cin.get();
}

int readChoice()
{
    int choice;
    cin >> choice;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        return -1;
    }
    cin.ignore(1000, '\n');
    return choice;
}

void studentMenu()
{
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n===== STUDENT MODULE =====" << endl;
        cout << "1. Add student" << endl;
        cout << "2. Search by roll" << endl;
        cout << "3. Search by name" << endl;
        cout << "4. Search as you type" << endl;
        cout << "5. Update student" << endl;
        cout << "6. Soft delete student" << endl;
        cout << "7. List active students" << endl;
        cout << "0. Back" << endl;
        cout << "Choice: ";
        choice = readChoice();

        if (choice == 1)
        {
            string roll, name, dept;
            double cgpa;
            cout << "Roll (BSAI-YY-XXX): ";
            getline(cin, roll);
            cout << "Name: ";
            getline(cin, name);
            cout << "Department: ";
            getline(cin, dept);
            cout << "CGPA: ";
            cin >> cgpa;
            cin.ignore(1000, '\n');
            addStudent(roll, name, dept, cgpa);
            waitForEnter();
        }
        else if (choice == 2)
        {
            string roll;
            cout << "Roll: ";
            getline(cin, roll);
            vector<string> row = searchByRoll(roll);
            if ((int)row.size() == 0)
            {
                cout << "Student not found." << endl;
            }
            else
            {
                vector<vector<string> > temp;
                temp.push_back(row);
                printStudentTable(temp);
            }
            waitForEnter();
        }
        else if (choice == 3)
        {
            string name;
            cout << "Name text: ";
            getline(cin, name);
            printStudentTable(searchByName(name));
            waitForEnter();
        }
        else if (choice == 4)
        {
            searchAsYouType();
            cin.ignore(1000, '\n');
            waitForEnter();
        }
        else if (choice == 5)
        {
            string roll, value;
            int field;
            cout << "Roll: ";
            getline(cin, roll);
            cout << "Field index: 1=name, 2=dept, 3=cgpa, 4=status: ";
            cin >> field;
            cin.ignore(1000, '\n');
            cout << "New value: ";
            getline(cin, value);
            updateStudent(roll, field, value);
            waitForEnter();
        }
        else if (choice == 6)
        {
            string roll;
            cout << "Roll: ";
            getline(cin, roll);
            softDelete(roll);
            waitForEnter();
        }
        else if (choice == 7)
        {
            printStudentTable(listActiveStudents());
            waitForEnter();
        }
    }
}

void courseMenu()
{
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n===== COURSE AND ENROLLMENT MODULE =====" << endl;
        cout << "1. List courses" << endl;
        cout << "2. Enroll student" << endl;
        cout << "3. Drop course" << endl;
        cout << "4. Check credit load" << endl;
        cout << "5. Check prerequisite" << endl;
        cout << "6. List enrolled students" << endl;
        cout << "0. Back" << endl;
        cout << "Choice: ";
        choice = readChoice();

        if (choice == 1)
        {
            printCourseTable();
            waitForEnter();
        }
        else if (choice == 2)
        {
            string roll, course, sem;
            cout << "Roll: ";
            getline(cin, roll);
            cout << "Course code: ";
            getline(cin, course);
            cout << "Semester: ";
            getline(cin, sem);
            EnrollResult result = enrollStudent(roll, course, sem);
            cout << enrollResultMessage(result) << endl;
            waitForEnter();
        }
        else if (choice == 3)
        {
            string roll, course, sem;
            cout << "Roll: ";
            getline(cin, roll);
            cout << "Course code: ";
            getline(cin, course);
            cout << "Semester: ";
            getline(cin, sem);
            dropCourse(roll, course, sem);
            waitForEnter();
        }
        else if (choice == 4)
        {
            string roll, sem;
            cout << "Roll: ";
            getline(cin, roll);
            cout << "Semester: ";
            getline(cin, sem);
            cout << "Credit load = " << getCreditLoad(roll, sem) << endl;
            waitForEnter();
        }
        else if (choice == 5)
        {
            string roll, course;
            cout << "Roll: ";
            getline(cin, roll);
            cout << "Course code: ";
            getline(cin, course);
            if (checkPrerequisite(roll, course) == true)
            {
                cout << "Prerequisite passed or not required." << endl;
            }
            else
            {
                cout << "Prerequisite not passed." << endl;
            }
            waitForEnter();
        }
        else if (choice == 6)
        {
            string course, sem;
            cout << "Course code: ";
            getline(cin, course);
            cout << "Semester: ";
            getline(cin, sem);
            printEnrolledTable(course, sem);
            waitForEnter();
        }
    }
}

void attendanceMenu()
{
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n===== ATTENDANCE MODULE =====" << endl;
        cout << "1. Mark attendance" << endl;
        cout << "2. Get attendance percentage" << endl;
        cout << "3. Shortage list" << endl;
        cout << "4. Undo last session" << endl;
        cout << "5. Print daily sheet" << endl;
        cout << "0. Back" << endl;
        cout << "Choice: ";
        choice = readChoice();

        if (choice == 1)
        {
            string course, sem, date;
            cout << "Course code: ";
            getline(cin, course);
            cout << "Semester: ";
            getline(cin, sem);
            cout << "Date DD-MM-YYYY: ";
            getline(cin, date);
            markAttendance(course, sem, date);
            waitForEnter();
        }
        else if (choice == 2)
        {
            string roll, course;
            cout << "Roll: ";
            getline(cin, roll);
            cout << "Course code: ";
            getline(cin, course);
            cout << "Attendance = " << doubleToString(getAttendancePct(roll, course)) << "%" << endl;
            waitForEnter();
        }
        else if (choice == 3)
        {
            string course, sem;
            cout << "Course code: ";
            getline(cin, course);
            cout << "Semester: ";
            getline(cin, sem);
            vector<vector<string> > rows = getShortageList(course, sem);
            printStudentTable(rows);
            waitForEnter();
        }
        else if (choice == 4)
        {
            undoLastSession();
            waitForEnter();
        }
        else if (choice == 5)
        {
            string course, sem, date;
            cout << "Course code: ";
            getline(cin, course);
            cout << "Semester: ";
            getline(cin, sem);
            cout << "Date DD-MM-YYYY: ";
            getline(cin, date);
            printDailySheet(course, sem, date);
            waitForEnter();
        }
    }
}

void gradesMenu()
{
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n===== GRADES MODULE =====" << endl;
        cout << "1. Enter marks" << endl;
        cout << "2. Compute GPA" << endl;
        cout << "3. Class statistics" << endl;
        cout << "4. Apply attendance penalty" << endl;
        cout << "0. Back" << endl;
        cout << "Choice: ";
        choice = readChoice();

        if (choice == 1)
        {
            string roll, course, sem;
            double quizzes[5];
            double assignments[10];
            int quizCount, assignmentCount;
            double mid, finalExam;

            cout << "Roll: ";
            getline(cin, roll);
            cout << "Course code: ";
            getline(cin, course);
            cout << "Semester: ";
            getline(cin, sem);
            cout << "Number of quizzes up to 5: ";
            cin >> quizCount;

            for (int i = 0; i < quizCount && i < 5; i++)
            {
                cout << "Quiz " << i + 1 << " out of 100: ";
                cin >> quizzes[i];
            }

            cout << "Number of assignments up to 10: ";
            cin >> assignmentCount;
            for (int i = 0; i < assignmentCount && i < 10; i++)
            {
                cout << "Assignment " << i + 1 << " out of 100: ";
                cin >> assignments[i];
            }

            cout << "Mid marks out of 40: ";
            cin >> mid;
            cout << "Final marks out of 60: ";
            cin >> finalExam;
            cin.ignore(1000, '\n');

            if (assignmentCount > 10)
            {
                assignmentCount = 10;
            }
            if (quizCount > 5)
            {
                quizCount = 5;
            }

            enterMarks(roll, course, sem, quizzes, quizCount, assignments, assignmentCount, mid, finalExam);
            waitForEnter();
        }
        else if (choice == 2)
        {
            string roll, sem;
            cout << "Roll: ";
            getline(cin, roll);
            cout << "Semester: ";
            getline(cin, sem);
            cout << "GPA = " << doubleToString(computeGPA(roll, sem)) << endl;
            waitForEnter();
        }
        else if (choice == 3)
        {
            string course, sem;
            cout << "Course code: ";
            getline(cin, course);
            cout << "Semester: ";
            getline(cin, sem);
            Stats st = computeClassState(course, sem);
            cout << "Highest: " << doubleToString(st.highest) << endl;
            cout << "Lowest : " << doubleToString(st.lowest) << endl;
            cout << "Mean   : " << doubleToString(st.mean) << endl;
            cout << "Median : " << doubleToString(st.median) << endl;
            waitForEnter();
        }
        else if (choice == 4)
        {
            string roll, course, sem;
            cout << "Roll: ";
            getline(cin, roll);
            cout << "Course code: ";
            getline(cin, course);
            cout << "Semester: ";
            getline(cin, sem);
            if (applyAttendancePenalty(roll, course, sem) == true)
            {
                cout << "Attendance penalty applied. Grade changed to F." << endl;
            }
            else
            {
                cout << "No penalty applied." << endl;
            }
            waitForEnter();
        }
    }
}

void feeMenu()
{
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n===== FEE MODULE =====" << endl;
        cout << "1. Record payment" << endl;
        cout << "2. Compute late fine" << endl;
        cout << "3. Generate receipt" << endl;
        cout << "4. Fee defaulters" << endl;
        cout << "0. Back" << endl;
        cout << "Choice: ";
        choice = readChoice();

        if (choice == 1)
        {
            string roll, sem, date;
            double amount;
            cout << "Roll: ";
            getline(cin, roll);
            cout << "Semester: ";
            getline(cin, sem);
            cout << "Amount: ";
            cin >> amount;
            cin.ignore(1000, '\n');
            cout << "Paid date DD-MM-YYYY: ";
            getline(cin, date);
            recordPayment(roll, sem, amount, date);
            waitForEnter();
        }
        else if (choice == 2)
        {
            string roll, sem;
            cout << "Roll: ";
            getline(cin, roll);
            cout << "Semester: ";
            getline(cin, sem);
            cout << "Late fine = " << doubleToString(computeLateFine(roll, sem)) << endl;
            waitForEnter();
        }
        else if (choice == 3)
        {
            string roll, sem;
            cout << "Roll: ";
            getline(cin, roll);
            cout << "Semester: ";
            getline(cin, sem);
            generateReceipt(roll, sem);
            waitForEnter();
        }
        else if (choice == 4)
        {
            string date;
            cout << "Current date DD-MM-YYYY: ";
            getline(cin, date);
            printFeeDefaulters(date);
            waitForEnter();
        }
    }
}

void reportsMenu()
{
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n===== REPORTS MODULE =====" << endl;
        cout << "1. Merit list" << endl;
        cout << "2. Attendance defaulters" << endl;
        cout << "3. Fee defaulters" << endl;
        cout << "4. Semester result" << endl;
        cout << "5. Department summary" << endl;
        cout << "6. Export report to file" << endl;
        cout << "0. Back" << endl;
        cout << "Choice: ";
        choice = readChoice();

        if (choice == 1)
        {
            printMeritList();
            waitForEnter();
        }
        else if (choice == 2)
        {
            string course, sem;
            cout << "Course code: ";
            getline(cin, course);
            cout << "Semester: ";
            getline(cin, sem);
            printAttendanceDefaulters(course, sem);
            waitForEnter();
        }
        else if (choice == 3)
        {
            string date;
            cout << "Current date DD-MM-YYYY: ";
            getline(cin, date);
            printFeeDefaulters(date);
            waitForEnter();
        }
        else if (choice == 4)
        {
            string sem;
            cout << "Semester: ";
            getline(cin, sem);
            printSemesterResult(sem);
            waitForEnter();
        }
        else if (choice == 5)
        {
            printDepartmentSummary();
            waitForEnter();
        }
        else if (choice == 6)
        {
            int reportChoice;
            string filename, sem, course, date;
            cout << "Report number 1=Merit, 2=Attendance, 3=Fee, 4=Result, 5=Dept: ";
            cin >> reportChoice;
            cin.ignore(1000, '\n');
            cout << "Output filename: ";
            getline(cin, filename);
            cout << "Semester if needed: ";
            getline(cin, sem);
            cout << "Course code if needed: ";
            getline(cin, course);
            cout << "Current date if needed DD-MM-YYYY: ";
            getline(cin, date);
            exportReportToFile(reportChoice, filename, sem, course, date);
            waitForEnter();
        }
    }
}

int main()
{
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n====================================" << endl;
        cout << "     CAMPUS ANALYTICS ENGINE" << endl;
        cout << "====================================" << endl;
        cout << "1. Students" << endl;
        cout << "2. Courses and Enrollments" << endl;
        cout << "3. Attendance" << endl;
        cout << "4. Grades" << endl;
        cout << "5. Fees" << endl;
        cout << "6. Reports" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
        choice = readChoice();

        if (choice == 1)
        {
            studentMenu();
        }
        else if (choice == 2)
        {
            courseMenu();
        }
        else if (choice == 3)
        {
            attendanceMenu();
        }
        else if (choice == 4)
        {
            gradesMenu();
        }
        else if (choice == 5)
        {
            feeMenu();
        }
        else if (choice == 6)
        {
            reportsMenu();
        }
        else if (choice == 0)
        {
            cout << "Program closed." << endl;
        }
        else
        {
            cout << "Invalid choice." << endl;
        }
    }

    return 0;
}
