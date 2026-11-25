#pragma once
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

std::string readLine(const std::string& prompt);
std::string readNonEmptyLine(const std::string& prompt);
double readPositiveDouble(const std::string& prompt);
bool readYesNo(const std::string& prompt);
std::set<int> readIdsFrom(const std::set<int>& allowedIds);
bool readObjectHeader(std::istream& in, int& id, std::string& name);

template <typename T>
bool tryParse(const std::string& text, T& value)
{
    std::istringstream input(text);
    char extra;
    return input >> value && !(input >> extra);
}

template <typename... Values>
bool readValuesLine(std::istream& in, Values&... values)
{
    std::string line;
    if (!std::getline(in, line))
        return false;
    std::istringstream input(line);
    std::string token;
    return ((input >> token && tryParse(token, values)) && ...) && !(input >> token);
}

// Читает число типа T из диапазона [minValue, maxValue], пока ввод не станет корректным
template <typename T>
T readNumber(const std::string& prompt, T minValue, T maxValue)
{
    while (true) {
        T value{};
        if (tryParse(readLine(prompt), value) && value >= minValue && value <= maxValue)
            return value;
        std::cout << "Некорректный ввод, повторите.\n";
    }
}

// Возвращает id объектов, для которых matches(объект) == true
template <typename T, typename Predicate>
std::set<int> findIds(const std::unordered_map<int, T>& objects, Predicate matches)
{
    std::set<int> ids;
    for (const auto& [id, object] : objects)
        if (matches(object))
            ids.insert(id);
    return ids;
}

// Печатает только объекты с id из ids
template <typename T>
void printObjects(const std::unordered_map<int, T>& objects, const std::set<int>& ids)
{
    for (int id : ids)
        std::cout << " " << objects.at(id) << "\n";
}

template <typename T>
void printObjects(const std::unordered_map<int, T>& objects)
{
    printObjects(objects, findIds(objects, [](const T&) { return true; }));
}