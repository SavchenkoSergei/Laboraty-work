#include "Laptop.h"
#include <iostream>

Laptop::Laptop(std::string_view devModel, std::string_view devManufacturer,
    double initialPrice, int initialWarranty,
    std::string_view cpu, int battery)
    : ElectronicDevice("Ноутбук", devModel, devManufacturer, initialPrice, initialWarranty),
    cpuModel(cpu), batteryCapacityWh(battery) {
}

void Laptop::print(std::ostream& os) const {
    ElectronicDevice::print(os);
    os << ", Процессор: " << cpuModel
        << ", Батарея: " << batteryCapacityWh << " Вт*ч";
}

void Laptop::read(std::istream& is) {
    ElectronicDevice::read(is);

    std::cout << "Введите модель процессора: ";
    std::getline(is, cpuModel);

    std::cout << "Введите емкость батареи (Вт*ч): ";
    if (!(is >> batteryCapacityWh) || batteryCapacityWh < 0) {
        std::cout << "Ошибка: некорректная емкость батареи!\n";
        is.setstate(std::ios::failbit);
        return;
    }
    is.ignore(10000, '\n');
}

std::unique_ptr<ElectronicDevice> Laptop::clone() const {
    return std::make_unique<Laptop>(*this);
}

std::string Laptop::getExtraSpec() const {
    return std::format("Процессор: {}, Батарея: {} Вт*ч", cpuModel, batteryCapacityWh);
}

void Laptop::setExtraSpec(std::string_view spec) {
    std::cout << "Текущие доп. характеристики: " << getExtraSpec() << "\n";
    std::cout << "Введите новую модель процессора: ";
    std::string newCpu;
    std::getline(std::cin, newCpu);
    if (!newCpu.empty()) {
        cpuModel = newCpu;
    }

    std::cout << "Введите новую емкость батареи (Вт*ч): ";
    int newBattery = 0;
    if (std::cin >> newBattery && newBattery > 0) {
        batteryCapacityWh = newBattery;
    }
    std::cin.ignore(10000, '\n');
}