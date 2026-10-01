#include "Laptop.h"
#include <iostream>

Laptop::Laptop(std::string_view devModel, std::string_view devManufacturer,
    double initialPrice, int initialWarranty,
    std::string_view cpu, int battery)
    : ElectronicDevice("Ноутбук", devModel, devManufacturer, initialPrice, initialWarranty),
    cpuModel(cpu), batteryCapacity(battery) {
}

void Laptop::print(std::ostream& os) const {
    ElectronicDevice::print(os);
    os << ", Процессор: " << cpuModel
        << ", Батарея: " << batteryCapacity << " Вт*ч";
}

void Laptop::setExtraSpec(std::string_view spec) {
    std::cout << "Текущие доп. характеристики: " << getExtraSpec() << "\n";
    std::cout << "Введите новую модель процессора: ";
    std::string newCpu;
    std::getline(std::cin >> std::ws, newCpu);
    if (newCpu.empty()) {
        throw InvalidDataException("Название процессора не может быть пустым");
    }
    cpuModel = newCpu;

    std::cout << "Введите новую емкость батареи (Вт*ч): ";
    int newBattery = 0;
    if (!(std::cin >> newBattery) || newBattery <= 0) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw InvalidDataException("Некорректная емкость батареи");
    }
    batteryCapacity = newBattery;
    std::cin.ignore(10000, '\n');
}

std::string Laptop::getExtraSpec() const {
    return std::format("Процессор: {}, Батарея: {} Вт*ч", cpuModel, batteryCapacity);
}

void Laptop::read(std::istream& is) {
    ElectronicDevice::read(is);

    std::cout << "Введите модель процессора: ";
    std::getline(is >> std::ws, cpuModel);

    std::cout << "Введите емкость батареи (Вт*ч): ";
    if (!(is >> batteryCapacity)) {
        is.clear();
        is.ignore(10000, '\n');
        throw InvalidDataException("Некорректный формат ввода: ожидается числовое значение емкости батареи");
    }

    if (batteryCapacity <= 0 || batteryCapacity > 200) {
        throw ConstraintViolationException(std::format(
            "Емкость батареи выходит за пределы допустимых норм (от 1 до 200 Вт*ч, введено: {})",
            batteryCapacity
        ));
    }
}