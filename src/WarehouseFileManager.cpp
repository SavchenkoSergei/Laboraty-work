#include "WarehouseFileManager.h"
#include <fstream>
#include <format>
#include <numeric>
#include "Smartphone.h"
#include "Tablet.h"
#include "Laptop.h"
#include "HomeAppliance.h"

void WarehouseFileManager::appendToExternalLog(std::string_view message) {
    std::ofstream logFile("journal.log", std::ios::app);
    if (!logFile.is_open()) {
        throw InvalidOperationException("Не удалось открыть файл журнала journal.log для записи");
    }
    logFile << message << "\n";
}

void WarehouseFileManager::saveStateToFile(const Warehouse& warehouse, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw InvalidOperationException(std::format(R"(Не удалось открыть файл "{}" для записи состояния)", filename));
    }

    file << warehouse.getWarehouseName() << "\n";
    const auto& inventory = warehouse.getInventory();
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

    const auto& history = warehouse.getActionHistory();
    file << history.size() << "\n";
    for (const auto& logEntry : history) {
        file << logEntry << "\n";
    }

    if (file.fail()) {
        throw InvalidOperationException("Ошибка во время записи данных в файл");
    }

    warehouse.logAction(std::format(R"(Состояние склада успешно сохранено в файл "{}")", filename));
}

void WarehouseFileManager::loadStateFromFile(Warehouse& warehouse, const std::string& filename) {
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

    warehouse.restoreState(std::move(newName), std::move(tempInventory), std::move(tempHistory));
    warehouse.logAction(std::format(R"(Состояние склада успешно загружено из файла "{}")", filename));
}

void WarehouseFileManager::generateReport(const Warehouse& warehouse, const std::string& filename) {
    std::ofstream report(filename);
    if (!report.is_open()) {
        throw InvalidOperationException(std::format("Не удалось создать файл отчета \"{}\"", filename));
    }

    const auto& inventory = warehouse.getInventory();
    double totalCost = warehouse.calculateTotalCost();
    int totalItemsCount = std::accumulate(inventory.begin(), inventory.end(), 0, [](int sum, const StockItem& item) {
        return sum + item.quantity;
        });

    report << "====================================================\n"
        << "          ОТЧЕТ ПО СОСТОЯНИЮ СКЛАДА                 \n"
        << "====================================================\n"
        << "Название склада: " << warehouse.getWarehouseName() << "\n"
        << "----------------------------------------------------\n"
        << "ОБЩАЯ СТАТИСТИКА:\n"
        << "Уникальных позиций (SKU): " << inventory.size() << "\n"
        << "Всего единиц товара: " << totalItemsCount << " шт.\n"
        << "Общая стоимость запасов: " << std::format("{:.2f}", totalCost) << " BYN\n"
        << "----------------------------------------------------\n"
        << "ГРУППИРОВКА ПО КАТЕГОРИЯМ:\n";

    auto stats = warehouse.getStatsByCategory();
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

    warehouse.logAction(std::format(R"(Сформирован текстовый отчет "{}")", filename));
}