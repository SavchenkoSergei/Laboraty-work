#include <iostream>
#include "Warehouse.h"
#include "Menu.h"

int main() {
    system("chcp 65001 > nul");
    std::setlocale(LC_ALL, ".65001");

    Warehouse centralWarehouse("Центральный склад электроники");

    int choice = -1;
    while (choice != 0) {
        showMenu();
        if (!(std::cin >> choice)) {
            std::cout << "Некорректный ввод. Завершение работы.\n";
            break;
        }
        std::cin.ignore(10000, '\n');

        switch (choice) {
        case 1:
            centralWarehouse.printWarehouseState();
            break;
        case 2:
            handleAddDevice(centralWarehouse);
            break;
        case 3:
            handleEditDeviceProperties(centralWarehouse);
            break;
        case 4:
            handleGetIndividualCharacteristics(centralWarehouse);
            break;
        case 5:
            handleRemoveDevice(centralWarehouse);
            break;
        case 0:
            std::cout << "Выход из программы. До свидания!\n";
            break;
        default:
            std::cout << "Неизвестный пункт меню. Попробуйте снова.\n";
            break;
        }
    }

    return 0;
}