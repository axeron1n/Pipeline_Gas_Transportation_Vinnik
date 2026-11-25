#include "GasNetwork.h"
#include "utils.h"
#include <algorithm>
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

void GasNetwork::setPipesUnderRepair(const set<int>& ids, bool underRepair)
{
    for (int id : ids)
        pipes.at(id).setUnderRepair(underRepair);
}

void GasNetwork::removePipes(const set<int>& ids)
{
    for (int id : ids)
        pipes.erase(id);
}

const unordered_map<int, Pipe>& GasNetwork::getPipes() const
{
    return pipes;
}

const unordered_map<int, CompressorStation>& GasNetwork::getStations() const
{
    return stations;
}

set<int> GasNetwork::findPipesByName(const string& part) const
{
    return findIds(pipes, [&part](const Pipe& pipe) {
        return pipe.getName().find(part) != string::npos;
    });
}

set<int> GasNetwork::findPipesByRepair(bool underRepair) const
{
    return findIds(pipes, [underRepair](const Pipe& pipe) {
        return pipe.isUnderRepair() == underRepair;
    });
}

set<int> GasNetwork::findStationsByName(const string& part) const
{
    return findIds(stations, [&part](const CompressorStation& station) {
        return station.getName().find(part) != string::npos;
    });
}

set<int> GasNetwork::findStationsByUnusedPercent(double minPercent, double maxPercent) const
{
    return findIds(stations, [minPercent, maxPercent](const CompressorStation& station) {
        double percent = station.getUnusedPercent();
        return percent >= minPercent && percent <= maxPercent;
    });
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

template <typename T>
static bool loadObjects(ifstream& in, unordered_map<int, T>& result, int& maxId)
{
    int count = 0;
    if (!readValuesLine(in, count) || count < 0)
        return false;
    for (int i = 0; i < count; ++i) {
        T object;
        if (!object.load(in))
            return false;
        const int id = object.getId();
        if (!result.emplace(id, move(object)).second)
            return false;
        maxId = max(maxId, id);
    }
    return true;
}

bool GasNetwork::load(const string& filename)
{
    ifstream in(filename);
    if (!in)
        return false;
    unordered_map<int, Pipe> loadedPipes;
    unordered_map<int, CompressorStation> loadedStations;
    int maxPipeId = Pipe::maxId;
    int maxStationId = CompressorStation::maxId;
    if (!loadObjects(in, loadedPipes, maxPipeId) || !loadObjects(in, loadedStations, maxStationId))
        return false;
    in >> ws;
    if (in.bad() || !in.eof())
        return false;
    pipes.swap(loadedPipes);
    stations.swap(loadedStations);
    Pipe::maxId = maxPipeId;
    CompressorStation::maxId = maxStationId;
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
