#pragma once

#include "WarehouseException.h"

class ObjectNotFoundException : public WarehouseException {
public:
    explicit ObjectNotFoundException(const std::string& message)
        : WarehouseException(std::format("Объект не найден: {}", message)) {
    }
};