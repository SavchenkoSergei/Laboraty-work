#pragma once
#include <string>
#include <string_view>
#include <vector>
#include "StockItem.h"

class Warehouse {
private:
    std::string warehouseName;
    std::vector<StockItem> inventory;

public:
    explicit Warehouse(std::string_view name);

    void addDevice(const ElectronicDevice& device, int quantity);
    void printWarehouseState() const;
    ElectronicDevice* findDeviceByModel(std::string_view model);
    bool removeDeviceByModel(std::string_view model);
};