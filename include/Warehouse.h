#pragma once

#include <string>
#include <string_view>
#include "StockItem.h"
#include "CategoryStats.h"
#include "AllExceptions.h"

class Warehouse {
private:
    std::string warehouseName;
    InventoryContainer inventory;
    mutable LogContainer actionHistory;

    static std::string getCurrentTimestamp();

public:
    explicit Warehouse(std::string_view name);

    void printWarehouseState() const;
    void showHistory() const;
    void clearWarehouse();

    StockItem* findStockItemByModel(std::string_view model);
    StockItem* getStockItemByCatalogNumber(size_t catalogNumber);
    ElectronicDevice* findDeviceByModel(std::string_view model);
    std::vector<StockItem> findByPriceRange(double minPrice, double maxPrice) const;

    void increaseStockQuantity(StockItem* item, int amount) const;
    void reduceStockQuantity(StockItem* item, int amount) const;

    void sortByPrice(bool ascending = true);
    void sortByName(bool ascending = true);

    const StockItem& getMostExpensiveItem() const;
    const StockItem& getCheapestItem() const;
    int countItemsMoreThan(int threshold) const;
    double calculateTotalCost() const;
    CategoryMap getStatsByCategory() const;

    void logAction(std::string_view message) const;

    void restoreState(std::string newName, InventoryContainer newInventory, LogContainer newHistory);

    Warehouse& operator+=(StockItem newItem);
    Warehouse& operator-=(std::string_view model);

    const std::string& getWarehouseName() const { return warehouseName; }
    const InventoryContainer& getInventory() const { return inventory; }
    const LogContainer& getActionHistory() const { return actionHistory; }
};