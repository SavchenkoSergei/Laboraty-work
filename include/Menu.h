#pragma once
#include "Warehouse.h"
#include "ElectronicDevice.h"

class Menu {
private:
    Warehouse& warehouse;

    void showMenu() const;
    void handleAddDevice();
    void handleEditDeviceProperties();
    void handleGetIndividualCharacteristics();
    void handleRemoveDevice();

    void updateType(ElectronicDevice& device) const;
    void updateModel(ElectronicDevice& device) const;
    void updateManufacturer(ElectronicDevice& device) const;
    void updatePrice(ElectronicDevice& device) const;
    void updateWarranty(ElectronicDevice& device) const;
    void updateExtraSpec(ElectronicDevice& device) const;
    void editDeviceMenu(ElectronicDevice& device) const;
    void printEditMenu(const ElectronicDevice& device) const;

public:
    explicit Menu(Warehouse& wh);
    void run();
};
