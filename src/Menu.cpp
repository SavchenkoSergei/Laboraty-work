#include "Menu.h"
#include "Warehouse.h"
#include "ElectronicDevice.h"
#include "Smartphone.h"
#include "Tablet.h"
#include "Laptop.h"
#include "HomeAppliance.h"
#include <iostream>
#include <memory>
#include <string>

Menu::Menu(Warehouse& wh) : warehouse(wh) {}

int Menu::getMenuChoice() const {
    int choice = -1;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Неверный ввод! Введите число.\n";
        return -1;
    }
    std::cin.ignore(10000, '\n');
    return choice;
}

void Menu::handleLoadTestData() {
    warehouse += StockItem{ std::make_unique<Smartphone>("iPhone 15 Pro", "Apple", 4200.0, 12, 8, "iOS 17"), 15 };
    warehouse += StockItem{ std::make_unique<Tablet>("Galaxy Tab S9", "Samsung", 2600.0, 24, 11.0, true), 8 };
    warehouse += StockItem{ std::make_unique<Laptop>("ThinkPad X1 Carbon", "Lenovo", 6300.0, 36, "Intel Core i7-1370P", 57), 5 };
    warehouse += StockItem{ std::make_unique<HomeAppliance>("Series 6 Washing Machine", "Bosch", 2200.0, 24, "A+++", 2300), 3 };
    std::cout << "Тестовые данные успешно загружены на склад!\n";
}

void Menu::handlePrintDeviceDetails() const {
    std::cout << "Введите название модели для просмотра: ";
    std::string modelName;
    std::getline(std::cin, modelName);

    const auto* item = warehouse.findStockItemByModel(modelName);
    if (item && item->device) {
        std::cout << "\n=== Информация об устройстве ==="
            << "\nХарактеристики: " << *item->device
            << "\nКоличество на складе: " << item->quantity << " шт.\n";
    }
    else {
        std::cout << "Устройство с моделью \"" << modelName << "\" не найдено.\n";
    }
}

void Menu::printMainMenu() const {
    std::cout << "\n=== Меню управления складом ===\n"
        << "1. Добавить устройство\n"
        << "2. Удалить устройство по модели\n"
        << "3. Показать весь каталог\n"
        << "4. Показать характеристики определенной модели\n"
        << "5. Редактировать устройство\n"
        << "6. Сортировать по цене\n"
        << "7. Загрузить тестовые данные\n"
        << "0. Выход\n"
        << "Выберите пункт: ";
}

void Menu::run() {
    int choice = -1;
    while (choice != 0) {
        printMainMenu();
        choice = getMenuChoice();

        switch (choice) {
        case 1:
            handleAddDevice();
            break;
        case 2:
            handleDeleteDevice();
            break;
        case 3:
            handlePrintWarehouse();
            break;
        case 4:
            handlePrintDeviceDetails();
            break;
        case 5:
            handleEditDevice();
            break;
        case 6:
            handleSortByPrice();
            break;
        case 7:
            handleLoadTestData();
            break;
        case 0:
            std::cout << "Выход из программы...\n";
            break;
        default:
            std::cout << "Неверный пункт меню. Попробуйте снова.\n";
            break;
        }
    }
}

void Menu::printAddDeviceMenu() const {
    std::cout << "\n--- Добавление нового устройства ---\n"
        << "1. Смартфон\n"
        << "2. Планшет\n"
        << "3. Ноутбук\n"
        << "4. Бытовая техника\n"
        << "0. Отмена\n"
        << "Выберите тип устройства: ";
}

void Menu::handleAddDevice() {
    printAddDeviceMenu();
    int typeChoice = getMenuChoice();

    std::unique_ptr<ElectronicDevice> newDev = nullptr;

    switch (typeChoice) {
    case 1:
        newDev = std::make_unique<Smartphone>("", "", 0, 0, 0, "");
        break;
    case 2:
        newDev = std::make_unique<Tablet>("", "", 0, 0, 0, false);
        break;
    case 3:
        newDev = std::make_unique<Laptop>("", "", 0, 0, "", 0);
        break;
    case 4:
        newDev = std::make_unique<HomeAppliance>("", "", 0, 0, "", 0);
        break;
    case 0:
        std::cout << "Возвращение в гланвное меню...\n";
        return;
    default:
        std::cout << "Неверный тип устройства.\n";
        return;
    }

    std::cin >> *newDev;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Ошибка ввода.\n";
        return;
    }

    std::cout << "Введите количество на склад: ";
    if (int quantity = 0; std::cin >> quantity && quantity > 0) {
        warehouse += StockItem{ std::move(newDev), quantity };
        std::cout << "Устройство успешно добавлено на склад!\n";
    }
    else {
        std::cout << "Некорректное количество.\n";
        std::cin.clear();
    }
    std::cin.ignore(10000, '\n');
}

void Menu::handleDeleteDevice() {
    std::string model;
    std::cout << "Введите модель устройства для удаления: ";
    std::getline(std::cin, model);
    warehouse -= model;
}

void Menu::handlePrintWarehouse() const{
    warehouse.printWarehouseState();
}

void Menu::handleEditDevice() {
    std::string model;
    std::cout << "Введите модель устройства для редактирования: ";
    std::getline(std::cin, model);

    ElectronicDevice* dev = warehouse.findDeviceByModel(model);
    if (!dev) {
        std::cout << "Устройство с такой моделью не найдено.\n";
        return;
    }

    editDeviceMenu(*dev);
}

void Menu::printEditMenu(const ElectronicDevice& device) const {
    std::cout << "\n--- Редактирование устройства ---"
        << "\n1. Изменить цену (" << device.getPrice() << " BYN)"
        << "\n2. Изменить гарантию (" << device.getWarrantyMonths() << " мес.)"
        << "\n3. Изменить производителя (" << device.getManufacturer() << ")"
        << "\n4. Изменить модель (" << device.getModel() << ")"
        << "\n5. Изменить специфические характеристики (" << device.getExtraSpec() << ")"
        << "\n0. Завершить редактирование"
        << "\nВыберите пункт: ";
}

void Menu::editPrice(ElectronicDevice& device) const {
    std::cout << "Введите новую цену (BYN): ";
    if (double newPrice = 0.0; std::cin >> newPrice) {
        device.setPrice(newPrice);
    }
    std::cin.ignore(10000, '\n');
}

void Menu::editWarranty(ElectronicDevice& device) const {
    std::cout << "Введите новый срок гарантии (мес.): ";
    if (int newWarranty = 0; std::cin >> newWarranty) {
        device.setWarrantyMonths(newWarranty);
    }
    std::cin.ignore(10000, '\n');
}

void Menu::editManufacturer(ElectronicDevice& device) const {
    std::cout << "Введите нового производителя: ";
    if (std::string newManufacturer; std::getline(std::cin, newManufacturer) && !newManufacturer.empty()) {
        device.setManufacturer(newManufacturer);
    }
}

void Menu::editModel(ElectronicDevice& device) const {
    std::cout << "Введите новую модель: ";
    if (std::string newModel; std::getline(std::cin, newModel) && !newModel.empty()) {
        device.setModel(newModel);
    }
}

void Menu::editExtraSpec(ElectronicDevice& device) const {
    device.setExtraSpec("");
    std::cout << "Доп. характеристики успешно обновлены!\n";
}

void Menu::editDeviceMenu(ElectronicDevice& device) const {
    int choice = -1;
    while (choice != 0) {
        printEditMenu(device);
        choice = getMenuChoice();

        switch (choice) {
        case 1:
            editPrice(device);
            break;
        case 2:
            editWarranty(device);
            break;
        case 3:
            editManufacturer(device);
            break;
        case 4:
            editModel(device);
            break;
        case 5:
            editExtraSpec(device);
            break;
        case 0:
            std::cout << "Редактирование завершено.\n";
            break;
        default:
            std::cout << "Неверный пункт меню!\n";
            break;
        }
    }
}

void Menu::handleSortByPrice() {
    warehouse.sortByPrice();
}