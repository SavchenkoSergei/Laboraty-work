#include "Tablet.h"
#include <iostream>

Tablet::Tablet(std::string_view devModel, std::string_view devManufacturer,
    double initialPrice, int initialWarranty,
    double screenSize, bool stylus)
    : ElectronicDevice("Планшет", devModel, devManufacturer, initialPrice, initialWarranty),
    screenSize(screenSize), stylusSupport(stylus) {
}

void Tablet::print(std::ostream& os) const {
    ElectronicDevice::print(os);
    os << ", Диагональ: " << screenSize << "\""
        << ", Поддержка стилуса: " << (stylusSupport ? "Да" : "Нет");
}

void Tablet::read(std::istream& is) {
    ElectronicDevice::read(is);

    std::cout << "Введите диагональ экрана (дюймы): ";
    if (!(is >> screenSize) || screenSize <= 0) {
        is.clear();
        is.ignore(10000, '\n');
        throw InvalidDataException("Некорректная диагональ экрана");
    }

    if (screenSize < 4.0 || screenSize > 20.0) {
        throw ConstraintViolationException(std::format(
            "Диагональ экрана выходит за пределы ограничений категории планшетов (от 4.0 до 20.0 дюймов, введено: {})", screenSize));
    }

    std::cout << "Поддержка стилуса (1 - Да, 0 - Нет): ";
    int stylusInput = 0;
    if (!(is >> stylusInput) || (stylusInput != 0 && stylusInput != 1)) {
        is.clear();
        is.ignore(10000, '\n');
        throw InvalidDataException("Некорректный выбор поддержки стилуса");
    }
    stylusSupport = (stylusInput == 1);
}

void Tablet::setExtraSpec(std::string_view spec) {
    std::cout << "Текущие доп. характеристики: " << getExtraSpec() << "\n";
    std::cout << "Введите новую диагональ (дюймы): ";
    double newSize = 0.0;
    if (!(std::cin >> newSize) || newSize <= 0) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidDataException("Некорректная диагональ экрана");
    }
    screenSize = newSize;

    std::cout << "Поддержка стилуса (1 - Да, 0 - Нет): ";
    int stylusInput = 0;
    if (!(std::cin >> stylusInput) || (stylusInput != 0 && stylusInput != 1)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidDataException("Некорректный выбор поддержки стилуса");
    }
    stylusSupport = (stylusInput == 1);
    std::cin.ignore(10000, '\n');
}

std::string Tablet::getExtraSpec() const {
    return std::format("Экран: {}\", Стилус: {}", screenSize, stylusSupport ? "Да" : "Нет");
}

void Tablet::saveToFile(std::ostream& os) const {
    ElectronicDevice::saveToFile(os);
    os << screenSize << "\n" << stylusSupport << "\n";
}

void Tablet::loadFromFile(std::istream& is) {
    ElectronicDevice::loadFromFile(is);
    if (!(is >> screenSize) || !(is >> stylusSupport)) {
        throw InvalidDataException("Ошибка чтения параметров планшета из файла");
    }
    if (screenSize < 4.0 || screenSize > 20.0) {
        throw ConstraintViolationException(std::format("Некорректная диагональ экрана в файле: {}", screenSize));
    }
}