#include "Pipe.h"
#include "utils.h"
#include <algorithm>
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

void Pipe::load(ifstream& in)
{
    in >> id;
    in.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(in, markName);
    in >> lengthKm >> diameterMm >> underRepair;
    maxId = max(maxId, id); // новые трубы не получат id, уже занятый загруженной
}

ostream& operator<<(ostream& out, const Pipe& pipe)
{
    out << "Труба id=" << pipe.id << " «" << pipe.markName << "»: длина " << pipe.lengthKm << "км, диаметр "
        << pipe.diameterMm << "мм, в ремонте: " << (pipe.underRepair ? "да" : "нет");
    return out;
}
