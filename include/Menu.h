#pragma once
#include "Warehouse.h"
#include "ElectronicDevice.h"

void handleAddDevice(Warehouse& warehouse);
void handleEditDeviceProperties(Warehouse& warehouse);
void handleGetIndividualCharacteristics(Warehouse& warehouse);
void showMenu();
void handleRemoveDevice(Warehouse& warehouse);