#include <iostream>
#include "Warehouse.h"
#include "Menu.h"

int main() {
    system("chcp 65001 > nul");
    std::setlocale(LC_ALL, ".65001");

    Warehouse centralWarehouse("Центральный склад электроники");

    Menu appMenu(centralWarehouse);
    appMenu.run();

    return 0;
}