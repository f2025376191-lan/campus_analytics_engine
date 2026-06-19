#include "filehandler.h"

vector<string> parseCSVLine(string line)
{
    vector<string> fields;
    string current = "";
    bool inQuotes = false;

    for (int i = 0; i < (int)line.length(); i++)
    {
        char ch = line[i];

        if (ch == '"')
        {
            inQuotes = !inQuotes;
        }
        else if (ch == ',' && inQuotes == false)
        {
            fields.push_back(current);
            current = "";
        }
        else
        {
            current = current + ch;
        }
    }

    fields.push_back(current);
    return fields;
}

string makeCSVField(string field)
{
    bool commaFound = false;

    for (int i = 0; i < (int)field.length(); i++)
    {
        if (field[i] == ',')
        {
            commaFound = true;
        }
    }

    if (commaFound == true)
    {
        return "\"" + field + "\"";
    }

    return field;
}

vector<string> readHeader(string filename)
{
    vector<string> header;
    ifstream fin(filename.c_str());
    string line;

    if (fin.is_open())
    {
        if (getline(fin, line))
        {
            header = parseCSVLine(line);
        }
        fin.close();
    }

    return header;
}

vector<vector<string> > readTXT(string filename)
{
    vector<vector<string> > rows;
    ifstream fin(filename.c_str());
    string line;
    bool firstLine = true;

    if (!fin.is_open())
    {
        return rows;
    }

    while (getline(fin, line))
    {
        if (firstLine == true)
        {
            firstLine = false;
        }
        else
        {
            if (line.length() > 0)
            {
                rows.push_back(parseCSVLine(line));
            }
        }
    }

    fin.close();
    return rows;
}

void writeTXT(string filename, vector<string> header, vector<vector<string> > rows)
{
    ofstream fout(filename.c_str());

    if (!fout.is_open())
    {
        cout << "File write error: " << filename << endl;
        return;
    }

    for (int i = 0; i < (int)header.size(); i++)
    {
        fout << makeCSVField(header[i]);
        if (i != (int)header.size() - 1)
        {
            fout << ",";
        }
    }
    fout << endl;

    for (int r = 0; r < (int)rows.size(); r++)
    {
        for (int c = 0; c < (int)rows[r].size(); c++)
        {
            fout << makeCSVField(rows[r][c]);
            if (c != (int)rows[r].size() - 1)
            {
                fout << ",";
            }
        }
        fout << endl;
    }

    fout.close();
}

void appendTXT(string filename, vector<string> row)
{
    ofstream fout(filename.c_str(), ios::app);

    if (!fout.is_open())
    {
        cout << "File append error: " << filename << endl;
        return;
    }

    for (int i = 0; i < (int)row.size(); i++)
    {
        fout << makeCSVField(row[i]);
        if (i != (int)row.size() - 1)
        {
            fout << ",";
        }
    }
    fout << endl;
    fout.close();
}

vector<string> findRow(string filename, int colIndex, string value)
{
    vector<vector<string> > rows = readTXT(filename);
    vector<string> emptyRow;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        if (colIndex >= 0 && colIndex < (int)rows[i].size())
        {
            if (rows[i][colIndex] == value)
            {
                return rows[i];
            }
        }
    }

    return emptyRow;
}

bool rowExists(string filename, int colIndex, string value)
{
    vector<vector<string> > rows = readTXT(filename);

    for (int i = 0; i < (int)rows.size(); i++)
    {
        if (colIndex >= 0 && colIndex < (int)rows[i].size())
        {
            if (rows[i][colIndex] == value)
            {
                return true;
            }
        }
    }

    return false;
}

bool isDigitChar(char ch)
{
    return ch >= '0' && ch <= '9';
}

bool isLetterChar(char ch)
{
    if (ch >= 'A' && ch <= 'Z')
    {
        return true;
    }
    if (ch >= 'a' && ch <= 'z')
    {
        return true;
    }
    return false;
}

bool hasDigit(string text)
{
    for (int i = 0; i < (int)text.length(); i++)
    {
        if (isDigitChar(text[i]) == true)
        {
            return true;
        }
    }
    return false;
}

bool stringContains(string text, string key)
{
    if (key.length() == 0)
    {
        return true;
    }
    if (key.length() > text.length())
    {
        return false;
    }

    for (int i = 0; i <= (int)text.length() - (int)key.length(); i++)
    {
        bool same = true;
        for (int j = 0; j < (int)key.length(); j++)
        {
            if (text[i + j] != key[j])
            {
                same = false;
            }
        }
        if (same == true)
        {
            return true;
        }
    }

    return false;
}

bool stringStartsWith(string text, string key)
{
    if (key.length() > text.length())
    {
        return false;
    }

    string startPart = text.substr(0, key.length());
    if (startPart == key)
    {
        return true;
    }
    return false;
}

bool stringLess(string a, string b)
{
    int n = (int)a.length();
    if ((int)b.length() < n)
    {
        n = (int)b.length();
    }

    for (int i = 0; i < n; i++)
    {
        if (a[i] < b[i])
        {
            return true;
        }
        if (a[i] > b[i])
        {
            return false;
        }
    }

    if (a.length() < b.length())
    {
        return true;
    }
    return false;
}

int stringToInt(string text)
{
    int value = 0;
    int sign = 1;
    int start = 0;

    if (text.length() > 0 && text[0] == '-')
    {
        sign = -1;
        start = 1;
    }

    for (int i = start; i < (int)text.length(); i++)
    {
        if (isDigitChar(text[i]) == true)
        {
            value = value * 10 + (text[i] - '0');
        }
    }

    return value * sign;
}

double stringToDouble(string text)
{
    double value = 0;
    double decimalPlace = 0.1;
    bool afterPoint = false;
    int sign = 1;
    int start = 0;

    if (text.length() > 0 && text[0] == '-')
    {
        sign = -1;
        start = 1;
    }

    for (int i = start; i < (int)text.length(); i++)
    {
        if (text[i] == '.')
        {
            afterPoint = true;
        }
        else if (isDigitChar(text[i]) == true)
        {
            if (afterPoint == false)
            {
                value = value * 10 + (text[i] - '0');
            }
            else
            {
                value = value + (text[i] - '0') * decimalPlace;
                decimalPlace = decimalPlace / 10.0;
            }
        }
    }

    return value * sign;
}

string intToString(int num)
{
    if (num == 0)
    {
        return "0";
    }

    string result = "";
    int n = num;

    if (n < 0)
    {
        result = "-";
        n = -n;
    }

    string reverseDigits = "";
    while (n > 0)
    {
        int digit = n % 10;
        reverseDigits = reverseDigits + char('0' + digit);
        n = n / 10;
    }

    for (int i = (int)reverseDigits.length() - 1; i >= 0; i--)
    {
        result = result + reverseDigits[i];
    }

    return result;
}

string doubleToString(double value)
{
    bool negative = false;
    if (value < 0)
    {
        negative = true;
        value = -value;
    }

    int scaled = (int)(value * 100.0 + 0.5);
    int whole = scaled / 100;
    int part = scaled % 100;

    string result = intToString(whole) + ".";
    if (part < 10)
    {
        result = result + "0";
    }
    result = result + intToString(part);

    if (negative == true)
    {
        result = "-" + result;
    }

    return result;
}

bool isValidDateString(string date)
{
    if (date.length() != 10)
    {
        return false;
    }

    if (date[2] != '-' || date[5] != '-')
    {
        return false;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i != 2 && i != 5)
        {
            if (isDigitChar(date[i]) == false)
            {
                return false;
            }
        }
    }

    int day = stringToInt(date.substr(0, 2));
    int month = stringToInt(date.substr(3, 2));
    int year = stringToInt(date.substr(6, 4));

    if (year < 1900 || month < 1 || month > 12 || day < 1)
    {
        return false;
    }

    int monthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
    {
        monthDays[1] = 29;
    }

    if (day > monthDays[month - 1])
    {
        return false;
    }

    return true;
}
