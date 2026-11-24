#pragma once
#include <fstream>
#include <iostream>
#include <string>

class CompressorStation {
public:
    bool readFromConsole();

    int getId() const;

    int getFreeShops() const;
    int getWorkingShops() const;
    bool startShops(int count);
    bool stopShops(int count);

    void save(std::ofstream& out) const;
    void load(std::ifstream& in);

    friend std::ostream& operator<<(std::ostream& out, const CompressorStation& station);
    
private:
    static int maxId; // наибольший выданный id: новый объект получает maxId + 1

    int id = 0;
    std::string name;
    int shopCount = 0;
    int shopsInWork = 0;
    int stationClass = 0;
};