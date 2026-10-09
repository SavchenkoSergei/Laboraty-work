#pragma once

#include "WarehouseException.h"

class BrokenLinkException : public WarehouseException {
public:
    explicit BrokenLinkException(const std::string& message)
        : WarehouseException(std::format("Нарушение связи между объектами: {}", message)) {
    }
};