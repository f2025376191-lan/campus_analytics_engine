#include "reports.h"

void printMeritList()
{
    vector<vector<string> > rows = listActiveStudents();

    for (int i = 0; i < (int)rows.size() - 1; i++)
    {
        int maxIndex = i;
        for (int j = i + 1; j < (int)rows.size(); j++)
        {
            double a = stringToDouble(rows[j][3]);
            double b = stringToDouble(rows[maxIndex][3]);
            if (a > b)
            {
                maxIndex = j;
            }
        }
        if (maxIndex != i)
        {
            vector<string> temp = rows[i];
            rows[i] = rows[maxIndex];
            rows[maxIndex] = temp;
        }
    }

    cout << left << setw(8) << "Rank"
         << setw(14) << "Roll"
         << setw(24) << "Name"
         << setw(12) << "Dept"
         << setw(8) << "CGPA" << endl;
    cout << "----------------------------------------------------------------" << endl;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        cout << left << setw(8) << i + 1
             << setw(14) << rows[i][0]
             << setw(24) << rows[i][1]
             << setw(12) << rows[i][2]
             << setw(8) << rows[i][3] << endl;
    }
}

void printAttendanceDefaulters(string courseCode, string semester)
{
    vector<vector<string> > rows = getShortageList(courseCode, semester);

    cout << left << setw(14) << "Roll"
         << setw(24) << "Name"
         << setw(14) << "Course"
         << setw(14) << "Attendance" << endl;
    cout << "----------------------------------------------------------------" << endl;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        cout << left << setw(14) << rows[i][0]
             << setw(24) << rows[i][1]
             << setw(14) << courseCode
             << setw(14) << doubleToString(getAttendancePct(rows[i][0], courseCode)) << endl;
    }
}

void printFeeDefaulters(string currentDate)
{
    vector<FeeDefaulter> rows = getDefaulters(currentDate);

    cout << left << setw(14) << "Roll"
         << setw(24) << "Name"
         << setw(12) << "Semester"
         << setw(14) << "Outstanding"
         << setw(14) << "Weeks Late" << endl;
    cout << "----------------------------------------------------------------------------" << endl;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        cout << left << setw(14) << rows[i].roll
             << setw(24) << rows[i].name
             << setw(12) << rows[i].semester
             << setw(14) << doubleToString(rows[i].outstanding)
             << setw(14) << rows[i].weeksOverdue << endl;
    }
}

void printSemesterResult(string semester)
{
    vector<vector<string> > rows = readTXT("enrollments.txt");

    cout << setfill('=') << setw(98) << "" << setfill(' ') << endl;
    cout << "SEMESTER RESULT SHEET: " << semester << endl;
    cout << setfill('=') << setw(98) << "" << setfill(' ') << endl;
    cout << left << setw(14) << "Roll"
         << setw(22) << "Name"
         << setw(12) << "Course"
         << setw(10) << "Total"
         << setw(8) << "Grade"
         << setw(8) << "GPA"
         << setw(16) << "Attendance" << endl;
    cout << "--------------------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        while ((int)rows[i].size() < 10)
        {
            rows[i].push_back("");
        }

        if (rows[i][2] == semester && rows[i][3] == "enrolled")
        {
            vector<string> st = searchByRoll(rows[i][0]);
            string name = "Unknown";
            if ((int)st.size() >= 2)
            {
                name = st[1];
            }

            double pct = getAttendancePct(rows[i][0], rows[i][1]);
            string attStatus = "OK";
            if (pct < 75.0)
            {
                attStatus = "Shortage";
            }

            cout << left << setw(14) << rows[i][0]
                 << setw(22) << name
                 << setw(12) << rows[i][1]
                 << setw(10) << rows[i][8]
                 << setw(8) << rows[i][9]
                 << setw(8) << doubleToString(computeGPA(rows[i][0], semester))
                 << setw(16) << attStatus << endl;
        }
    }

    cout << setfill('=') << setw(98) << "" << setfill(' ') << endl;
}

void printDepartmentSummary()
{
    vector<vector<string> > students = listActiveStudents();
    string depts[100];
    int counts[100];
    double cgpaSums[100];
    int passCounts[100];
    int deptCount = 0;

    for (int i = 0; i < 100; i++)
    {
        depts[i] = "";
        counts[i] = 0;
        cgpaSums[i] = 0;
        passCounts[i] = 0;
    }

    for (int i = 0; i < (int)students.size(); i++)
    {
        string dept = students[i][2];
        int index = -1;

        for (int j = 0; j < deptCount; j++)
        {
            if (depts[j] == dept)
            {
                index = j;
            }
        }

        if (index == -1 && deptCount < 100)
        {
            index = deptCount;
            depts[index] = dept;
            deptCount++;
        }

        if (index != -1)
        {
            counts[index]++;
            double cgpa = stringToDouble(students[i][3]);
            cgpaSums[index] = cgpaSums[index] + cgpa;
            if (cgpa >= 2.0)
            {
                passCounts[index]++;
            }
        }
    }

    cout << left << setw(14) << "Dept"
         << setw(10) << "Count"
         << setw(14) << "Avg CGPA"
         << setw(14) << "Pass Rate" << endl;
    cout << "---------------------------------------------------" << endl;

    for (int i = 0; i < deptCount; i++)
    {
        double avg = 0;
        double passRate = 0;
        if (counts[i] > 0)
        {
            avg = cgpaSums[i] / counts[i];
            passRate = (passCounts[i] * 100.0) / counts[i];
        }

        cout << left << setw(14) << depts[i]
             << setw(10) << counts[i]
             << setw(14) << doubleToString(avg)
             << setw(14) << doubleToString(passRate) << endl;
    }
}

void exportReportToFile(int reportChoice, string filename, string semester, string courseCode, string currentDate)
{
    ofstream fout(filename.c_str());

    if (!fout.is_open())
    {
        cout << "Cannot open output file." << endl;
        return;
    }

    streambuf *oldBuffer = cout.rdbuf();
    cout.rdbuf(fout.rdbuf());

    if (reportChoice == 1)
    {
        printMeritList();
    }
    else if (reportChoice == 2)
    {
        printAttendanceDefaulters(courseCode, semester);
    }
    else if (reportChoice == 3)
    {
        printFeeDefaulters(currentDate);
    }
    else if (reportChoice == 4)
    {
        printSemesterResult(semester);
    }
    else if (reportChoice == 5)
    {
        printDepartmentSummary();
    }
    else
    {
        cout << "Invalid report choice." << endl;
    }

    cout.rdbuf(oldBuffer);
    fout.close();
    cout << "Report saved to " << filename << endl;
}
