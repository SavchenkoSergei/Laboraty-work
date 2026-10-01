#include "ElectronicDevice.h"

ElectronicDevice::ElectronicDevice(std::string_view type, std::string_view devModel,
    std::string_view devManufacturer, double initialPrice,
    int initialWarranty)
    : typeName(type), manufacturer(devManufacturer), model(devModel),
    price(initialPrice), warrantyMonths(initialWarranty) {
    if (initialPrice < 0) {
        throw InvalidDataException("Начальная цена не может быть отрицательной");
    }
    if (initialWarranty < 0) {
        throw InvalidDataException("Начальный срок гарантии не может быть отрицательным");
    }
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

void ElectronicDevice::setModel(std::string_view newModel) {
    model = newModel; 
}
void ElectronicDevice::setManufacturer(std::string_view newManufacturer) { 
    manufacturer = newManufacturer; 
}

void ElectronicDevice::setPrice(double newPrice) {
    if (newPrice < 0) {
        throw InvalidDataException(std::format("Цена не может быть отрицательной ({})", newPrice));
    }
    price = newPrice;
}

void ElectronicDevice::setWarrantyMonths(int months) {
    if (months < 0) {
        throw InvalidDataException(std::format("Срок гарантии не может быть отрицательным ({})", months));
    }
    warrantyMonths = months;
}

bool ElectronicDevice::operator==(const ElectronicDevice& other) const {
    return (model == other.model) && (manufacturer == other.manufacturer);
}

void ElectronicDevice::print(std::ostream& os) const {
    os << "[" << typeName << "] Производитель: " << manufacturer
        << ", Модель: " << model
        << ", Цена: " << price << " BYN"
        << ", Гарантия: " << warrantyMonths << " мес.";
}

void ElectronicDevice::read(std::istream& is) {
    std::cout << "Введите модель: ";
    std::getline(is >> std::ws, model);
    if (model.empty()) {
        throw InvalidDataException("Название модели не может быть пустым");
    }

    std::cout << "Введите производителя: ";
    std::getline(is >> std::ws, manufacturer);
    if (manufacturer.empty()) {
        throw InvalidDataException("Название производителя не может быть пустым");
    }

    std::cout << "Введите цену (BYN): ";
    if (!(is >> price) || price < 0) {
        is.clear();
        is.ignore(10000, '\n');
        throw InvalidDataException("Введена некорректная цена");
    }

    std::cout << "Введите гарантию (мес.): ";
    if (!(is >> warrantyMonths) || warrantyMonths < 0) {
        is.clear();
        is.ignore(10000, '\n');
        throw InvalidDataException("Введен некорректный срок гарантии");
    }
}