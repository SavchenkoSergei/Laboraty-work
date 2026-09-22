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

    std::cout << "Введите класс энергоэффективности (например, A++): ";
    std::getline(is, energyClass);

    std::cout << "Введите потребляемую мощность (Вт): ";
    if (!(is >> powerWatts) || powerWatts < 0) {
        std::cout << "Ошибка: некорректная мощность!\n";
        is.setstate(std::ios::failbit);
        return;
    }
    is.ignore(10000, '\n');
}

std::unique_ptr<ElectronicDevice> HomeAppliance::clone() const {
    return std::make_unique<HomeAppliance>(*this);
}

std::string HomeAppliance::getExtraSpec() const {
    return std::format("Энергокласс: {}, Мощность: {} Вт", energyClass, powerWatts);
}

void HomeAppliance::setExtraSpec(std::string_view spec) {
    std::cout << "Текущие доп. характеристики: " << getExtraSpec() << "\n";
    std::cout << "Введите новый класс энергоэффективности: ";
    std::string newEnergy;
    std::getline(std::cin, newEnergy);
    if (!newEnergy.empty()) {
        energyClass = newEnergy;
    }

    std::cout << "Введите новую мощность (Вт): ";
    if (int newPower = 0; std::cin >> newPower && newPower > 0) {
        powerWatts = newPower;
    }
    std::cin.ignore(10000, '\n');
}