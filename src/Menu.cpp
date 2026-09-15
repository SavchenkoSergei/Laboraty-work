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
            handleEditDeviceProperties();
            break;
        case 5:
            handleGetIndividualCharacteristics();
            break;
        case 6:
            handleRemoveDevice();
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

void Menu::handleAddDevice(){
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

    if (price < 0) {
        std::cout << "Ошибка: цена не может быть отрицательной!\n";
        return;
    }

    device.setPrice(price);
    std::cout << "Цена успешно обновлена.\n";
}

void Menu::updateWarranty(ElectronicDevice& device) const {
    int warranty = 0;
    std::cout << "Введите новый гарантийный срок: ";
    std::cin >> warranty;
    std::cin.ignore(10000, '\n');

    if (warranty < 0) {
        std::cout << "Ошибка: гарантийный срок не может быть отрицательным!\n";
        return;
    }

    device.setWarrantyMonths(warranty);
    std::cout << "Гарантийный срок успешно обновлён.\n";
}

void Menu::updateExtraSpec(ElectronicDevice& device) const {
    std::string buffer;
    std::cout << "Введите новую доп. характеристику: ";
    std::getline(std::cin, buffer);
    device.setExtraSpec(buffer);
}

void Menu::printEditMenu(const ElectronicDevice& device) const{
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

void Menu::showMenu() const{
    std::cout << "\n----------------- МЕНЮ СКЛАДА -----------------\n"
        << "1. Показать каталог и состояние склада\n"
        << "2. Добавить новый товар\n"
        << "3. Изменить характеристики товара\n"
        << "4. Посмотреть информацию об отдельном товаре\n"
        << "5. Удаление товара\n"
        << "0. Выход\n"
        << "Выберите пункт меню: ";
}

void Menu::handleRemoveDevice() {
    std::string model;
    std::cout << "Введите модель устройства для удаления: ";

    std::getline(std::cin, model);

    if (warehouse.removeDeviceByModel(model)) {
        std::cout << "Товар успешно удален.\n";
    }
    else {
        std::cout << "Товар не найден.\n";
    }
}