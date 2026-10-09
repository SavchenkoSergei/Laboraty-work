#pragma once

#include "WarehouseException.h"

class InvalidDataException : public WarehouseException {
public:
    explicit InvalidDataException(const std::string& message)
        : WarehouseException(std::format("Ошибка данных: {}", message)) {
    }
};