#include "Warehouse.h"
#include <iostream>
#include <algorithm>

Warehouse::Warehouse(std::string_view name) : warehouseName(name) {}

void Warehouse::addDevice(const ElectronicDevice& device, int quantity) {
    *this += StockItem{ device, quantity };
}

Warehouse& Warehouse::operator+=(const StockItem& newItem) {
    if (newItem.quantity <= 0) {
        std::cout << "Склад \"" << warehouseName << "\": ошибка, количество должно быть больше 0.\n";
        return *this;
    }

    for (auto& item : inventory) {
        if (item.device == newItem.device) {
            item.quantity += newItem.quantity;
            std::cout << "Склад \"" << warehouseName << "\": добавлено " << newItem.quantity
                << " шт. к существующему товару " << newItem.device.getModel() << "\n";
            return *this;
        }
    }
    inventory.push_back(newItem);
    std::cout << "Склад \"" << warehouseName << "\": новый товар \"" << newItem.device.getModel()
        << "\" успешно добавлен в каталог.\n";
    return *this;
}

Warehouse& Warehouse::operator-=(std::string_view model) {
    auto initialSize = inventory.size();

    std::erase_if(inventory, [model](const StockItem& item) {
        return item.device.getModel() == model;
        });

    if (inventory.size() < initialSize) {
        std::cout << "Товар с моделью \"" << model << "\" успешно удален со склада.\n";
    }
    else {
        std::cout << "Ошибка: товар с моделью \"" << model << "\" не найден на складе для удаления.\n";
    }
    return *this;
}


void Warehouse::printWarehouseState() const {
    std::cout << "\n=== Состояние склада: \"" << warehouseName << "\" ===\n";
    if (inventory.empty()) {
        std::cout << "Склад пуст.\n";
        return;
    }

    for (size_t i = 0; i < inventory.size(); ++i) {
        std::cout << i + 1 << ". Остаток: " << inventory[i].quantity << " шт. | ";
        inventory[i].device.printInfo();
    }
    std::cout << "\n";
}

ElectronicDevice* Warehouse::findDeviceByModel(std::string_view model) {
    for (auto& item : inventory) {
        if (item.device.getModel() == model) {
            return &(item.device);
        }
    }
    return nullptr;
}

bool Warehouse::removeDeviceByModel(std::string_view model) {
    auto initialSize = inventory.size();

    std::erase_if(inventory, [model](const StockItem& item) {
        return item.device.getModel() == model;
        });

    return inventory.size() < initialSize;
}

void Warehouse::sortByPrice() {
    if (inventory.empty()) {
        std::cout << "Склад пуст, сортировать нечего.\n";
        return;
    }

    std::ranges::sort(inventory, [](const auto& a, const auto& b) {
        return a.device < b.device;
        });

    std::cout << "Склад \"" << warehouseName << "\" успешно отсортирован по цене (от дешевых к дорогим).\n";
}