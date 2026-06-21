#include "student_ops.h"

string STUDENT_FILE = "students.txt";

bool validateRoll(string roll)
{
    if (roll.length() != 11)
    {
        return false;
    }

    if (roll.substr(0, 5) != "BSAI-")
    {
        return false;
    }

    if (roll[7] != '-')
    {
        return false;
    }

    if (isDigitChar(roll[5]) == false || isDigitChar(roll[6]) == false)
    {
        return false;
    }

    if (isDigitChar(roll[8]) == false || isDigitChar(roll[9]) == false || isDigitChar(roll[10]) == false)
    {
        return false;
    }

    return true;
}

bool addStudent(string roll, string name, string dept, double cgpa)
{
    if (validateRoll(roll) == false)
    {
        cout << "Invalid roll format. Use BSAI-YY-XXX." << endl;
        return false;
    }

    if (rowExists(STUDENT_FILE, 0, roll) == true)
    {
        cout << "Duplicate roll number is not allowed." << endl;
        return false;
    }

    if (name.length() == 0 || hasDigit(name) == true)
    {
        cout << "Invalid name. Name must not contain digits." << endl;
        return false;
    }

    if (cgpa < 0.0 || cgpa > 4.0)
    {
        cout << "Invalid CGPA. Range must be 0.0 to 4.0." << endl;
        return false;
    }

    vector<string> row;
    row.push_back(roll);
    row.push_back(name);
    row.push_back(dept);
    row.push_back(doubleToString(cgpa));
    row.push_back("active");

    appendTXT(STUDENT_FILE, row);
    cout << "Student added successfully." << endl;
    return true;
}

vector<string> searchByRoll(string roll)
{
    return findRow(STUDENT_FILE, 0, roll);
}

vector<vector<string> > searchByName(string namePart)
{
    vector<vector<string> > rows = readTXT(STUDENT_FILE);
    vector<vector<string> > result;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        if ((int)rows[i].size() >= 5)
        {
            if (stringContains(rows[i][1], namePart) == true)
            {
                result.push_back(rows[i]);
            }
        }
    }

    return result;
}

bool updateStudent(string roll, int fieldIndex, string newValue)
{
    if (fieldIndex == 0)
    {
        cout << "Roll number cannot be updated." << endl;
        return false;
    }

    if (fieldIndex < 1 || fieldIndex > 4)
    {
        cout << "Invalid field index." << endl;
        return false;
    }

    if (fieldIndex == 1 && hasDigit(newValue) == true)
    {
        cout << "Name must not contain digits." << endl;
        return false;
    }

    if (fieldIndex == 3)
    {
        double cgpa = stringToDouble(newValue);
        if (cgpa < 0.0 || cgpa > 4.0)
        {
            cout << "CGPA must be between 0.0 and 4.0." << endl;
            return false;
        }
    }

    if (fieldIndex == 4)
    {
        if (newValue != "active" && newValue != "inactive")
        {
            cout << "Status must be active or inactive." << endl;
            return false;
        }
    }

    vector<vector<string> > rows = readTXT(STUDENT_FILE);
    bool found = false;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        if ((int)rows[i].size() >= 5 && rows[i][0] == roll)
        {
            rows[i][fieldIndex] = newValue;
            found = true;
        }
    }

    if (found == false)
    {
        cout << "Student not found." << endl;
        return false;
    }

    vector<string> header;
    header.push_back("roll");
    header.push_back("name");
    header.push_back("dept");
    header.push_back("cgpa");
    header.push_back("status");

    writeTXT(STUDENT_FILE, header, rows);
    cout << "Student updated successfully." << endl;
    return true;
}

bool softDelete(string roll)
{
    return updateStudent(roll, 4, "inactive");
}

vector<vector<string> > listActiveStudents()
{
    vector<vector<string> > rows = readTXT(STUDENT_FILE);
    vector<vector<string> > activeRows;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        if ((int)rows[i].size() >= 5 && rows[i][4] == "active")
        {
            activeRows.push_back(rows[i]);
        }
    }

    for (int i = 0; i < (int)activeRows.size() - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < (int)activeRows.size(); j++)
        {
            if (stringLess(activeRows[j][0], activeRows[minIndex][0]) == true)
            {
                minIndex = j;
            }
        }
        if (minIndex != i)
        {
            vector<string> temp = activeRows[i];
            activeRows[i] = activeRows[minIndex];
            activeRows[minIndex] = temp;
        }
    }

    return activeRows;
}

bool isActiveStudent(string roll)
{
    vector<string> row = searchByRoll(roll);

    if ((int)row.size() >= 5 && row[4] == "active")
    {
        return true;
    }

    return false;
}

void printStudentRow(vector<string> row)
{
    if ((int)row.size() < 5)
    {
        return;
    }

    cout << left << setw(14) << row[0]
         << setw(24) << row[1]
         << setw(12) << row[2]
         << setw(8) << row[3]
         << setw(10) << row[4] << endl;
}

void printStudentTable(vector<vector<string> > rows)
{
    cout << left << setw(14) << "Roll"
         << setw(24) << "Name"
         << setw(12) << "Dept"
         << setw(8) << "CGPA"
         << setw(10) << "Status" << endl;
    cout << "--------------------------------------------------------------------" << endl;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        printStudentRow(rows[i]);
    }
}

void searchAsYouType()
{
    string prefix = "";
    char ch;

    cout << "Enter one character at a time. Enter # to stop." << endl;

    while (true)
    {
        cout << "Current prefix [" << prefix << "] next char: ";
        cin >> ch;

        if (ch == '#')
        {
            break;
        }

        prefix = prefix + ch;

        vector<vector<string> > rows = listActiveStudents();
        vector<vector<string> > matched;

        for (int i = 0; i < (int)rows.size(); i++)
        {
            bool nameMatch = stringStartsWith(rows[i][1], prefix);
            bool rollMatch = stringStartsWith(rows[i][0], prefix);

            if (nameMatch == true || rollMatch == true)
            {
                matched.push_back(rows[i]);
            }
        }

        printStudentTable(matched);
    }
}
