#include "utils.h"
#include <limits>

using namespace std;

string readLine(const string& prompt)
{
    cout << prompt;
    string line;
    getline(cin, line);
    return line;
}

string readNonEmptyLine(const string& prompt)
{
    while (true) {
        string line = readLine(prompt);
        if (line.find_first_not_of(" \t") != string::npos)
            return line;
        cout << "Строка не должна быть пустой.\n";
    }
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
