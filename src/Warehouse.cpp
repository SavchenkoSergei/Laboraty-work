#include "Warehouse.h"
#include <iostream>

Warehouse::Warehouse(std::string_view name) : warehouseName(name) {}

void Warehouse::addDevice(const ElectronicDevice& device, int quantity) {
    for (auto& item : inventory) {
        if (item.device.getModel() == device.getModel() &&
            item.device.getManufacturer() == device.getManufacturer()) {
            item.quantity += quantity;
            std::cout << "Склад \"" << warehouseName << "\": добавлено " << quantity
                << " шт. к существующему товару " << device.getModel() << "\n";
            return;
        }
    }
    StockItem newItem{ device, quantity };
    inventory.push_back(newItem);
    std::cout << "Склад \"" << warehouseName << "\": новый товар \"" << device.getModel()
        << "\" успешно добавлен в каталог.\n";
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
    auto it = std::remove_if(inventory.begin(), inventory.end(),
        [model](const StockItem& item) {
            return item.device.getModel() == model;
        });
    if (it == inventory.end()) {
        return false;
    }
    inventory.erase(it, inventory.end());
    return true;
}