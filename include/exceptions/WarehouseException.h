#pragma once

#include <stdexcept>
#include <string>
#include <format>

class WarehouseException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};