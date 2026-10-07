#pragma once
#include <memory>
#include <iostream>
#include "ElectronicDevice.h"

struct StockItem {
    std::unique_ptr<ElectronicDevice> device{ nullptr };
    int quantity{ 0 };

    StockItem() = default;

    StockItem(std::unique_ptr<ElectronicDevice> dev, int qty)
        : device(std::move(dev)), quantity(qty) {
    }

    StockItem(const StockItem&) = delete;
    StockItem& operator=(const StockItem&) = delete;

    StockItem(StockItem&&) noexcept = default;
    StockItem& operator=(StockItem&&) noexcept = default;

    friend std::ostream& operator<<(std::ostream& os, const StockItem& item) {
        if (item.device) {
            os << *item.device << " | Количество: " << item.quantity << " шт.";
        }
        return os;
    }
};