#include "Smartphone.h"
#include <iostream>

Smartphone::Smartphone(std::string_view devModel, std::string_view devManufacturer,
    double initialPrice, int initialWarranty,
    int ram, std::string_view os)
    : ElectronicDevice("Смартфон", devModel, devManufacturer, initialPrice, initialWarranty),
    ramSizeGB(ram), osName(os) {
}

void Smartphone::print(std::ostream& os) const {
    ElectronicDevice::print(os);
    os << ", ОЗУ: " << ramSizeGB << " ГБ"
        << ", ОС: " << osName;
}

void Smartphone::read(std::istream& is) {
    ElectronicDevice::read(is);

    std::cout << "Введите объем ОЗУ (ГБ): ";
    if (!(is >> ramSizeGB) || ramSizeGB < 0) {
        std::cout << "Ошибка: некорректный объем ОЗУ!\n";
        is.setstate(std::ios::failbit);
        return;
    }
    is.ignore(10000, '\n');

    std::cout << "Введите операционную систему: ";
    std::getline(is, osName);
}

std::unique_ptr<ElectronicDevice> Smartphone::clone() const {
    return std::make_unique<Smartphone>(*this);
}

std::string Smartphone::getExtraSpec() const {
    return std::format("ОЗУ: {} ГБ, ОС: {}", ramSizeGB, osName);
}

void Smartphone::setExtraSpec(std::string_view spec) {
    std::cout << "Текущие доп. характеристики: " << getExtraSpec() << "\n";
    std::cout << "Введите новый объем ОЗУ (ГБ): ";
    int newRam = 0;
    if (std::cin >> newRam && newRam > 0) {
        ramSizeGB = newRam;
    }
    std::cin.ignore(10000, '\n');

    std::cout << "Введите новую ОС: ";
    std::string newOs;
    std::getline(std::cin, newOs);
    if (!newOs.empty()) {
        osName = newOs;
    }
}