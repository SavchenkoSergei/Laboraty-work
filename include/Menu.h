#pragma once

#include "Exceptions.h"
#include "Warehouse.h"
#include "Collection.h"
#include "ElectronicDevice.h"
#include "Smartphone.h"
#include "Tablet.h"
#include "Laptop.h"
#include "HomeAppliance.h"
#include <memory>

class Menu {
private:
    Warehouse& warehouse;

    int getMenuChoice() const;
    void printMainMenu() const;

    void handleAddDevice();
    void handleDeleteDevice();
    void handleReduceStock();
    void handlePrintWarehouse() const;
    void handlePrintDeviceDetails() const;
    void handleEditDevice() const;
    void handleSortByPrice();
    void handleLoadTestData();
    void handleClearWarehouse();
    void handleException(std::exception_ptr eptr) const;

    StockItem* selectStockItem() const;
    void printAddDeviceMenu() const;
    void printEditMenu(const ElectronicDevice& device) const;
    void editDeviceMenu(ElectronicDevice& device) const;

    void editPrice(ElectronicDevice& device) const;
    void editWarranty(ElectronicDevice& device) const;
    void editManufacturer(ElectronicDevice& device) const;
    void editModel(ElectronicDevice& device) const;
    void editExtraSpec(ElectronicDevice& device) const;

public:
    explicit Menu(Warehouse& wh);

    void run();
};