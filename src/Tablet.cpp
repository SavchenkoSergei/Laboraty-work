#include "Tablet.h"
#include <iostream>

Tablet::Tablet(std::string_view devModel, std::string_view devManufacturer,
    double initialPrice, int initialWarranty,
    double screenSize, bool stylus)
    : ElectronicDevice("Планшет", devModel, devManufacturer, initialPrice, initialWarranty),
    screenSizeInches(screenSize), hasStylusSupport(stylus) {
}

void Tablet::print(std::ostream& os) const {
    ElectronicDevice::print(os);
    os << ", Диагональ: " << screenSizeInches << "\""
        << ", Поддержка стилуса: " << (hasStylusSupport ? "Да" : "Нет");
}

void Tablet::read(std::istream& is) {
    ElectronicDevice::read(is);

    std::cout << "Введите диагональ экрана (дюймы): ";
    if (!(is >> screenSizeInches) || screenSizeInches <= 0) {
        std::cout << "Ошибка: некорректная диагональ!\n";
        is.setstate(std::ios::failbit);
        return;
    }

    std::cout << "Поддержка стилуса (1 - Да, 0 - Нет): ";
    int stylusInput = 0;
    if (!(is >> stylusInput)) {
        std::cout << "Ошибка ввода!\n";
        is.setstate(std::ios::failbit);
        return;
    }
    hasStylusSupport = (stylusInput != 0);
    is.ignore(10000, '\n');
}

std::string Tablet::getExtraSpec() const {
    return std::format("Экран: {}\", Стилус: {}", screenSizeInches, hasStylusSupport ? "Да" : "Нет");
}

void Tablet::setExtraSpec(std::string_view spec) {
    std::cout << "Текущие доп. характеристики: " << getExtraSpec() << "\n";
    std::cout << "Введите новую диагональ (дюймы): ";
    if (double newSize = 0.0; std::cin >> newSize && newSize > 0) {
        screenSizeInches = newSize;
    }

    std::cout << "Поддержка стилуса (1 - Да, 0 - Нет): ";
    if (int stylusInput = 0; std::cin >> stylusInput) {
        hasStylusSupport = (stylusInput != 0);
    }
    std::cin.ignore(10000, '\n');
}