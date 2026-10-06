#pragma once

#include <string>
#include <string_view>
#include "StockItem.h"
#include "Collection.h"
#include "Exceptions.h"
#include "Smartphone.h"
#include "Tablet.h"
#include "Laptop.h"
#include "HomeAppliance.h"

class Warehouse {
private:
    std::string warehouseName;
    Collection<StockItem> inventory;       
    mutable Collection<std::string> actionHistory;

    static std::string getCurrentTimestamp();

public:
    explicit Warehouse(std::string_view name);

    void printWarehouseState() const;
    void showHistory() const;
    void clearWarehouse();
    StockItem* findStockItemByModel(std::string_view model);
    StockItem* getStockItemByCatalogNumber(size_t catalogNumber);
    ElectronicDevice* findDeviceByModel(std::string_view model);
    void increaseStockQuantity(StockItem* item, int amount) const;
    void reduceStockQuantity(StockItem* item, int amount) const;
    void sortByPrice();
    void logAction(std::string_view message) const;

    void saveStateToFile(const std::string& filename = "warehouse_data.txt") const;
    void loadStateFromFile(const std::string& filename = "warehouse_data.txt");
    void appendToExternalLog(const std::string& message) const;
    void generateReport(const std::string& filename = "report.txt") const;

    Warehouse& operator+=(StockItem newItem);
    Warehouse& operator-=(std::string_view model);
};