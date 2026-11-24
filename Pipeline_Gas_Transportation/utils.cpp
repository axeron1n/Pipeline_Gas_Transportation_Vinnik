#include "utils.h"
#include <cstdlib>
#include <limits>

using namespace std;

static bool hasText(const string& line)
{
    return line.find_first_not_of(" \t\r\v\f") != string::npos;
}

string readLine(const string& prompt)
{
    cout << prompt;
    string line;
    if (!getline(cin, line)) {
        cout << "\nВвод завершен.\n";
        exit(0);
    }
    return line;
}

string readNonEmptyLine(const string& prompt)
{
    while (true) {
        string line = readLine(prompt);
        if (hasText(line))
            return line;
        cout << "Строка не должна быть пустой.\n";
    }
}

bool readObjectHeader(istream& in, int& id, string& name)
{
    if (!readValuesLine(in, id) || id <= 0 || !getline(in, name) || !hasText(name))
        return false;
    if (name.back() == '\r')
        name.pop_back();
    return true;
}

double readPositiveDouble(const string& prompt)
{
    // numeric_limits<double>::min() - наименьшее положительное число double
    return readNumber(prompt, numeric_limits<double>::min(), numeric_limits<double>::max());
}

bool readYesNo(const string& prompt)
{
    return readNumber(prompt, 0, 1) == 1;
}
