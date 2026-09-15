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
    (void)pipe; // переменная пока не используется, добавил в промежуточном варианте, чтобы не было ошибок
    (void)station; // переменная пока не используется, добавил в промежуточном варианте, чтобы не было ошибок

    int choice = -1;
    while (choice != 0) {
        printMenu();
        cout << "Выберите действие: ";
        cin >> choice;
    }

}
