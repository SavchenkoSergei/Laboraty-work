#pragma once
#include <memory>
#include "ElectronicDevice.h"

struct StockItem {
    std::unique_ptr<ElectronicDevice> device;
    int quantity;
};