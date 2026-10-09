#pragma once

#include "WarehouseException.h"

class DuplicateItemException : public WarehouseException {
public:
    explicit DuplicateItemException(const std::string& message)
        : WarehouseException(std::format("Дубликат объекта: {}", message)) {
    }
};