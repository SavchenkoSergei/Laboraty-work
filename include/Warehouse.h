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

    void printWarehouseState() const;
    ElectronicDevice* findDeviceByModel(std::string_view model);
    StockItem* findStockItemByModel(std::string_view model);
    void sortByPrice();

    Warehouse& operator+=(const StockItem& newItem);
    Warehouse& operator-=(std::string_view model);
};