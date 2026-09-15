#include "Menu.h"
#include <iostream>
#include <string>

Menu::Menu(Warehouse& wh) : warehouse(wh) {}

void Menu::run() {
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
            warehouse.printWarehouseState();
            break;
        case 2:
            handleAddDevice();
            break;
        case 3:
            handleLoadTestData();
            break;
        case 4:
            handleEditDeviceProperties();
            break;
        case 5:
            handleGetIndividualCharacteristics();
            break;
        case 6:
            handleRemoveDevice();
            break;
        case 7:
            handleSortByPrice();
            break;
        case 0:
            std::cout << "Выход из программы. До свидания!\n";
            break;
        default:
            std::cout << "Неизвестный пункт меню. Попробуйте снова.\n";
            break;
        }
    }
}

void Menu::handleLoadTestData() {
    ElectronicDevice dev1("Смартфон", "Galaxy S23", "Samsung", 2500.0, 12, "AMOLED экран, 128GB");
    ElectronicDevice dev2("Ноутбук", "MacBook Pro", "Apple", 6000.0, 24, "M2 Pro, 16GB");
    ElectronicDevice dev3("Наушники", "AirPods Pro", "Apple", 750.0, 12, "Активное шумоподавление");

    warehouse += StockItem{ dev1, 5 };
    warehouse += StockItem{ dev2, 2 };
    warehouse += StockItem{ dev3, 10 };

    std::cout << "Тестовый набор из 3 устройств успешно загружен на склад!\n";
}

void Menu::handleSortByPrice() {
    warehouse.sortByPrice();
}

void Menu::handleAddDevice() {
    ElectronicDevice newDev("", "", "", 0.0, 0, "");
    std::cin >> newDev;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Операция добавления отменена из-за неверного ввода.\n";
        return;
    }

    int quantity = 0;
    std::cout << "Введите количество на склад: ";
    if (!(std::cin >> quantity) || quantity <= 0) {
        std::cout << "Ошибка: некорректное количество!\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }
    std::cin.ignore(10000, '\n');

    warehouse += StockItem{ newDev, quantity };
}

void Menu::updateType(ElectronicDevice& device) const{
    std::string buffer;
    std::cout << "Введите новый тип: ";
    std::getline(std::cin, buffer);
    device.setType(buffer);
}

void Menu::updateModel(ElectronicDevice& device) const{
    std::string buffer;
    std::cout << "Введите новую модель: ";
    std::getline(std::cin, buffer);
    device.setModel(buffer);
}

void Menu::updateManufacturer(ElectronicDevice& device) const {
    std::string buffer;
    std::cout << "Введите нового производителя: ";
    std::getline(std::cin, buffer);
    device.setManufacturer(buffer);
}

void Menu::updatePrice(ElectronicDevice& device) const {
    double price = 0.0;
    std::cout << "Введите новую цену: ";
    std::cin >> price;
    std::cin.ignore(10000, '\n');
    device.setPrice(price);
}

void Menu::updateWarranty(ElectronicDevice& device) const {
    int warranty = 0;
    std::cout << "Введите новый гарантийный срок: ";
    std::cin >> warranty;
    std::cin.ignore(10000, '\n');
    device.setWarrantyMonths(warranty);
}

void Menu::updateExtraSpec(ElectronicDevice& device) const {
    std::string buffer;
    std::cout << "Введите новую доп. характеристику: ";
    std::getline(std::cin, buffer);
    device.setExtraSpec(buffer);
}

void Menu::printEditMenu(const ElectronicDevice& device) const {
    std::cout << "\n--- Редактирование характеристик товара (" << device.getModel() << ") ---\n"
        << "1. Изменить тип (текущий: " << device.getType() << ")\n"
        << "2. Изменить модель (текущая: " << device.getModel() << ")\n"
        << "3. Изменить производителя (текущий: " << device.getManufacturer() << ")\n"
        << "4. Изменить цену (текущая: " << device.getPrice() << " руб.)\n"
        << "5. Изменить гарантию (текущая: " << device.getWarrantyMonths() << " мес.)\n"
        << "6. Изменить доп. характеристику (текущая: " << device.getExtraSpec() << ")\n"
        << "0. Вернуться в главное меню\n"
        << "Выберите пункт: ";
}

void Menu::editDeviceMenu(ElectronicDevice& device) const{
    int subChoice = -1;
    while (subChoice != 0) {
        printEditMenu(device);

        if (!(std::cin >> subChoice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            break;
        }
        std::cin.ignore(10000, '\n');

        switch (subChoice) {
        case 1: 
            updateType(device);
            break;
        case 2: 
            updateModel(device); 
            break;
        case 3: 
            updateManufacturer(device); 
            break;
        case 4: 
            updatePrice(device); 
            break;
        case 5: 
            updateWarranty(device); 
            break;
        case 6: 
            updateExtraSpec(device); 
            break;
        case 0: 
            break;
        default: 
            std::cout << "Неверный пункт меню.\n"; 
            break;
        }
    }
}

void Menu::handleEditDeviceProperties() {
    std::string model;
    std::cout << "Введите модель товара для изменения характеристик: ";
    std::getline(std::cin, model);

    ElectronicDevice* foundDevice = warehouse.findDeviceByModel(model);
    if (foundDevice == nullptr) {
        std::cout << "Товар с такой моделью не найден на складе!\n";
        return;
    }

    editDeviceMenu(*foundDevice);
}

void Menu::handleGetIndividualCharacteristics() {
    std::string model;
    std::cout << "Введите модель товара для просмотра: ";
    std::getline(std::cin, model);

    const StockItem* foundItem = warehouse.findStockItemByModel(model);
    if (foundItem == nullptr) {
        std::cout << "Товар с такой моделью не найден на складе!\n";
        return;
    }

    std::cout << "\n--- Информация о товаре ---\n"
        << "Остаток на складе: " << foundItem->quantity << " шт.\n"
        << "Характеристики: " << foundItem->device << "\n\n";
}

void Menu::showMenu() const {
    std::cout << "\n----------------- МЕНЮ СКЛАДА -----------------\n"
        << "1. Показать каталог и состояние склада\n"
        << "2. Добавить новый товар\n"
        << "3. Быстрая загрузка тестовых товаров\n"
        << "4. Изменить характеристики товара\n"
        << "5. Посмотреть информацию об отдельном товаре\n"
        << "6. Удалить товар со склада\n"
        << "7. Отсортировать склад по цене\n"
        << "0. Выход\n"
        << "Выберите пункт меню: ";
}

void Menu::handleRemoveDevice() {
    std::string model;
    std::cout << "Введите модель устройства для удаления: ";
    std::getline(std::cin, model);

    warehouse.removeDeviceByModel(model);
}