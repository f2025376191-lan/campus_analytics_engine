#include "grades.h"

string GRADE_ENROLL_FILE = "enrollments.txt";

vector<string> gradeEnrollmentHeader()
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

void makeEnrollmentRowFull(vector<string> &row)
{
    while ((int)row.size() < 10)
    {
        row.push_back("");
    }
}

bool validRange(double value, double minValue, double maxValue)
{
    if (value < minValue || value > maxValue)
    {
        return false;
    }
    return true;
}

double bestThreeOfFive(double marks[], int n)
{
    if (n <= 0)
    {
        return 0.0;
    }

    if (n < 3)
    {
        double sum = 0;
        for (int i = 0; i < n; i++)
        {
            sum = sum + marks[i];
        }
        return sum / n;
    }

    int low1 = 0;
    int low2 = 1;

    if (marks[low2] < marks[low1])
    {
        low1 = 1;
        low2 = 0;
    }

    for (int i = 2; i < n; i++)
    {
        if (marks[i] < marks[low1])
        {
            low2 = low1;
            low1 = i;
        }
        else if (marks[i] < marks[low2])
        {
            low2 = i;
        }
    }

    double sum = 0;
    int count = 0;

    for (int i = 0; i < n; i++)
    {
        if (i != low1 && i != low2)
        {
            sum = sum + marks[i];
            count++;
        }
    }

    if (count == 0)
    {
        return 0.0;
    }

    return sum / count;
}

double computeWeightedTotal(double quizAvg, double assignmentAvg, double mid, double finalExam)
{
    return quizAvg * 0.10 + assignmentAvg * 0.10 + mid * 0.30 + finalExam * 0.50;
}

string getLetterGrade(double total)
{
    if (total >= 85.0) return "A";
    if (total >= 80.0) return "B+";
    if (total >= 70.0) return "B";
    if (total >= 65.0) return "C+";
    if (total >= 60.0) return "C";
    if (total >= 50.0) return "D";
    return "F";
}

double gradePoint(string grade)
{
    if (grade == "A") return 4.0;
    if (grade == "B+") return 3.5;
    if (grade == "B") return 3.0;
    if (grade == "C+") return 2.5;
    if (grade == "C") return 2.0;
    if (grade == "D") return 1.0;
    return 0.0;
}

bool enterMarks(string roll, string courseCode, string semester, double quizzes[], int quizCount, double assignments[], int assignmentCount, double mid, double finalExam)
{
    if (quizCount < 1 || quizCount > 5)
    {
        cout << "Quiz count must be from 1 to 5." << endl;
        return false;
    }

    for (int i = 0; i < quizCount; i++)
    {
        if (validRange(quizzes[i], 0.0, 100.0) == false)
        {
            cout << "Quiz marks must be 0 to 100." << endl;
            return false;
        }
    }

    if (assignmentCount < 1)
    {
        cout << "At least one assignment mark is required." << endl;
        return false;
    }

    for (int i = 0; i < assignmentCount; i++)
    {
        if (validRange(assignments[i], 0.0, 100.0) == false)
        {
            cout << "Assignment marks must be 0 to 100." << endl;
            return false;
        }
    }

    if (validRange(mid, 0.0, 40.0) == false)
    {
        cout << "Mid marks must be 0 to 40." << endl;
        return false;
    }

    if (validRange(finalExam, 0.0, 60.0) == false)
    {
        cout << "Final marks must be 0 to 60." << endl;
        return false;
    }

    double quizAvg = bestThreeOfFive(quizzes, quizCount);

    double assignmentAvg = 0;
    for (int i = 0; i < assignmentCount; i++)
    {
        assignmentAvg = assignmentAvg + assignments[i];
    }
    assignmentAvg = assignmentAvg / assignmentCount;

    double midOutOf100 = (mid / 40.0) * 100.0;
    double finalOutOf100 = (finalExam / 60.0) * 100.0;
    double total = computeWeightedTotal(quizAvg, assignmentAvg, midOutOf100, finalOutOf100);
    string grade = getLetterGrade(total);

    if (getAttendancePct(roll, courseCode) < 75.0)
    {
        grade = "F";
    }

    vector<vector<string> > rows = readTXT(GRADE_ENROLL_FILE);
    bool found = false;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        makeEnrollmentRowFull(rows[i]);

        if (rows[i][0] == roll && rows[i][1] == courseCode && rows[i][2] == semester && rows[i][3] == "enrolled")
        {
            rows[i][4] = doubleToString(quizAvg);
            rows[i][5] = doubleToString(assignmentAvg);
            rows[i][6] = doubleToString(mid);
            rows[i][7] = doubleToString(finalExam);
            rows[i][8] = doubleToString(total);
            rows[i][9] = grade;
            found = true;
        }
    }

    if (found == false)
    {
        cout << "Enrollment not found." << endl;
        return false;
    }

    writeTXT(GRADE_ENROLL_FILE, gradeEnrollmentHeader(), rows);
    cout << "Marks saved. Total: " << doubleToString(total) << " Grade: " << grade << endl;
    return true;
}

double computeGPA(string roll, string semester)
{
    vector<vector<string> > rows = readTXT(GRADE_ENROLL_FILE);
    double pointCreditSum = 0;
    double creditSum = 0;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        makeEnrollmentRowFull(rows[i]);

        if (rows[i][0] == roll && rows[i][2] == semester && rows[i][3] == "enrolled" && rows[i][9] != "")
        {
            int credits = getCourseCredits(rows[i][1]);
            pointCreditSum = pointCreditSum + gradePoint(rows[i][9]) * credits;
            creditSum = creditSum + credits;
        }
    }

    if (creditSum == 0)
    {
        return 0.0;
    }

    return pointCreditSum / creditSum;
}

Stats computeClassState(string courseCode, string semester)
{
    vector<vector<string> > rows = readTXT(GRADE_ENROLL_FILE);
    double totals[200];
    int count = 0;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        makeEnrollmentRowFull(rows[i]);

        if (rows[i][1] == courseCode && rows[i][2] == semester && rows[i][3] == "enrolled" && rows[i][8] != "")
        {
            if (count < 200)
            {
                totals[count] = stringToDouble(rows[i][8]);
                count++;
            }
        }
    }

    Stats st;
    st.highest = 0;
    st.lowest = 0;
    st.mean = 0;
    st.median = 0;

    if (count == 0)
    {
        return st;
    }

    st.highest = totals[0];
    st.lowest = totals[0];
    double sum = 0;

    for (int i = 0; i < count; i++)
    {
        if (totals[i] > st.highest) st.highest = totals[i];
        if (totals[i] < st.lowest) st.lowest = totals[i];
        sum = sum + totals[i];
    }

    st.mean = sum / count;

    for (int i = 0; i < count - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < count; j++)
        {
            if (totals[j] < totals[minIndex])
            {
                minIndex = j;
            }
        }
        if (minIndex != i)
        {
            double temp = totals[i];
            totals[i] = totals[minIndex];
            totals[minIndex] = temp;
        }
    }

    if (count % 2 == 1)
    {
        st.median = totals[count / 2];
    }
    else
    {
        st.median = (totals[count / 2 - 1] + totals[count / 2]) / 2.0;
    }

    return st;
}

bool applyAttendancePenalty(string roll, string courseCode, string semester)
{
    if (getAttendancePct(roll, courseCode) >= 75.0)
    {
        return false;
    }

    vector<vector<string> > rows = readTXT(GRADE_ENROLL_FILE);
    bool changed = false;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        makeEnrollmentRowFull(rows[i]);

        if (rows[i][0] == roll && rows[i][1] == courseCode && rows[i][2] == semester && rows[i][3] == "enrolled")
        {
            rows[i][9] = "F";
            changed = true;
        }
    }

    if (changed == true)
    {
        writeTXT(GRADE_ENROLL_FILE, gradeEnrollmentHeader(), rows);
    }

    return changed;
}
