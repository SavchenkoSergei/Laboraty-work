#include "Menu.h"
#include <iostream>
#include <string>

void handleAddDevice(Warehouse& warehouse) {
    std::string type;
    std::string model;
    std::string manufacturer;
    std::string extraSpec;
    double price = 0;
    int warranty = 0;
    int quantity = 0;

    std::cout << "Введите тип товара: ";
    std::getline(std::cin, type);
    std::cout << "Введите модель: ";
    std::getline(std::cin, model);
    std::cout << "Введите производителя: ";
    std::getline(std::cin, manufacturer);

    std::cout << "Введите цену (руб.): ";
    std::cin >> price;
    std::cout << "Введите гарантию (мес.): ";
    std::cin >> warranty;
    std::cout << "Введите количество на склад: ";
    std::cin >> quantity;
    std::cin.ignore(10000, '\n');

    std::cout << "Введите доп. характеристики: ";
    std::getline(std::cin, extraSpec);

    ElectronicDevice newDev(type, model, manufacturer, price, warranty, extraSpec);
    warehouse.addDevice(newDev, quantity);
}

static void updateType(ElectronicDevice& device) {
    std::string buffer;
    std::cout << "Введите новый тип: ";
    std::getline(std::cin, buffer);
    device.setType(buffer);
}

static void updateModel(ElectronicDevice& device) {
    std::string buffer;
    std::cout << "Введите новую модель: ";
    std::getline(std::cin, buffer);
    device.setModel(buffer);
}

static void updateManufacturer(ElectronicDevice& device) {
    std::string buffer;
    std::cout << "Введите нового производителя: ";
    std::getline(std::cin, buffer);
    device.setManufacturer(buffer);
}

static void updatePrice(ElectronicDevice& device) {
    double price = 0.0;
    std::cout << "Введите новую цену: ";
    std::cin >> price;
    std::cin.ignore(10000, '\n');
    device.setPrice(price);
}

static void updateWarranty(ElectronicDevice& device) {
    int warranty = 0;
    std::cout << "Введите новый гарантийный срок: ";
    std::cin >> warranty;
    std::cin.ignore(10000, '\n');
    device.setWarrantyMonths(warranty);
}

static void updateExtraSpec(ElectronicDevice& device) {
    std::string buffer;
    std::cout << "Введите новую доп. характеристику: ";
    std::getline(std::cin, buffer);
    device.setExtraSpec(buffer);
}

static void printEditMenu(const ElectronicDevice& device) {
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

static void editDeviceMenu(ElectronicDevice& device) {
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
        case 1: updateType(device); break;
        case 2: updateModel(device); break;
        case 3: updateManufacturer(device); break;
        case 4: updatePrice(device); break;
        case 5: updateWarranty(device); break;
        case 6: updateExtraSpec(device); break;
        case 0: break;
        default: std::cout << "Неверный пункт меню.\n"; break;
        }
    }
}

void handleEditDeviceProperties(Warehouse& warehouse) {
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

void handleGetIndividualCharacteristics(Warehouse& warehouse) {
    std::string model;
    std::cout << "Введите модель товара для просмотра отдельных характеристик: ";
    std::getline(std::cin, model);

    const ElectronicDevice* foundDevice = warehouse.findDeviceByModel(model);
    if (foundDevice == nullptr) {
        std::cout << "Товар с такой моделью не найден на складе!\n";
        return;
    }

    std::cout << "\n--- Отдельные характеристики товара ---\n"
        << "Тип: " << foundDevice->getType() << "\n"
        << "Модель: " << foundDevice->getModel() << "\n"
        << "Производитель: " << foundDevice->getManufacturer() << "\n"
        << "Цена: " << foundDevice->getPrice() << " руб.\n"
        << "Гарантия: " << foundDevice->getWarrantyMonths() << " мес.\n"
        << "Доп. характеристика: " << foundDevice->getExtraSpec() << "\n";
}

void showMenu() {
    std::cout << "\n----------------- МЕНЮ СКЛАДА -----------------\n"
        << "1. Показать каталог и состояние склада\n"
        << "2. Добавить новый товар\n"
        << "3. Изменить характеристики товара\n"
        << "4. Посмотреть информацию об отдельном товаре\n"
        << "0. Выход\n"
        << "Выберите пункт меню: ";
}