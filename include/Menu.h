#pragma once

#include "Warehouse.h"
#include "ElectronicDevice.h"

class Menu {
private:
    Warehouse& warehouse;

    int getMenuChoice() const;

    void printMainMenu() const;
    void handleAddDevice();
    void handleDeleteDevice();
    void handlePrintWarehouse() const;
    void handlePrintDeviceDetails() const;
    void handleEditDevice();
    void handleSortByPrice();
    void handleLoadTestData();

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