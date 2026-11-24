#pragma once
#include <fstream>
#include <iostream>
#include <string>

class Pipe {
public:
    bool readFromConsole();

    int getId() const;
    const std::string& getName() const;

    bool isUnderRepair() const;
    void setUnderRepair(bool value);

    void save(std::ofstream& out) const;
    bool load(std::ifstream& in);

    friend std::ostream& operator<<(std::ostream& out, const Pipe& pipe);

private:
    friend class GasNetwork;
    static int maxId;

    int id = 0;
    std::string markName;
    double lengthKm = 0;
    double diameterMm = 0;
    bool underRepair = false;
};
