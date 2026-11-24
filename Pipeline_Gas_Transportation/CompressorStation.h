#pragma once
#include <fstream>
#include <iostream>
#include <string>

class CompressorStation {
public:
    void readFromConsole();

    int getFreeShops() const;
    int getWorkingShops() const;
    bool startShops(int count);
    bool stopShops(int count);

    void save(std::ofstream& out) const;
    void load(std::ifstream& in);

    friend std::ostream& operator<<(std::ostream& out, const CompressorStation& station);
    
private:
    std::string name;
    int shopCount = 0;
    int shopsInWork = 0;
    int stationClass = 0;
};