#include <iostream>
#include <string>
#ifdef _WIN32   // поддержка русского языка
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
    (void)station; // переменная пока не используется, добавил в промежуточном варианте, чтобы не было ошибок

    bool pipeExists = false;

    while (true) {
        printMenu();
        int choice = readIntInRange("Выберите действие: ", 0, 7);;
        
        switch (choice) {
        case 1:
            inputPipe(pipe);
            pipeExists = true;
            break;
        case 3:
            if (pipeExists) printPipe(pipe);
            else cout << "Труба еще не создана.\n";
            break;
        case 4:
            if (pipeExists) editPipeRepairFlag(pipe);
            else cout << "Труба еще не создана.\n";
            break;
        case 0:
            return 0;
        default:
            cout << "Этот пункт меню будет реализован позже.\n";
        }
    }
}
