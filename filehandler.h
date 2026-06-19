#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

vector<vector<string> > readTXT(string filename);
vector<string> readHeader(string filename);
void writeTXT(string filename, vector<string> header, vector<vector<string> > rows);
void appendTXT(string filename, vector<string> row);
vector<string> findRow(string filename, int colIndex, string value);
bool rowExists(string filename, int colIndex, string value);

bool isDigitChar(char ch);
bool isLetterChar(char ch);
bool hasDigit(string text);
bool stringContains(string text, string key);
bool stringStartsWith(string text, string key);
bool stringLess(string a, string b);
int stringToInt(string text);
double stringToDouble(string text);
string intToString(int num);
string doubleToString(double value);
bool isValidDateString(string date);

#endif
