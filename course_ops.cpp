#include "course_ops.h"

string COURSE_FILE = "courses.txt";
string ENROLL_FILE = "enrollments.txt";

vector<string> getEnrollmentHeader()
{
    vector<string> header;
    header.push_back("roll");
    header.push_back("course_code");
    header.push_back("semester");
    header.push_back("status");
    header.push_back("quiz_avg");
    header.push_back("assignment_avg");
    header.push_back("mid");
    header.push_back("final");
    header.push_back("total");
    header.push_back("grade");
    return header;
}

vector<string> getCourseRow(string courseCode)
{
    return findRow(COURSE_FILE, 0, courseCode);
}

bool courseExists(string courseCode)
{
    vector<string> row = getCourseRow(courseCode);
    if ((int)row.size() >= 5)
    {
        return true;
    }
    return false;
}

int getCourseCredits(string courseCode)
{
    vector<string> row = getCourseRow(courseCode);
    if ((int)row.size() >= 3)
    {
        return stringToInt(row[2]);
    }
    return 0;
}

int countEnrolledInCourse(string courseCode, string semester)
{
    vector<vector<string> > enrollments = readTXT(ENROLL_FILE);
    int count = 0;

    for (int i = 0; i < (int)enrollments.size(); i++)
    {
        if ((int)enrollments[i].size() >= 4)
        {
            if (enrollments[i][1] == courseCode && enrollments[i][2] == semester && enrollments[i][3] == "enrolled")
            {
                count++;
            }
        }
    }

    return count;
}

bool alreadyEnrolled(string roll, string courseCode, string semester)
{
    vector<vector<string> > rows = readTXT(ENROLL_FILE);

    for (int i = 0; i < (int)rows.size(); i++)
    {
        if ((int)rows[i].size() >= 4)
        {
            if (rows[i][0] == roll && rows[i][1] == courseCode && rows[i][2] == semester && rows[i][3] == "enrolled")
            {
                return true;
            }
        }
    }

    return false;
}

int getCreditLoad(string roll, string semester)
{
    vector<vector<string> > enrollments = readTXT(ENROLL_FILE);
    vector<vector<string> > courses = readTXT(COURSE_FILE);
    int totalCredits = 0;

    for (int i = 0; i < (int)enrollments.size(); i++)
    {
        if ((int)enrollments[i].size() >= 4)
        {
            if (enrollments[i][0] == roll && enrollments[i][2] == semester && enrollments[i][3] == "enrolled")
            {
                for (int j = 0; j < (int)courses.size(); j++)
                {
                    if ((int)courses[j].size() >= 3 && courses[j][0] == enrollments[i][1])
                    {
                        totalCredits = totalCredits + stringToInt(courses[j][2]);
                    }
                }
            }
        }
    }

    return totalCredits;
}

bool checkPrerequisite(string roll, string courseCode)
{
    vector<string> course = getCourseRow(courseCode);

    if ((int)course.size() < 5)
    {
        return false;
    }

    string prereq = course[4];

    if (prereq == "NONE")
    {
        return true;
    }

    vector<vector<string> > enrollments = readTXT(ENROLL_FILE);

    for (int i = 0; i < (int)enrollments.size(); i++)
    {
        if ((int)enrollments[i].size() >= 10)
        {
            if (enrollments[i][0] == roll && enrollments[i][1] == prereq)
            {
                if (enrollments[i][9] != "" && enrollments[i][9] != "F")
                {
                    return true;
                }
            }
        }
    }

    return false;
}

EnrollResult enrollStudent(string roll, string courseCode, string semester)
{
    if (isActiveStudent(roll) == false)
    {
        return ENROLL_STUDENT_INACTIVE;
    }

    vector<string> course = getCourseRow(courseCode);
    if ((int)course.size() < 5)
    {
        return ENROLL_COURSE_NOT_FOUND;
    }

    int seats = stringToInt(course[3]);
    int usedSeats = countEnrolledInCourse(courseCode, semester);

    if (usedSeats >= seats)
    {
        return ENROLL_SEATS_FULL;
    }

    if (alreadyEnrolled(roll, courseCode, semester) == true)
    {
        return ENROLL_ALREADY_EXISTS;
    }

    int currentLoad = getCreditLoad(roll, semester);
    int newCredits = stringToInt(course[2]);

    if (currentLoad + newCredits > 21)
    {
        return ENROLL_CREDIT_OVERLOAD;
    }

    if (checkPrerequisite(roll, courseCode) == false)
    {
        return ENROLL_PREREQ_FAIL;
    }

    vector<string> row;
    row.push_back(roll);
    row.push_back(courseCode);
    row.push_back(semester);
    row.push_back("enrolled");
    row.push_back("");
    row.push_back("");
    row.push_back("");
    row.push_back("");
    row.push_back("");
    row.push_back("");

    appendTXT(ENROLL_FILE, row);
    return ENROLL_SUCCESS;
}

bool dropCourse(string roll, string courseCode, string semester)
{
    vector<vector<string> > attendanceRows = readTXT("attendance_log.txt");

    for (int i = 0; i < (int)attendanceRows.size(); i++)
    {
        if ((int)attendanceRows[i].size() >= 4)
        {
            if (attendanceRows[i][0] == roll && attendanceRows[i][1] == courseCode)
            {
                cout << "Course cannot be dropped because attendance record exists." << endl;
                return false;
            }
        }
    }

    vector<vector<string> > rows = readTXT(ENROLL_FILE);
    bool found = false;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        if ((int)rows[i].size() >= 4)
        {
            if (rows[i][0] == roll && rows[i][1] == courseCode && rows[i][2] == semester && rows[i][3] == "enrolled")
            {
                rows[i][3] = "dropped";
                found = true;
            }
        }
    }

    if (found == false)
    {
        cout << "Active enrollment not found." << endl;
        return false;
    }

    writeTXT(ENROLL_FILE, getEnrollmentHeader(), rows);
    cout << "Course dropped successfully." << endl;
    return true;
}

vector<vector<string> > listEnrolledStudents(string courseCode, string semester)
{
    vector<vector<string> > enrollments = readTXT(ENROLL_FILE);
    vector<vector<string> > students;

    for (int i = 0; i < (int)enrollments.size(); i++)
    {
        if ((int)enrollments[i].size() >= 4)
        {
            if (enrollments[i][1] == courseCode && enrollments[i][2] == semester && enrollments[i][3] == "enrolled")
            {
                vector<string> student = searchByRoll(enrollments[i][0]);
                if ((int)student.size() >= 5 && student[4] == "active")
                {
                    students.push_back(student);
                }
            }
        }
    }

    return students;
}

string enrollResultMessage(EnrollResult result)
{
    if (result == ENROLL_SUCCESS) return "Enrollment successful.";
    if (result == ENROLL_STUDENT_INACTIVE) return "Student is not active or does not exist.";
    if (result == ENROLL_COURSE_NOT_FOUND) return "Course does not exist.";
    if (result == ENROLL_SEATS_FULL) return "Course seats are full.";
    if (result == ENROLL_ALREADY_EXISTS) return "Student already enrolled in this course.";
    if (result == ENROLL_CREDIT_OVERLOAD) return "Credit load cannot be more than 21 hours.";
    if (result == ENROLL_PREREQ_FAIL) return "Prerequisite course not passed.";
    return "Unknown enrollment result.";
}

void printCourseTable()
{
    vector<vector<string> > courses = readTXT(COURSE_FILE);

    cout << left << setw(12) << "Code"
         << setw(26) << "Title"
         << setw(10) << "Credits"
         << setw(8) << "Seats"
         << setw(12) << "Prereq" << endl;
    cout << "--------------------------------------------------------------------" << endl;

    for (int i = 0; i < (int)courses.size(); i++)
    {
        if ((int)courses[i].size() >= 5)
        {
            cout << left << setw(12) << courses[i][0]
                 << setw(26) << courses[i][1]
                 << setw(10) << courses[i][2]
                 << setw(8) << courses[i][3]
                 << setw(12) << courses[i][4] << endl;
        }
    }
}

void printEnrolledTable(string courseCode, string semester)
{
    vector<vector<string> > rows = listEnrolledStudents(courseCode, semester);
    printStudentTable(rows);
}
