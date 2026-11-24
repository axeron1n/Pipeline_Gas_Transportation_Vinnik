#include "CompressorStation.h"
#include "Pipe.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <fstream>
#ifdef _WIN32   // поддержка русского языка
#define NOMINMAX
#include <Windows.h>
#endif

using namespace std;

const string dataFile = "data_file.txt";

void editPipeRepairFlag(Pipe& pipe)
{
    pipe.setUnderRepair(readYesNo("Установить признак 'в ремонте'? (1-да/0-нет): "));
    cout << "Признак обновлен.\n";
}

int readShopChangeCount(int available, bool start)
{
    if (available == 0) {
        cout << (start ? "Все цехи уже в работе.\n" : "Нет работающих цехов.\n");
        return 0;
    }
    string prompt = start ? "Количество цехов для запуска" : "Количество цехов для остановки";
    return readNumber(prompt + " (1.." + to_string(available) + "): ", 1, available);
}

void changeStationShops(CompressorStation& station, bool start, int count)
{
    bool changed = start ? station.startShops(count) : station.stopShops(count);
    if (changed)
        cout << (start ? "Запущено цехов: " : "Остановлено цехов: ") << count << ". " << station << "\n";
    else
        cout << station << ". " << (start ? "Недостаточно свободныъ цехов." : "Недостаточно работающих цехов.") << "\n";
}

void editStationShops(CompressorStation& station)
{
    cout << "1. Запустить цехи\n2. Остановить цехи\n0. Назад\n";
    int choice = readNumber("Выберите действие: ", 0, 2);
    if (choice == 0)
        return;
    bool start = choice == 1;
    int available = start ? station.getFreeShops() : station.getWorkingShops();
    int count = readShopChangeCount(available, start);
    if (count > 0)
        changeStationShops(station, start, count);
}

bool savePipeData(ofstream& out, bool pipeExists, const Pipe& pipe)
{
    if (pipeExists) {
        out << "PIPE\n";
        pipe.save(out);
    }
    else {
        out << "NOPIPE\n";
    }
    return out.good();
}

bool loadPipeData(ifstream& in, bool& pipeExists, Pipe& pipe)
{
    string tag;
    in >> tag;
    pipeExists = (tag == "PIPE");
    if (pipeExists)
        pipe.load(in);
    return in.good();
}

bool saveStationData(ofstream& out, bool stationExists, const CompressorStation& station)
{
    if (stationExists) {
        out << "STATION\n";
        station.save(out);
    }
    else {
        out << "NOSTATION\n";
    }
    return out.good();
}

bool loadStationData(ifstream& in, bool& stationExists, CompressorStation& station)
{
    string tag;
    in >> tag;
    stationExists = (tag == "STATION");
    if (stationExists)
        station.load(in);
    return in.good();
}

void printMenu()
{
    cout << "\n1. Добавить трубу\n"
        << "2. Добавить КС\n"
        << "3. Просмотр всех объектов\n"
        << "4. Редактировать трубу\n"
        << "5. Редактировать КС\n"
        << "6. Сохранить\n"
        << "7. Загрузить\n"
        << "0. Выход\n";
}

int main()
{
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
        int choice = readNumber("Выберите действие: ", 0, 7);;
        
        switch (choice) {
        case 1:
            if (pipeExists && !readYesNo("Труба уже создана. Пересоздать? (1-да/0-нет): ")) break;
            pipe.readFromConsole();
            pipeExists = true;
            break;
        case 2:
            if (stationExists && !readYesNo("КС уже создана. Пересоздать?: (1-да/0-нет) ")) break;
            station.readFromConsole();
            stationExists = true;
            break;
        case 3:
            if (pipeExists) cout << pipe << "\n";
            else cout << "Труба еще не создана.\n";
            if (stationExists) cout << station << "\n";
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
        case 6: {
            ofstream out(dataFile);
            if (out && savePipeData(out, pipeExists, pipe) && saveStationData(out, stationExists, station))
                cout << "Данные сохранены.\n";
            else
                cout << "Ошибка сохранения данных.\n";
            break;
        }
        case 7: {
            ifstream in(dataFile);
            if (in && loadPipeData(in, pipeExists, pipe) && loadStationData(in, stationExists, station))
                cout << "Данные загружены.\n";
            else
                cout << "Ошибка загрузки данных.\n";
            break;
        }
        case 0:
            return 0;
        }
    }
}
