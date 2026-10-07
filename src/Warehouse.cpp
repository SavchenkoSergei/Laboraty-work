#include "Warehouse.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <format>
#include <algorithm>
#include <numeric>
#include <cctype>

std::string Warehouse::getCurrentTimestamp() {
    const auto now = std::chrono::system_clock::now();
    const auto sec = std::chrono::floor<std::chrono::seconds>(now);
    const std::chrono::sys_days days = std::chrono::floor<std::chrono::days>(sec);
    const std::chrono::year_month_day ymd{ days };
    const std::chrono::hh_mm_ss hms{ sec - days };

    return std::format("[{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}] ",
        static_cast<int>(ymd.year()),
        static_cast<unsigned>(ymd.month()),
        static_cast<unsigned>(ymd.day()),
        hms.hours().count(),
        hms.minutes().count(),
        hms.seconds().count());
}

Warehouse::Warehouse(std::string_view name) : warehouseName(name) {
    if (warehouseName.empty()) {
        throw InvalidDataException("Название склада не может быть пустым.");
    }
    logAction(std::format("Склад \"{}\" был создан.", warehouseName));
}

StockItem* Warehouse::findStockItemByModel(std::string_view model) {
    if (auto it = std::find_if(inventory.begin(), inventory.end(), [model](const StockItem& item) {
        return item.device && item.device->getModel() == model;
        }); it != inventory.end()) {
        return std::to_address(it);
    }

    throw ObjectNotFoundException(std::format("Устройство с моделью \"{}\" не найдено на складе", model));
}

StockItem* Warehouse::getStockItemByCatalogNumber(size_t catalogNumber) {
    if (catalogNumber == 0 || catalogNumber > inventory.size()) {
        throw OutOfBoundsException(std::format(
            "Позиция {} отсутствует в каталоге (всего элементов: {})", catalogNumber, inventory.size()));
    }
    return &inventory[catalogNumber - 1];
}

void Warehouse::clearWarehouse() {
    if (inventory.empty()) {
        throw InvalidOperationException("Невозможно очистить склад: он уже пуст.");
    }
    inventory.clear();
    logAction("Выполнена очистка склада.");
}

ElectronicDevice* Warehouse::findDeviceByModel(std::string_view model) {
    const StockItem* item = findStockItemByModel(model);
    return item ? item->device.get() : nullptr;
}

std::vector<StockItem> Warehouse::findByPriceRange(double minPrice, double maxPrice) const {
    std::vector<StockItem> result;

    auto cloneDevice = [](const ElectronicDevice* dev) -> std::unique_ptr<ElectronicDevice> {
        if (auto* sm = dynamic_cast<const Smartphone*>(dev)) return std::make_unique<Smartphone>(*sm);
        if (auto* tb = dynamic_cast<const Tablet*>(dev)) return std::make_unique<Tablet>(*tb);
        if (auto* lp = dynamic_cast<const Laptop*>(dev)) return std::make_unique<Laptop>(*lp);
        if (auto* ha = dynamic_cast<const HomeAppliance*>(dev)) return std::make_unique<HomeAppliance>(*ha);
        return nullptr;
        };

    for (const auto& item : inventory) {
        if (!item.device) continue;

        if (item.device->getPrice() < minPrice || item.device->getPrice() > maxPrice) continue;

        if (auto devCopy = cloneDevice(item.device.get())) {
            result.push_back(StockItem{ std::move(devCopy), item.quantity });
        }
    }

    return result;
}

void Warehouse::sortByPrice(bool ascending) {
    if (inventory.empty()) {
        throw InvalidOperationException("Невозможно отсортировать склад: каталог товаров пуст.");
    }

    std::sort(inventory.begin(), inventory.end(), [ascending](const StockItem& a, const StockItem& b) {
        if (!a.device || !b.device) return false;
        return ascending ? (a.device->getPrice() < b.device->getPrice())
            : (a.device->getPrice() > b.device->getPrice());
        });

    logAction(std::format("Выполнена сортировка товаров по цене ({})", ascending ? "по возрастанию" : "по убыванию"));
}

void Warehouse::sortByName(bool ascending) {
    if (inventory.empty()) {
        throw InvalidOperationException("Невозможно отсортировать склад: каталог товаров пуст.");
    }

    auto toLowerStr = [](std::string_view str) {
        std::string lowerStr;
        lowerStr.reserve(str.size());
        for (char ch : str) {
            lowerStr.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
        }
        return lowerStr;
        };

    std::sort(inventory.begin(), inventory.end(), [&toLowerStr, ascending](const StockItem& a, const StockItem& b) {
        if (!a.device || !b.device) return false;

        std::string modelA = toLowerStr(a.device->getModel());
        std::string modelB = toLowerStr(b.device->getModel());

        if (modelA == modelB) {
            return ascending ? (a.device->getModel() < b.device->getModel())
                : (a.device->getModel() > b.device->getModel());
        }

        return ascending ? (modelA < modelB) : (modelA > modelB);
        });

    logAction(std::format("Выполнена сортировка товаров по наименованию модели ({})",
        ascending ? "по возрастанию, А-Я" : "по убыванию, Я-А"));
}

const StockItem& Warehouse::getMostExpensiveItem() const {
    if (inventory.empty()) throw InvalidOperationException("Склад пуст");
    auto it = std::max_element(inventory.begin(), inventory.end(), [](const StockItem& a, const StockItem& b) {
        return a.device->getPrice() < b.device->getPrice();
        });
    return *it;
}

const StockItem& Warehouse::getCheapestItem() const {
    if (inventory.empty()) throw InvalidOperationException("Склад пуст");
    auto it = std::min_element(inventory.begin(), inventory.end(), [](const StockItem& a, const StockItem& b) {
        return a.device->getPrice() < b.device->getPrice();
        });
    return *it;
}

int Warehouse::countItemsMoreThan(int threshold) const {
    return static_cast<int>(std::count_if(inventory.begin(), inventory.end(), [threshold](const StockItem& item) {
        return item.quantity > threshold;}));
}

double Warehouse::calculateTotalCost() const {
    return std::accumulate(inventory.begin(), inventory.end(), 0.0, [](double sum, const StockItem& item) {
        return sum + (item.device ? item.device->getPrice() * item.quantity : 0.0);});
}

CategoryMap Warehouse::getStatsByCategory() const {
    CategoryMap stats;
    std::for_each(inventory.begin(), inventory.end(), [&stats](const StockItem& item) {
        if (item.device) {
            std::string type = item.device->getType();
            stats[type].totalQuantity += item.quantity;
            stats[type].totalValue += item.device->getPrice() * item.quantity;
        }
        });
    return stats;
}

void Warehouse::increaseStockQuantity(StockItem* item, int amount) const {
    if (amount <= 0) {
        throw InvalidDataException(std::format("Количество прихода должно быть больше нуля (введено: {})", amount));
    }

    item->quantity += amount;
    logAction(std::format("Пополнен остаток товара \"{}\" на {} шт. (Текущий остаток: {} шт.)",
        item->device->getModel(), amount, item->quantity));
}

void Warehouse::reduceStockQuantity(StockItem* item, int amount) const {
    if (amount <= 0) {
        throw InvalidDataException(std::format("Количество для списания должно быть больше нуля (введено: {})", amount));
    }

    if (item->quantity < amount) {
        throw ConstraintViolationException(std::format(
            "Запрошено к списанию {} шт., однако на складе доступно всего {} шт. товара \"{}\"",
            amount, item->quantity, item->device->getModel()
        ));
    }

    auto modelName = std::string(item->device->getModel());
    item->quantity -= amount;
    logAction(std::format("Списано {} шт. товара \"{}\"", amount, modelName));

    if (item->quantity == 0) {
        logAction(std::format("Остаток товара \"{}\" достиг 0 шт.", modelName));
    }
}

Warehouse& Warehouse::operator+=(StockItem newItem) {
    if (!newItem.device) {
        throw BrokenLinkException("Попытка добавить запись на склад без инициализированного устройства (nullptr)");
    }
    if (newItem.quantity <= 0) {
        throw ConstraintViolationException(std::format("Количество добавляемого товара должно быть больше нуля (передано: {})", newItem.quantity));
    }

    if (auto it = std::find_if(inventory.begin(), inventory.end(), [&newItem](const StockItem& item) {
        return item.device && *(item.device) == *(newItem.device);
        }); it != inventory.end()) {
        throw DuplicateItemException(std::format("Модель \"{}\" уже существует на складе. Используйте редактирование или пополнение остатков.", newItem.device->getModel()));
    }

    const auto modelName = std::string(newItem.device->getModel());
    inventory.push_back(std::move(newItem));
    logAction(std::format("Добавлен новый товар в каталог: {}", modelName));

    return *this;
}

Warehouse& Warehouse::operator-=(std::string_view model) {
    if (const StockItem* foundItem = findStockItemByModel(model); foundItem->quantity > 0) {
        throw BrokenLinkException(std::format(
            "Нельзя удалить позицию \"{}\", к ней привязаны товары на складе (в наличии: {} шт.). Сначала спишите остатки!",
            model, foundItem->quantity));
    }

    std::erase_if(inventory, [model](const StockItem& i) {return i.device && i.device->getModel() == model;});

    logAction(std::format("Удален товар по модели: {}", std::string(model)));
    return *this;
}

void Warehouse::printWarehouseState() const {
    std::cout << std::format("\n=== Состояние склада: \"{}\" ===\n", warehouseName);
    if (inventory.empty()) {
        std::cout << "Склад пуст.\n";
        return;
    }

    for (size_t i = 0; i < inventory.size(); ++i) {
        std::cout << (i + 1) << ". " << inventory[i] << "\n";
    }

    auto activeCount = static_cast<int>(std::count_if(inventory.begin(), inventory.end(), [](const StockItem& item) {
        return item.quantity > 0 && item.device != nullptr;}));

    std::cout << "Всего позиций в наличии: " << activeCount << "\n";
}

void Warehouse::showHistory() const {
    std::cout << "\n=== Журнал операций (Логи) ===\n";
    if (actionHistory.empty()) {
        std::cout << "История пуста.\n";
        return;
    }

    size_t idx = 1;
    for (const auto& logEntry : actionHistory) {
        std::cout << idx++ << ". " << logEntry << "\n";
    }
}

void Warehouse::logAction(std::string_view message) const {
    std::string entry = std::format("{}{}", getCurrentTimestamp(), message);
    actionHistory.push_back(entry);
    appendToExternalLog(entry);
}

void Warehouse::appendToExternalLog(const std::string& message) const {
    std::ofstream logFile("journal.log", std::ios::app);
    if (!logFile.is_open()) {
        throw InvalidOperationException("Не удалось открыть файл журнала journal.log для записи");
    }
    logFile << message << "\n";
}

void Warehouse::saveStateToFile(const std::string& filename) const {
    std::string saveMsg = std::format(R"(Состояние склада успешно сохранено в файл "{}")", filename);
    logAction(saveMsg);

    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw InvalidOperationException(std::format(R"(Не удалось открыть файл "{}" для записи состояния)", filename));
        }

        file << warehouseName << "\n";
        file << inventory.size() << "\n";

        for (const auto& item : inventory) {
            if (!item.device) continue;

            std::string typeTag;
            if (dynamic_cast<Smartphone*>(item.device.get())) typeTag = "SMARTPHONE";
            else if (dynamic_cast<Tablet*>(item.device.get())) typeTag = "TABLET";
            else if (dynamic_cast<Laptop*>(item.device.get())) typeTag = "LAPTOP";
            else if (dynamic_cast<HomeAppliance*>(item.device.get())) typeTag = "APPLIANCE";

            file << typeTag << "\n";
            file << item.quantity << "\n";
            item.device->saveToFile(file);
        }

        file << actionHistory.size() << "\n";
        for (const auto& logEntry : actionHistory) {
            file << logEntry << "\n";
        }

        if (file.fail()) {
            throw InvalidOperationException("Ошибка во время записи данных в файл");
        }
    }
    catch (...) {
        actionHistory.pop_back();
        throw;
    }
}


void Warehouse::loadStateFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw ObjectNotFoundException(std::format("Файл сохранения \"{}\" не найден или не может быть открыт", filename));
    }

    std::string newName;
    if (!std::getline(file >> std::ws, newName)) {
        throw InvalidDataException("Файл сохранения пуст или поврежден");
    }

    size_t itemCount = 0;
    if (!(file >> itemCount)) {
        throw InvalidDataException("Ошибка чтения количества товаров из файла");
    }

    std::vector<StockItem> tempInventory;

    for (size_t i = 0; i < itemCount; ++i) {
        std::string typeTag;
        int quantity = 0;

        if (!std::getline(file >> std::ws, typeTag) || !(file >> quantity)) {
            throw InvalidDataException(std::format("Ошибка чтения заголовка товара #{}", i + 1));
        }

        if (quantity <= 0) {
            throw ConstraintViolationException(std::format("Загруженное количество товара должно быть > 0 (получено: {})", quantity));
        }

        std::unique_ptr<ElectronicDevice> dev = nullptr;
        if (typeTag == "SMARTPHONE") dev = std::make_unique<Smartphone>("", "", 0, 0, 0, "");
        else if (typeTag == "TABLET") dev = std::make_unique<Tablet>("", "", 0, 0, 0, false);
        else if (typeTag == "LAPTOP") dev = std::make_unique<Laptop>("", "", 0, 0, "", 0);
        else if (typeTag == "APPLIANCE") dev = std::make_unique<HomeAppliance>("", "", 0, 0, "", 0);
        else {
            throw InvalidDataException(std::format("Неизвестный тип устройства в файле: {}", typeTag));
        }

        dev->loadFromFile(file);

        if (auto it = std::find_if(tempInventory.begin(), tempInventory.end(), [&dev](const StockItem& item) {
            return item.device && *(item.device) == *dev;
            }); it != tempInventory.end()) {
            throw DuplicateItemException(std::format(R"(Обнаружен дубликат устройства при загрузке: "{}")", dev->getModel()));
        }

        tempInventory.push_back(StockItem{ std::move(dev), quantity });
    }

    size_t historyCount = 0;
    std::list<std::string> tempHistory;
    if (file >> historyCount) {
        std::string line;
        for (size_t i = 0; i < historyCount; ++i) {
            if (std::getline(file >> std::ws, line)) {
                tempHistory.push_back(line);
            }
        }
    }

    warehouseName = std::move(newName);
    inventory = std::move(tempInventory);
    actionHistory = std::move(tempHistory);

    logAction(std::format(R"(Состояние склада успешно загружено из файла "{}")", filename));
}

void Warehouse::generateReport(const std::string& filename) const {
    std::ofstream report(filename);
    if (!report.is_open()) {
        throw InvalidOperationException(std::format("Не удалось создать файл отчета \"{}\"", filename));
    }

    double totalCost = calculateTotalCost();
    int totalItemsCount = std::accumulate(inventory.begin(), inventory.end(), 0, [](int sum, const StockItem& item) {
        return sum + item.quantity;
        });

    report << "====================================================\n"
        << "          ОТЧЕТ ПО СОСТОЯНИЮ СКЛАДА                 \n"
        << "====================================================\n"
        << "Название склада: " << warehouseName << "\n"
        << "Дата формирования: " << getCurrentTimestamp() << "\n"
        << "----------------------------------------------------\n"
        << "ОБЩАЯ СТАТИСТИКА:\n"
        << "Уникальных позиций (SKU): " << inventory.size() << "\n"
        << "Всего единиц товара: " << totalItemsCount << " шт.\n"
        << "Общая стоимость запасов: " << std::format("{:.2f}", totalCost) << " BYN\n"
        << "----------------------------------------------------\n"
        << "ГРУППИРОВКА ПО КАТЕГОРИЯМ:\n";

    auto stats = getStatsByCategory();
    for (const auto& [category, catStats] : stats) {
        report << std::format("  * {:<15}: {:<5} шт. | Сумма: {:.2f} BYN\n",
            category, catStats.totalQuantity, catStats.totalValue);
    }

    report << "----------------------------------------------------\n"
        << "ДЕТАЛИЗАЦИЯ ПОЗИЦИЙ:\n";

    for (size_t i = 0; i < inventory.size(); ++i) {
        const auto& item = inventory[i];
        report << std::format("{}. [{}] {} {} - {} шт. x {:.2f} BYN = {:.2f} BYN\n",
            i + 1,
            item.device->getType(),
            item.device->getManufacturer(),
            item.device->getModel(),
            item.quantity,
            item.device->getPrice(),
            item.device->getPrice() * item.quantity
        );
        report << "   Характеристики: " << item.device->getExtraSpec() << "\n";
    }

    report << "====================================================\n";

    logAction(std::format(R"(Сформирован текстовый отчет "{}")", filename));
}