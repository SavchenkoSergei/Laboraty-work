#pragma once

#include "WarehouseException.h"

class InvalidOperationException : public WarehouseException {
public:
    explicit InvalidOperationException(const std::string& message)
        : WarehouseException(std::format("Недопустимая операция: {}", message)) {
    }
};