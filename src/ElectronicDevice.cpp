#include "ElectronicDevice.h"

ElectronicDevice::ElectronicDevice(std::string_view devType, std::string_view devModel,
    std::string_view devManufacturer, double initialPrice,
    int initialWarranty, std::string_view initialExtra)
    : type(devType), model(devModel), manufacturer(devManufacturer),
    price(initialPrice), warrantyMonths(initialWarranty), extraSpec(initialExtra) {
}

std::string ElectronicDevice::getType() const {
    return type;
}

std::string ElectronicDevice::getModel() const {
    return model;
}

std::string ElectronicDevice::getManufacturer() const {
    return manufacturer;
}

double ElectronicDevice::getPrice() const {
    return price;
}

int ElectronicDevice::getWarrantyMonths() const {
    return warrantyMonths;
}

std::string ElectronicDevice::getExtraSpec() const {
    return extraSpec;
}

void ElectronicDevice::setType(std::string_view newType) {
    type = newType;
}

void ElectronicDevice::setModel(std::string_view newModel) {
    model = newModel;
}

void ElectronicDevice::setManufacturer(std::string_view newManufacturer) {
    manufacturer = newManufacturer;
}

void ElectronicDevice::setPrice(double newPrice) {
    if (newPrice < 0) {
        std::cout << "Отказ операции: цена при изменении не может быть отрицательной!\n";
        return;
    }
    price = newPrice;
}

void ElectronicDevice::setWarrantyMonths(int months) {
    if (months < 0) {
        std::cout << "Отказ операции: гарантийный срок при изменении не может быть отрицательным!\n";
        return;
    }
    warrantyMonths = months;
}

void ElectronicDevice::setExtraSpec(std::string_view newExtra) {
    extraSpec = newExtra;
}

bool ElectronicDevice::operator==(const ElectronicDevice& other) const {
    return (model == other.model) && (manufacturer == other.manufacturer);
}

void ElectronicDevice::print(std::ostream& os) const {
    os << "[" << type << "] "
        << "Производитель: " << manufacturer
        << ", Модель: " << model
        << ", Цена: " << price << " руб."
        << ", Гарантия: " << warrantyMonths << " мес."
        << ", Особенность: " << extraSpec;
}

void ElectronicDevice::read(std::istream& is) {
    std::cout << "Введите тип товара: ";
    std::getline(is, type);

    std::cout << "Введите модель: ";
    std::getline(is, model);

    std::cout << "Введите производителя: ";
    std::getline(is, manufacturer);

    std::cout << "Введите цену (руб.): ";
    if (!(is >> price) || price < 0) {
        std::cout << "Ошибка: некорректная цена!\n";
        is.setstate(std::ios::failbit);
        return;
    }

    std::cout << "Введите гарантию (мес.): ";
    if (!(is >> warrantyMonths) || warrantyMonths < 0) {
        std::cout << "Ошибка: некорректный срок гарантии!\n";
        is.setstate(std::ios::failbit);
        return;
    }
    is.ignore(10000, '\n');

    std::cout << "Введите доп. характеристики: ";
    std::getline(is, extraSpec);
}