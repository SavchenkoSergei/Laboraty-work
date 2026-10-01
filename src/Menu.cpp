#include "Menu.h"
#include <iostream>
#include <string>
#include <format>
#include <cctype>

Menu::Menu(Warehouse& wh) : warehouse(wh) {}

int Menu::getMenuChoice() const {
    int choice = -1;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidDataException("Введен некорректный пункт меню. Ожидается число.");
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
    StockItem* item = selectStockItem();
    if (item && item->device) {
        std::cout << "\n=== Детальные характеристики ===\n";
        std::cout << *(item->device) << "\n";
        std::cout << std::format("Количество на складе: {} шт.\n", item->quantity);
    }
}

void Menu::printMainMenu() const {
    std::cout << "\n=== Меню управления складом ===\n"
        << "1. Добавить устройство\n"
        << "2. Удалить устройство по модели\n"
        << "3. Списать/Уменьшить количество товара\n"
        << "4. Показать весь каталог\n"
        << "5. Показать характеристики определенной модели\n"
        << "6. Редактировать устройство\n"
        << "7. Сортировать по цене\n"
        << "8. Загрузить тестовые данные\n"
        << "9. Показать журнал операций (Логи)\n"
        << "10. Очистить весь склад\n"
        << "0. Выход\n"
        << "Выберите пункт: ";
}

StockItem* Menu::selectStockItem() const {
    std::cout << "Введите название модели или номер позиции из каталога: ";
    std::string input;
    std::getline(std::cin >> std::ws, input);

    if (input.empty()) {
        throw InvalidDataException("Ввод не может быть пустым");
    }

    bool isNumber = true;
    for (char ch : input) {
        if (!std::isdigit(static_cast<unsigned char>(ch))) {
            isNumber = false;
            break;
        }
    }

    if (isNumber) {
        size_t index = std::stoull(input);
        return warehouse.getStockItemByCatalogNumber(index);
    }

    return warehouse.findStockItemByModel(input);
}

void Menu::handleClearWarehouse() {
    std::cout << "Вы уверены, что хотите полностью очистить склад? (1 - Да, 0 - Нет): ";

    if (int confirm = 0; (std::cin >> confirm) && confirm == 1) {
        warehouse.clearWarehouse();
        std::cout << "Склад успешно очищен.\n";
    }
    else {
        std::cout << "Очистка отменена.\n";
    }

    std::cin.ignore(10000, '\n');
}

void Menu::handleReduceStock() {
    std::cout << "=== Списание товара со склада ===\n";
    std::cout << "Введите номер позиции из каталога: ";
    size_t catalogNumber = 0;
    if (!(std::cin >> catalogNumber)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidDataException("Введен некорректный номер позиции");
    }

    std::cout << "Введите количество для списания: ";
    int amount = 0;
    if (!(std::cin >> amount)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidDataException("Введено некорректное количество");
    }

    warehouse.reduceStockQuantity(catalogNumber, amount);
    std::cout << "Товар успешно списан!\n";
}

void Menu::run() {
    int choice = -1;
    while (choice != 0) {
        printMainMenu();

        try {
            choice = getMenuChoice();

            switch (choice) {
            case 1:
                handleAddDevice();
                break;
            case 2:
                handleDeleteDevice();
                break;
            case 3:
                handleReduceStock();
                break;
            case 4:
                handlePrintWarehouse();
                break;
            case 5:
                handlePrintDeviceDetails();
                break;
            case 6:
                handleEditDevice();
                break;
            case 7:
                handleSortByPrice();
                break;
            case 8:
                handleLoadTestData();
                break;
            case 9:
                warehouse.showHistory();
                break;
            case 10:
                handleClearWarehouse();
                break;
            case 0:
                std::cout << "Выход из программы...\n";
                break;
            default:
                std::cout << "Неверный пункт меню. Попробуйте снова.\n";
                break;
            }
        }
        catch (const WarehouseException&) {
            handleException(std::current_exception());
        }
    }
}

void Menu::handleException(std::exception_ptr eptr) const {
    if (!eptr) return;

    try {
        std::rethrow_exception(eptr);
    }
    catch (const InvalidDataException& ex) {
        std::cout << std::format("\n[ОШИБКА ВВОДА] {}\n", ex.what());
    }
    catch (const ObjectNotFoundException& ex) {
        std::cout << std::format("\n[ОШИБКА ПОИСКА] {}\n", ex.what());
    }
    catch (const DuplicateItemException& ex) {
        std::cout << std::format("\n[ОШИБКА КОНФЛИКТА] {}\n", ex.what());
    }
    catch (const ConstraintViolationException& ex) {
        std::cout << std::format("\n[ОШИБКА ОГРАНИЧЕНИЯ] {}\n", ex.what());
    }
    catch (const InvalidOperationException& ex) {
        std::cout << std::format("\n[ОШИБКА ОПЕРАЦИИ] {}\n", ex.what());
    }
    catch (const OutOfBoundsException& ex) {
        std::cout << std::format("\n[ОШИБКА ИНДЕКСАЦИИ] {}\n", ex.what());
    }
    catch (const BrokenLinkException& ex) {
        std::cout << std::format("\n[ОШИБКА СВЯЗИ] {}\n", ex.what());
    }
    catch (const WarehouseException& ex) {
        std::cout << std::format("\n[ОБЩАЯ ОШИБКА СКЛАДА] {}\n", ex.what());
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
        std::cout << "Возвращение в главное меню...\n";
        return;
    default:
        throw InvalidDataException("Выбран некорректный тип устройства.");
    }

    std::cin >> *newDev;

    std::cout << "Введите количество на склад: ";
    int quantity = 0;
    if (!(std::cin >> quantity) || quantity <= 0) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidDataException("Количество товара на складе должно быть целым положительным числом.");
    }
    std::cin.ignore(10000, '\n');

    warehouse += StockItem{ std::move(newDev), quantity };
    std::cout << "Устройство успешно добавлено на склад!\n";
}

void Menu::handleDeleteDevice() {
    std::string model;
    std::cout << "Введите модель устройства для удаления: ";
    std::getline(std::cin >> std::ws, model);

    if (model.empty()) {
        throw InvalidDataException("Название модели не может быть пустым");
    }

    warehouse -= model;
}

void Menu::handlePrintWarehouse() const {
    warehouse.printWarehouseState();
}

void Menu::handleEditDevice() const {
    StockItem* item = selectStockItem();
    if (item && item->device) {
        editDeviceMenu(*(item->device));
    }
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
    double newPrice = 0.0;
    if (!(std::cin >> newPrice) || newPrice < 0) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidDataException("Введена некорректная цена");
    }
    device.setPrice(newPrice);
    std::cin.ignore(10000, '\n');
}

void Menu::editWarranty(ElectronicDevice& device) const {
    std::cout << "Введите новый срок гарантии (мес.): ";
    int newWarranty = 0;
    if (!(std::cin >> newWarranty) || newWarranty < 0) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidDataException("Введен некорректный срок гарантии");
    }
    device.setWarrantyMonths(newWarranty);
    std::cin.ignore(10000, '\n');
}

void Menu::editManufacturer(ElectronicDevice& device) const {
    std::cout << "Введите нового производителя: ";
    std::string newManufacturer;
    std::getline(std::cin >> std::ws, newManufacturer);
    if (newManufacturer.empty()) {
        throw InvalidDataException("Название производителя не может быть пустым");
    }
    device.setManufacturer(newManufacturer);
}

void Menu::editModel(ElectronicDevice& device) const {
    std::cout << "Введите новую модель: ";
    std::string newModel;
    std::getline(std::cin >> std::ws, newModel);
    if (newModel.empty()) {
        throw InvalidDataException("Название модели не может быть пустым");
    }
    device.setModel(newModel);
}

void Menu::editExtraSpec(ElectronicDevice& device) const {
    std::cout << std::format("Текущие доп. характеристики: {}\n", device.getExtraSpec());
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
            throw InvalidDataException(std::format("Выбран несуществующий пункт меню редактирования ({})", choice));
        }
    }
}

void Menu::handleSortByPrice() {
    warehouse.sortByPrice();
}