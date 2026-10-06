#include "Smartphone.h"
#include <iostream>

Smartphone::Smartphone(std::string_view devModel, std::string_view devManufacturer,
    double initialPrice, int initialWarranty,
    int ram, std::string_view os)
    : ElectronicDevice("Смартфон", devModel, devManufacturer, initialPrice, initialWarranty),
    ramSize(ram), osName(os) {
}

void Smartphone::print(std::ostream& os) const {
    ElectronicDevice::print(os);
    os << ", ОЗУ: " << ramSize << " ГБ"
        << ", ОС: " << osName;
}

void Smartphone::read(std::istream& is) {
    ElectronicDevice::read(is);

    std::cout << "Введите объем ОЗУ (ГБ): ";
    if (!(is >> ramSize) || ramSize <= 0) {
        is.clear();
        is.ignore(10000, '\n');
        throw InvalidDataException("Некорректный объем ОЗУ");
    }

    if (ramSize <= 0 || ramSize > 128) {
        throw ConstraintViolationException(std::format(
            "Объем ОЗУ выходит за границы допустимых технологических ограничений (от 1 до 128 ГБ, введено: {})", ramSize));
    }

    std::cout << "Введите ОС: ";
    std::getline(is >> std::ws, osName);
    if (osName.empty()) {
        throw InvalidDataException("Название ОС не может быть пустым");
    }
}

void Smartphone::setExtraSpec(std::string_view spec) {
    std::cout << "Текущие доп. характеристики: " << getExtraSpec() << "\n";
    std::cout << "Введите новый объем ОЗУ (ГБ): ";
    int newRam = 0;
    if (!(std::cin >> newRam) || newRam <= 0) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidDataException("Некорректный объем ОЗУ");
    }
    ramSize = newRam;
    std::cin.ignore(10000, '\n');

    std::cout << "Введите новую ОС: ";
    std::string newOs;
    std::getline(std::cin >> std::ws, newOs);
    if (newOs.empty()) {
        throw InvalidDataException("Название ОС не может быть пустым");
    }
    osName = newOs;
}

std::string Smartphone::getExtraSpec() const {
    return std::format("ОЗУ: {} ГБ, ОС: {}", ramSize, osName);
}

void Smartphone::saveToFile(std::ostream& os) const {
    ElectronicDevice::saveToFile(os);
    os << ramSize << "\n" << osName << "\n";
}

void Smartphone::loadFromFile(std::istream& is) {
    ElectronicDevice::loadFromFile(is);
    if (!(is >> ramSize) || !std::getline(is >> std::ws, osName)) {
        throw InvalidDataException("Ошибка чтения параметров смартфона из файла");
    }
    if (ramSize <= 0 || ramSize > 128) {
        throw ConstraintViolationException(std::format("Некорректный объем ОЗУ в файле: {}", ramSize));
    }
}