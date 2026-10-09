#pragma once

#include "WarehouseException.h"

class OutOfBoundsException : public WarehouseException {
public:
    explicit OutOfBoundsException(const std::string& message)
        : WarehouseException(std::format("Выход за пределы: {}", message)) {
    }
};