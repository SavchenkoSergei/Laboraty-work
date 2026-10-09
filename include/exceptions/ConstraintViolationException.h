#pragma once

#include "WarehouseException.h"

class ConstraintViolationException : public WarehouseException {
public:
    explicit ConstraintViolationException(const std::string& message)
        : WarehouseException(std::format("Нарушение ограничений: {}", message)) {
    }
};