#pragma once

#include <string>
#include <string_view>
#include "StockItem.h"
#include "Collection.h"
#include "Exceptions.h"

class Warehouse {
private:
    std::string warehouseName;
    Collection<StockItem> inventory;       
    Collection<std::string> actionHistory;  

    static std::string getCurrentTimestamp();

public:
    explicit Warehouse(std::string_view name);

    void printWarehouseState() const;
    void showHistory() const;
    void clearWarehouse();
    StockItem* findStockItemByModel(std::string_view model);
    StockItem* getStockItemByCatalogNumber(size_t catalogNumber);
    ElectronicDevice* findDeviceByModel(std::string_view model);
    void increaseStockQuantity(StockItem* item, int amount);
    void reduceStockQuantity(StockItem* item, int amount);
    void sortByPrice();
    Warehouse& operator+=(StockItem newItem);
    Warehouse& operator-=(std::string_view model);
};