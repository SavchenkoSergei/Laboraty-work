#pragma once

#include <stdexcept>
#include <string>
#include <format>

class WarehouseException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class InvalidDataException : public WarehouseException {
public:
    explicit InvalidDataException(const std::string& message)
        : WarehouseException(std::format("Ошибка данных: {}", message)) {
    }
};

class ObjectNotFoundException : public WarehouseException {
public:
    explicit ObjectNotFoundException(const std::string& message)
        : WarehouseException(std::format("Объект не найден: {}", message)) {
    }
};

class DuplicateItemException : public WarehouseException {
public:
    explicit DuplicateItemException(const std::string& message)
        : WarehouseException(std::format("Дубликат объекта: {}", message)) {
    }
};

class ConstraintViolationException : public WarehouseException {
public:
    explicit ConstraintViolationException(const std::string& message)
        : WarehouseException(std::format("Нарушение ограничений: {}", message)) {
    }
};

class OutOfBoundsException : public WarehouseException {
public:
    explicit OutOfBoundsException(const std::string& message)
        : WarehouseException(std::format("Выход за пределы: {}", message)) {
    }
};

class InvalidOperationException : public WarehouseException {
public:
    explicit InvalidOperationException(const std::string& message)
        : WarehouseException(std::format("Недопустимая операция: {}", message)) {
    }
};

class BrokenLinkException : public WarehouseException {
public:
    explicit BrokenLinkException(const std::string& message)
        : WarehouseException(std::format("Нарушение связи между объектами: {}", message)) {
    }
};