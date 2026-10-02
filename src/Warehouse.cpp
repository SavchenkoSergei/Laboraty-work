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
    if (warehouseName.empty()) {
        throw InvalidDataException("Название склада не может быть пустым.");
    }
    actionHistory.add(std::format("{}Склад \"{}\" был создан.", getCurrentTimestamp(), warehouseName));
}

StockItem* Warehouse::findStockItemByModel(std::string_view model) {
    auto* item = inventory.find([model](const StockItem& item) {
        return item.device && item.device->getModel() == model;
        });
    if (!item) {
        throw ObjectNotFoundException(std::format("Устройство с моделью \"{}\" не найдено на складе", model));
    }
    return item;
}

StockItem* Warehouse::getStockItemByCatalogNumber(size_t catalogNumber) {
    if (catalogNumber == 0 || catalogNumber > inventory.size()) {
        throw OutOfBoundsException(std::format(
            "Позиция {} отсутствует в каталоге (всего элементов: {})",catalogNumber,inventory.size()));
    }
    return &inventory.getAt(catalogNumber - 1);
}

void Warehouse::clearWarehouse() {
    if (inventory.empty()) {
        throw InvalidOperationException("Невозможно очистить склад: он уже пуст.");
    }

    inventory.clear();
    actionHistory.add(std::format("{}Выполнена очистка склада.", getCurrentTimestamp()));
}

ElectronicDevice* Warehouse::findDeviceByModel(std::string_view model) {
    const auto* item = findStockItemByModel(model);
    return item ? item->device.get() : nullptr;
}

void Warehouse::sortByPrice() {
    if (inventory.empty()) {
        throw InvalidOperationException("Невозможно отсортировать склад: каталог товаров пуст.");
    }

    inventory.sort([](const StockItem& a, const StockItem& b) {
        if (!a.device || !b.device) return false;
        return a.device->getPrice() < b.device->getPrice();
        });

    actionHistory.add(std::format("{}Выполнена сортировка товаров по цене.", getCurrentTimestamp()));
}

void Warehouse::increaseStockQuantity(StockItem* item, int amount) {
    if (amount <= 0) {
        throw InvalidDataException(std::format("Количество прихода должно быть больше нуля (введено: {})", amount));
    }

    item->quantity += amount;
    actionHistory.add(std::format("{}Пополнен остаток товара \"{}\" на {} шт. (Текущий остаток: {} шт.)",
        getCurrentTimestamp(),item->device->getModel(),amount,item->quantity));
}

void Warehouse::reduceStockQuantity(StockItem* item, int amount) {
    if (!item || !item->device) {
        throw BrokenLinkException("Выбранная позиция не содержит устройства!");
    }

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
    actionHistory.add(std::format("{}Списано {} шт. товара \"{}\"", getCurrentTimestamp(), amount, modelName));

    if (item->quantity == 0) {
        actionHistory.add(std::format("{}Остаток товара \"{}\" достиг 0 шт.", getCurrentTimestamp(), modelName));
    }
}

Warehouse& Warehouse::operator+=(StockItem newItem) {
    if (!newItem.device) {
        throw BrokenLinkException("попытка добавить запись на склад без инициализированного устройства (nullptr)");
    }
    if (newItem.quantity <= 0) {
        throw ConstraintViolationException(std::format("количество добавляемого товара должно быть больше нуля (передано: {})",newItem.quantity));
    }

    if (inventory.find([&newItem](const StockItem& item) { return *(item.device) == *(newItem.device); }) != nullptr) {
        throw DuplicateItemException(std::format("Модель \"{}\" уже существует на складе. Используйте редактирование.", newItem.device->getModel()));
    }

    const auto modelName = std::string(newItem.device->getModel());
    inventory.add(std::move(newItem));
    actionHistory.add(std::format("{}Добавлен новый товар: {}", getCurrentTimestamp(), modelName));

    return *this;
}

Warehouse& Warehouse::operator-=(std::string_view model) {
    if (StockItem* foundItem = findStockItemByModel(model); foundItem->quantity > 0) {
        throw BrokenLinkException(std::format(
            "Нельзя удалить позицию \"{}\", к ней привязаны товары на складе (в наличии: {} шт.). Сначала спишите остатки!",
            std::string(model), foundItem->quantity
        ));
    }

    inventory.removeIf([model](const StockItem& i) {
        return i.device && i.device->getModel() == model;
        });

    actionHistory.add(std::format("{}Удален товар по модели: {}", getCurrentTimestamp(), std::string(model)));
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