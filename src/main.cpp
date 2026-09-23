#include <iostream>
#include "Warehouse.h"
#include "Menu.h"
#include <Windows.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Warehouse centralWarehouse("Центральный склад электроники");

    Menu appMenu(centralWarehouse);
    appMenu.run();

    return 0;
}