#include "CompressorStation.h"
#include "utils.h"
#include <algorithm>
#include <limits>

using namespace std;

int CompressorStation::maxId = 0;

bool CompressorStation::readFromConsole()
{
    if (maxId == numeric_limits<int>::max())
        return false;
    name = readNonEmptyLine("Название КС: ");
    shopCount = readNumber("Количество цехов: ", 1, numeric_limits<int>::max());
    shopsInWork = readNumber("Количество цехов в работе (0.." + to_string(shopCount) + "): ", 0, shopCount);
    stationClass = readNumber("Класс станции: ", 0, numeric_limits<int>::max());
    id = ++maxId;
    return true;
}

int CompressorStation::getId() const
{
    return id;
}

int CompressorStation::getFreeShops() const
{
    return shopCount - shopsInWork;
}

int CompressorStation::getWorkingShops() const
{
    return shopsInWork;
}

bool CompressorStation::startShops(int count)
{
    if (count <= 0 || count > getFreeShops())
        return false;
    shopsInWork += count;
    return true;
}

bool CompressorStation::stopShops(int count)
{
    if (count <= 0 || count > getWorkingShops())
        return false;
    shopsInWork -= count;
    return true;
}

void CompressorStation::save(ofstream& out) const
{
    out << id << "\n" << name << "\n"
        << shopCount << " " << shopsInWork << " " << stationClass << "\n";
}

void CompressorStation::load(ifstream& in)
{
    in >> id;
    in.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(in, name);
    in >> shopCount >> shopsInWork >> stationClass;
    maxId = max(maxId, id);
}

ostream& operator<<(ostream& out, const CompressorStation& station)
{
    out << "КС id=" << station.id << " «" << station.name << "»: цехов " << station.shopCount
        << ", в работе " << station.shopsInWork << ", класс " << station.stationClass;
    return out;
}
