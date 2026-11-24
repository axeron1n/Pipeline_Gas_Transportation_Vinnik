#include "Pipe.h"
#include "utils.h"
#include <limits>

using namespace std;

void Pipe::readFromConsole()
{
    markName = readNonEmptyLine("Километровая отметка (название) трубы: ");
    lengthKm = readPositiveDouble("Длина трубы, км: ");
    diameterMm = readPositiveDouble("Диаметр трубы, мм: ");
    underRepair = readYesNo("Труба в ремонте? (1-да/0-нет): ");
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
    out << markName << "\n"
        << lengthKm << " " << diameterMm << " " << underRepair << "\n";
}

void Pipe::load(ifstream& in)
{
    in.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(in, markName);
    in >> lengthKm >> diameterMm >> underRepair;
}

ostream& operator<<(ostream& out, const Pipe& pipe)
{
    out << "Труба «" << pipe.markName << "»: длина " << pipe.lengthKm << "км, диаметр "
        << pipe.diameterMm << "мм, в ремонте: " << (pipe.underRepair ? "да" : "нет");
    return out;
}
