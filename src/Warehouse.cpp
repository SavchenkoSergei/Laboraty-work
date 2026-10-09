#include "Warehouse.h"
#include "WarehouseFileManager.h"
#include "Smartphone.h"
#include "Tablet.h"
#include "Laptop.h"
#include "HomeAppliance.h"
#include <iostream>
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
    if (inventory.empty()) {
        throw InvalidOperationException("Невозможно выполнить поиск: склад пуст.");
    }

    auto count = static_cast<int>(std::count_if(inventory.begin(), inventory.end(), [threshold](const StockItem& item) {
        return item.quantity > threshold; }));

    if (count == 0) {
        throw ObjectNotFoundException(std::format("На складе не найдено товаров с остатком более {} шт.", threshold));
    }

    std::for_each(inventory.begin(), inventory.end(), [threshold](const StockItem& item) {
        if (item.quantity > threshold) {
            std::cout << item << "\n";
        }
        });

    return count;
}

double Warehouse::calculateTotalCost() const {
    return std::accumulate(inventory.begin(), inventory.end(), 0.0, [](double sum, const StockItem& item) {
        return sum + (item.device ? item.device->getPrice() * item.quantity : 0.0); });
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

    std::erase_if(inventory, [model](const StockItem& i) {return i.device && i.device->getModel() == model; });

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
        return item.quantity > 0 && item.device != nullptr; }));

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

    WarehouseFileManager::appendToExternalLog(entry);
}

void Warehouse::restoreState(std::string newName, InventoryContainer newInventory, LogContainer newHistory) {
    warehouseName = std::move(newName);
    inventory = std::move(newInventory);
    actionHistory = std::move(newHistory);
}