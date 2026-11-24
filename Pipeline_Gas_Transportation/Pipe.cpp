#include "Pipe.h"
#include "utils.h"
#include <cmath>
#include <limits>

using namespace std;

int Pipe::maxId = 0;

bool Pipe::readFromConsole()
{
    if (maxId == numeric_limits<int>::max())
        return false;
    markName = readNonEmptyLine("Километровая отметка (название) трубы: ");
    lengthKm = readPositiveDouble("Длина трубы, км: ");
    diameterMm = readPositiveDouble("Диаметр трубы, мм: ");
    underRepair = readYesNo("Труба в ремонте? (1-да/0-нет): ");
    id = ++maxId;
    return true;
}

int Pipe::getId() const
{
    return id;
}

const string& Pipe::getName() const
{
    return markName;
}

bool Pipe::isUnderRepair() const
{
    return underRepair;
}

void Pipe::setUnderRepair(bool value)
{
    underRepair = value;
}

void Pipe::save(ofstream& out) const
{
    out << id << "\n" << markName << "\n"
        << lengthKm << " " << diameterMm << " " << underRepair << "\n";
}

bool Pipe::load(ifstream& in)
{
    return readObjectHeader(in, id, markName)
        && readValuesLine(in, lengthKm, diameterMm, underRepair)
        && isfinite(lengthKm) && lengthKm > 0
        && isfinite(diameterMm) && diameterMm > 0;
}

ostream& operator<<(ostream& out, const Pipe& pipe)
{
    out << "Труба id=" << pipe.id << " «" << pipe.markName << "»: длина " << pipe.lengthKm << "км, диаметр "
        << pipe.diameterMm << "мм, в ремонте: " << (pipe.underRepair ? "да" : "нет");
    return out;
}
