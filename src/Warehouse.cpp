#include "Warehouse.h"
#include <iostream>
#include <chrono>
#include <format>

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
    actionHistory.add(std::format("{}Склад \"{}\" был создан.", getCurrentTimestamp(), warehouseName));
}

StockItem* Warehouse::findStockItemByModel(std::string_view model) {
    return inventory.find([model](const StockItem& item) {
        return item.device && item.device->getModel() == model;
        });
}

void Warehouse::clearWarehouse() {
    inventory.clear();
    std::cout << "Склад полностью очищен.\n";
    actionHistory.add(std::format("{}Выполнена очистка склада.", getCurrentTimestamp()));
}

ElectronicDevice* Warehouse::findDeviceByModel(std::string_view model) {
    const auto* item = findStockItemByModel(model);
    return item ? item->device.get() : nullptr;
}

void Warehouse::sortByPrice() {
    inventory.sort([](const StockItem& a, const StockItem& b) {
        if (!a.device || !b.device) return false;
        return a.device->getPrice() < b.device->getPrice();
        });
    actionHistory.add(std::format("{}Выполнена сортировка товаров по цене.", getCurrentTimestamp()));
}

Warehouse& Warehouse::operator+=(StockItem newItem) {
    if (newItem.quantity <= 0) return *this;

    if (auto* existing = inventory.find([&newItem](const StockItem& item) {
        return *(item.device) == *(newItem.device);
        })) {
        existing->quantity += newItem.quantity;
        actionHistory.add(std::format("{}Пополнение: {} (+{} шт.)",
            getCurrentTimestamp(), newItem.device->getModel(), newItem.quantity));
    }
    else {
        const auto modelName = std::string(newItem.device->getModel());
        inventory.add(std::move(newItem));
        actionHistory.add(std::format("{}Добавлен новый товар: {}", getCurrentTimestamp(), modelName));
    }

    return *this;
}

Warehouse& Warehouse::operator-=(std::string_view model) {
    if (inventory.removeIf([model](const StockItem& item) {
        return item.device->getModel() == model;
        })) {
        actionHistory.add(std::format("{}Удален товар по модели: {}", getCurrentTimestamp(), model));
    }

    return *this;
}

void Warehouse::printWarehouseState() const {
    std::cout << std::format("\n=== Состояние склада: \"{}\" ===\n", warehouseName);
    inventory.print();

    size_t activeCount = countMatches(inventory, [](const StockItem& item) {
        return item.quantity > 0 && item.device != nullptr;
        });

    std::cout << "Всего позиций в наличии: " << activeCount << "\n";
}

void Warehouse::showHistory() const {
    std::cout << "\n=== Журнал операций (Логи) ===\n";
    actionHistory.print();
}