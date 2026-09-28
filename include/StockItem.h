#pragma once
#include <memory>
#include "ElectronicDevice.h"

struct StockItem {
    std::unique_ptr<ElectronicDevice> device;
    int quantity;

    friend std::ostream& operator<<(std::ostream& os, const StockItem& item) {
        if (item.device) {
            os << *item.device << " | Количество: " << item.quantity << " шт.";
        }
        return os;
    }
};