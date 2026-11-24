#include "GasNetwork.h"
#include <limits>
#include <utility>

using namespace std;

const Pipe& GasNetwork::addPipe(Pipe pipe)
{
    int id = pipe.getId();
    return pipes.emplace(id, move(pipe)).first->second;
}

const CompressorStation& GasNetwork::addStation(CompressorStation station)
{
    int id = station.getId();
    return stations.emplace(id, move(station)).first->second;
}

Pipe& GasNetwork::getPipe(int id)
{
    return pipes.at(id);
}

CompressorStation& GasNetwork::getStation(int id)
{
    return stations.at(id);
}

bool GasNetwork::removePipe(int id)
{
    return pipes.erase(id) > 0;
}

bool GasNetwork::removeStation(int id)
{
    return stations.erase(id) > 0;
}

const unordered_map<int, Pipe>& GasNetwork::getPipes() const
{
    return pipes;
}

const unordered_map<int, CompressorStation>& GasNetwork::getStations() const
{
    return stations;
}

bool GasNetwork::save(const string& filename) const
{
    ofstream out(filename);
    if (!out)
        return false;
    out.precision(numeric_limits<double>::max_digits10);
    savePipes(out);
    saveStations(out);
    return out.good();
}

bool GasNetwork::load(const string& filename)
{
    ifstream in(filename);
    if (!in)
        return false;
    // Читаем во временные контейнеры, чтобы при ошибке в файле не потерять текущие данные
    unordered_map<int, Pipe> loadedPipes;
    unordered_map<int, CompressorStation> loadedStations;
    if (!loadPipes(in, loadedPipes) || !loadStations(in, loadedStations))
        return false;
    pipes.swap(loadedPipes);
    stations.swap(loadedStations);
    return true;
}

void GasNetwork::savePipes(ofstream& out) const
{
    out << pipes.size() << "\n";
    for (const auto& [id, pipe] : pipes)
        pipe.save(out);
}

void GasNetwork::saveStations(ofstream& out) const
{
    out << stations.size() << "\n";
    for (const auto& [id, station] : stations)
        station.save(out);
}

bool GasNetwork::loadPipes(ifstream& in, unordered_map<int, Pipe>& result)
{
    size_t count = 0;
    in >> count;
    for (size_t i = 0; i < count && in; ++i) {
        Pipe pipe;
        pipe.load(in);
        int id = pipe.getId();
        result.emplace(id, move(pipe));
    }
    return !in.fail();
}

bool GasNetwork::loadStations(ifstream& in, unordered_map<int, CompressorStation>& result)
{
    size_t count = 0;
    in >> count;
    for (size_t i = 0; i < count && in; ++i) {
        CompressorStation station;
        station.load(in);
        int id = station.getId();
        result.emplace(id, move(station));
    }
    return !in.fail();
}
