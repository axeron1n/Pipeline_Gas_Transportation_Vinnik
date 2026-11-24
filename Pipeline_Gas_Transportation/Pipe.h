#pragma once
#include <fstream>
#include <iostream>
#include <string>

class Pipe {
public:
    void readFromConsole();

    bool isUnderRepair() const;
    void setUnderRepair(bool value);

    void save(std::ofstream& out) const;
    void load(std::ifstream& in);

    friend std::ostream& operator<<(std::ostream& out, const Pipe& pipe);

private:
    std::string markName;
    double lengthKm = 0;
    double diameterMm = 0;
    bool underRepair = false;
};
