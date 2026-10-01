#include "HomeAppliance.h"
#include <iostream>

HomeAppliance::HomeAppliance(std::string_view devModel, std::string_view devManufacturer,
    double initialPrice, int initialWarranty,
    std::string_view energy, int power)
    : ElectronicDevice("Бытовая техника", devModel, devManufacturer, initialPrice, initialWarranty),
    energyClass(energy), powerWatts(power) {
}

void HomeAppliance::print(std::ostream& os) const {
    ElectronicDevice::print(os);
    os << ", Класс энергоэффективности: " << energyClass
        << ", Мощность: " << powerWatts << " Вт";
}

void HomeAppliance::read(std::istream& is) {
    ElectronicDevice::read(is);

    std::cout << "Введите класс энергопотребления (например, A++): ";
    std::getline(is >> std::ws, energyClass);
    if (energyClass.empty()) {
        throw InvalidDataException("Класс энергопотребления не может быть пустым");
    }

    std::cout << "Введите потребляемую мощность (Вт): ";
    if (!(is >> powerWatts) || powerWatts <= 0) {
        is.clear();
        is.ignore(10000, '\n');
        throw InvalidDataException("Некорректная мощность");
    }
}

std::string HomeAppliance::getExtraSpec() const {
    return std::format("Энергокласс: {}, Мощность: {} Вт", energyClass, powerWatts);
}

void HomeAppliance::setExtraSpec(std::string_view spec) {
    std::cout << "Текущие доп. характеристики: " << getExtraSpec() << "\n";
    std::cout << "Введите новый класс энергоэффективности: ";
    std::string newEnergy;
    std::getline(std::cin >> std::ws, newEnergy);
    if (newEnergy.empty()) {
        throw InvalidDataException("Класс энергоэффективности не может быть пустым");
    }
    energyClass = newEnergy;

    std::cout << "Введите новую мощность (Вт): ";
    int newPower = 0;
    if (!(std::cin >> newPower) || newPower <= 0) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidDataException("Некорректная мощность");
    }
    powerWatts = newPower;
    std::cin.ignore(10000, '\n');
}