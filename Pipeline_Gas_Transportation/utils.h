#pragma once
#include <iostream>
#include <sstream>
#include <string>

std::string readLine(const std::string& prompt);
std::string readNonEmptyLine(const std::string& prompt);
double readPositiveDouble(const std::string& prompt);
bool readYesNo(const std::string& prompt);

// Читает число типа T из диапазона [minValue, maxValue], пока ввод не станет корректным
template <typename T>
T readNumber(const std::string& prompt, T minValue, T maxValue)
{
    while (true) {
        std::istringstream input(readLine(prompt));
        T value;
        char extra;
        if (input >> value && !(input >> extra) && value >= minValue && value <= maxValue)
            return value;
        std::cout << "Некорректный ввод, повторите.\n";
    }
}
