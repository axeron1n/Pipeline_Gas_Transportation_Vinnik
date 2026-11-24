#include "GasNetwork.h"
#include "utils.h"
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <set>
#include <utility>
#ifdef _WIN32   // поддержка русского языка
#define NOMINMAX
#include <Windows.h>
#endif

using namespace std;

// Просит id существующего объекта, пока не введут верный; 0 - отмена
template <typename T>
int readExistingId(const unordered_map<int, T>& objects, const string prompt)
{
    if (objects.empty()) {
        cout << "Список пуст.\n";
        return 0;
    }
    while (true) {
        int id = readNumber(prompt + " (0 - отмена): ", 0, numeric_limits<int>::max());
        if (id == 0 || objects.count(id) > 0)
            return id;
        cout << "Объект с id " << id << " не найден.\n";
    }
}

void addPipe(GasNetwork& network)
{
    Pipe pipe;
    if (!pipe.readFromConsole()) {
        cout << " Не удалось добавить трубу: достигнут предел ID.\n";
        return;
    }
    const Pipe& added = network.addPipe(move(pipe));
    cout << "Добавлено: " << added << "\n";
}

void addStation(GasNetwork& network)
{
    CompressorStation station;
    if (!station.readFromConsole()) {
        cout << "Не удалось добавить КС: достигнут предел ID.\n";
        return;
    }
    const CompressorStation& added = network.addStation(move(station));
    cout << "Добавлено: " << added << "\n";
}

void showAll(const GasNetwork& network)
{
    const auto& pipes = network.getPipes();
    const auto& stations = network.getStations();
    cout << "Трубы (" << pipes.size() << "):\n";
    printObjects(pipes);
    cout << "КС (" << stations.size() << "):\n";
    printObjects(stations);
}

void editPipe(GasNetwork& network)
{
    int id = readExistingId(network.getPipes(), "id трубы");
    if (id == 0)
        return;
    Pipe& pipe = network.getPipe(id);
    pipe.setUnderRepair(readYesNo("Труба в ремонте? (1-да/0-нет): "));
    cout << "Изменено: " << pipe << "\n";
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
        cout << station << ". " << (start ? "Недостаточно свободных цехов." : "Недостаточно работающих цехов.") << "\n";
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

void editStation(GasNetwork& network)
{
    int id = readExistingId(network.getStations(), "id КС");
    if (id != 0)
        editStationShops(network.getStation(id));
}

void deletePipe(GasNetwork& network)
{
    int id = readExistingId(network.getPipes(), "id трубы для удаления");
    if (id != 0 && network.removePipe(id))
        cout << "Труба id=" << id << " удалена.\n";
}

void deleteStation(GasNetwork& network)
{
    int id = readExistingId(network.getStations(), "id КС для удаления");
    if (id != 0 && network.removeStation(id))
        cout << "КС id=" << id << " удалена.\n";
}

void searchPipes(const GasNetwork& network)
{
    cout << "Поиск труб:\n1. По названию\n2. По признаку «в ремонте»\n0. Назад\n";
    int choice = readNumber("Выберите фильтр: ", 0, 2);
    if (choice == 0)
        return;
    set<int> ids;
    if (choice == 1)
        ids = network.findPipesByName(readNonEmptyLine("Часть названия: "));
    else
        ids = network.findPipesByRepair(readYesNo("Искать трубы в ремонте? (1-да/0-нет): "));
    cout << "Найдено труб: " << ids.size() << "\n";
    printObjects(network.getPipes(), ids);
}

void searchStations(const GasNetwork& network)
{
    cout << "Поиск КС:\n1. По названию\n2. По проценту незадействованных цехов\n0. Назад\n";
    int choice = readNumber("Выберите фильтр: ", 0, 2);
    if (choice == 0)
        return;

    set<int> ids;
    if (choice == 1) {
        ids = network.findStationsByName(readNonEmptyLine("Часть названия: "));
    }
    else {
        double minPercent = readNumber("Процент незадействованных цехов от (0..100): ", 0.0, 100.0);
        double maxPercent = readNumber("до (не меньше нижней границы, не больше 100): ", minPercent, 100.0);
        ids = network.findStationsByUnusedPercent(minPercent, maxPercent);
    }

    cout << "Найдено КС: " << ids.size() << "\n";
    printObjects(network.getStations(), ids);
}

void saveData(const GasNetwork& network)
{
    string fileName = readNonEmptyLine("Имя файла для сохранения: ");
    if (network.save(fileName))
        cout << "Данные сохранены в файл «" << fileName << "».\n";
    else
        cout << "Не удалось сохранить данные в файл «" << fileName << "».\n";
}

void loadData(GasNetwork& network)
{
    string fileName = readNonEmptyLine("Имя файла для загрузки: ");
    if (network.load(fileName))
        cout << "Данные загружены из файла «" << fileName << "».\n";
    else
        cout << "Не удалось загрузить данные из файла «" << fileName << "».\n";
}

void printMenu()
{
    cout << "\n1. Добавить трубу\n"
        << "2. Добавить КС\n"
        << "3. Просмотр всех объектов\n"
        << "4. Редактировать трубу\n"
        << "5. Редактировать КС\n"
        << "6. Удалить трубу\n"
        << "7. Удалить КС\n"
        << "8. Поиск труб\n"
        << "9. Поиск КС\n"
        << "10. Сохранить\n"
        << "11. Загрузить\n"
        << "0. Выход\n";
}

int main()
{
#ifdef _WIN32   // поддержка русского языка
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    GasNetwork network;

    while (true) {
        printMenu();
        switch (readNumber("Выберите действие: ", 0, 11)) {
        case 1:
            addPipe(network);
            break;
        case 2:
            addStation(network);
            break;
        case 3:
            showAll(network);
            break;
        case 4:
            editPipe(network);
            break;
        case 5:
            editStation(network);
            break;
        case 6:
            deletePipe(network);
            break;
        case 7:
            deleteStation(network);
            break;
        case 8:
            searchPipes(network);
            break;
        case 9:
            searchStations(network);
            break;
        case 10:
            saveData(network);
            break;
        case 11:
            loadData(network);
            break;
        case 0:
            return 0;
        }
    }
}
