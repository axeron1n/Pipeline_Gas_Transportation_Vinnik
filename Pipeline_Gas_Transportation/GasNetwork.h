#pragma once
#include "CompressorStation.h"
#include "Pipe.h"
#include <fstream>
#include <string>
#include <unordered_map>

class GasNetwork {
public:
    const Pipe& addPipe(Pipe pipe);
    const CompressorStation& addStation(CompressorStation station);

    Pipe& getPipe(int id);
    CompressorStation& getStation(int id);

    bool removePipe(int id);
    bool removeStation(int id);

    const std::unordered_map<int, Pipe>& getPipes() const;
    const std::unordered_map<int, CompressorStation>& getStations() const;

    bool save(const std::string& fileName) const;
    bool load(const std::string& fileName);
private:
    std::unordered_map<int, Pipe> pipes;
    std::unordered_map<int, CompressorStation> stations;

    void savePipes(std::ofstream& out) const;
    void saveStations(std::ofstream& out) const;
};
