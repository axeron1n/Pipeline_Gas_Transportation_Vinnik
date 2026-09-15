#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#ifdef _WIN32   // поддержка русского языка
#define NOMINMAX
#include <Windows.h>
#endif

using namespace std;

struct Pipe {
    string markName;
    double lengthKm = 0;
    double diameterMm = 0;
    bool underRepair = false;
};

struct CompressorStation {
    string name;
    int shopCount = 0;
    int shopInWork = 0;
    int stationClass = 0;
};

const string DATA_FILE = "pipeline_data.txt";

string readLine(const string& prompt) {
    cout << prompt;
    string line;
    getline(cin, line); // читаем всю строку до Enter, пробелы не мешают
    return line;
}

double readPositiveDouble(const string& prompt) {
    while (true) {
        string line = readLine(prompt);
        try {
            size_t pos; // сюда stod запишет докуда дошел разбор строки
            double value = stod(line, &pos); // пытается прочитать число с плавающей точкой из строки
            if (pos == line.size() && value > 0) return value;  // разобрано целиком и число положительное - подходит
        }
        catch (...) {}  // ввод не число - игнорируем и переспрашиваем
        cout << "Введите положительное число.\n";
    }
}

int readNonNegative(const string& prompt) {
    while (true) {
        string line = readLine(prompt);
        try {
            size_t pos;
            int value = stoi(line, &pos); // тоже самое что stod, но для int
            if (pos == line.size() && value >= 0) return value;
        }
        catch (...) {}
        cout << "Введите неотрицательное число.\n";
    }
}

int readIntInRange(const string& prompt, int minVal, int maxVal) {
    while (true) {
        string line = readLine(prompt);
        try {
            size_t pos;
            int value = stoi(line, &pos);
            if (pos == line.size() && value >= minVal && value <= maxVal) return value;
        }
        catch (...) {}
        cout << "Введите число от " << minVal << " до " << maxVal << ".\n";
    }
}

bool readYesNo(const string& prompt) {
    while (true) {
        string line = readLine(prompt);
        if (line == "1" || line == "y" || line == "Y") return true;
        if (line == "0" || line == "n" || line == "N") return false;
        cout << "Введите 1 (да) или 0 (нет).\n";
    }
}

void inputPipe(Pipe& pipe) {
    pipe.markName = readLine("Киллометровая отметка (название) трубы: ");
    pipe.lengthKm = readPositiveDouble("Длина трубы, км: ");
    pipe.diameterMm = readPositiveDouble("Диаметр трубы, мм: ");
    pipe.underRepair = readYesNo("Труба в ремонте? (1-да/0-нет): ");
}

void printPipe(const Pipe& pipe) {
    cout << "--- Труба ---\n"
        << "Отметка: " << pipe.markName << "\n"
        << "Длина, км: " << pipe.lengthKm << "\n"
        << "Диаметр, мм: " << pipe.diameterMm << "\n"
        << "В ремонте: " << (pipe.underRepair ? "да" : "нет") << "\n";
}

void editPipeRepairFlag(Pipe& pipe) {
    pipe.underRepair = readYesNo("Установить признак 'в ремонте'? (1-да/0-нет): ");
    cout << "Признак обновлен.\n";
}

void inputStation(CompressorStation& station) {
    station.name = readLine("Название КС: ");
    station.shopCount = readNonNegative("Количество цехов: ");
    station.shopInWork = readIntInRange("Количество цехов в работе (0.." + to_string(station.shopCount) + "): ",
        0, station.shopCount);
    station.stationClass = readNonNegative("Класс станции: ");
}

void printStation(const CompressorStation& station) {
    cout << "--- КС ---\n"
        << "Название: " << station.name << "\n"
        << "Цехов всего: " << station.shopCount << "\n"
        << "Цехов в работе: " << station.shopInWork << "\n"
        << "Класс станции: " << station.stationClass << "\n";
}

void editStationShops(CompressorStation& station) {
    cout << "1. Запустить цех\n2. Остановить цех\n0. Назад\n";
    int choice = readIntInRange("Выберите действие: ", 0, 2);
    if (choice == 1) {
        if (station.shopInWork < station.shopCount) {
            ++station.shopInWork;
            cout << "Цех запущен. В работе: " << station.shopInWork << "/" << station.shopCount << "\n";
        }
        else {
            cout << "Все цехи уже в работе.\n";
        }
    }
    else if (choice == 2) {
        if (station.shopInWork > 0) {
            --station.shopInWork;
            cout << "Цех остановлен. В работе: " << station.shopInWork << "/" << station.shopCount << "\n";
        }
        else {
            cout << "Нет работающих цехов.\n";
        }
    }
}

bool saveData(bool pipeExists, const Pipe& pipe, bool stationExists, const CompressorStation& station) {
    ofstream out(DATA_FILE);
    if (!out) return false;

    if (pipeExists) {
        out << "PIPE\n" << pipe.markName << "\n"
            << pipe.lengthKm << " " << pipe.diameterMm << " " << pipe.underRepair << "\n";
    }
    else {
        out << "NOPIPE\n";
    }

    if (stationExists) {
        out << "STATION\n" << station.name << "\n"
            << station.shopCount << " " << station.shopInWork << " " << station.stationClass << "\n";
    }
    else {
        out << "NOSTATION\n";
    }
    return true;
}

bool loadData(bool& pipeExists, Pipe& pipe, bool& stationExists, CompressorStation& station) {
    ifstream in(DATA_FILE);
    if (!in) return false;

    string tag;
    getline(in, tag);
    if (tag == "PIPE") {
        getline(in, pipe.markName);
        in >> pipe.lengthKm >> pipe.diameterMm >> pipe.underRepair;
        in.ignore(numeric_limits<streamsize>::max(), '\n');
        pipeExists = true;
    }
    else {
        pipeExists = false;
    }

    getline(in, tag);
    if (tag == "STATION") {
        getline(in, station.name);
        in >> station.shopCount >> station.shopInWork >> station.stationClass;
        in.ignore(numeric_limits<streamsize>::max(), '\n');
        stationExists = true;
    }
    else {
        stationExists = false;
    }
    return true;
}

void printMenu() {
    cout << "\n1. Добавить трубу\n"
        << "2. Добавить КС\n"
        << "3. Просмотр всех объектов\n"
        << "4. Редактировать трубу\n"
        << "5. Редактировать КС\n"
        << "6. Сохранить\n"
        << "7. Загрузить\n"
        << "0. Выход\n";
}

int main() {
#ifdef _WIN32   // поддержка русского языка
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    Pipe pipe;
    CompressorStation station;
    bool pipeExists = false;
    bool stationExists = false;

    while (true) {
        printMenu();
        int choice = readIntInRange("Выберите действие: ", 0, 7);;
        
        switch (choice) {
        case 1:
            if (pipeExists && !readYesNo("Труба уже создана. Пересоздать? (1-да/0-нет): ")) break;
            inputPipe(pipe);
            pipeExists = true;
            break;
        case 2:
            if (stationExists && !readYesNo("КС уже создана. Пересоздать?: (1-да/0-нет) ")) break;
            inputStation(station);
            stationExists = true;
            break;
        case 3:
            if (pipeExists) printPipe(pipe);
            else cout << "Труба еще не создана.\n";
            if (stationExists) printStation(station);
            else cout << "КС еще не создана.\n";
            break;
        case 4:
            if (pipeExists) editPipeRepairFlag(pipe);
            else cout << "Труба еще не создана.\n";
            break;
        case 5:
            if (stationExists) editStationShops(station);
            else cout << "КС еще не создана.\n";
            break;
        case 6:
            cout << (saveData(pipeExists, pipe, stationExists, station)
                ? "Данные сохранены.\n" : "Ошибка сохранения.\n");
            break;
        case 7:
            cout << (loadData(pipeExists, pipe, stationExists, station)
                ? "Данные загружены.\n" : "Файл не найден.\n");
            break;
        case 0:
            return 0;
        }
    }
}
