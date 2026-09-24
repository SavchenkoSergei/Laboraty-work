#include "ElectronicDevice.h"

ElectronicDevice::ElectronicDevice(std::string_view type, std::string_view devModel,
    std::string_view devManufacturer, double initialPrice,
    int initialWarranty)
    : typeName(type), manufacturer(devManufacturer), model(devModel),
    price(initialPrice), warrantyMonths(initialWarranty) {
}

std::string ElectronicDevice::getModel() const { return model; }
std::string ElectronicDevice::getManufacturer() const { return manufacturer; }
double ElectronicDevice::getPrice() const { return price; }
int ElectronicDevice::getWarrantyMonths() const { return warrantyMonths; }

void ElectronicDevice::setModel(std::string_view newModel) { model = newModel; }
void ElectronicDevice::setManufacturer(std::string_view newManufacturer) { manufacturer = newManufacturer; }

void ElectronicDevice::setPrice(double newPrice) {
    if (newPrice < 0) {
        std::cout << "Отказ: цена не может быть отрицательной!\n";
        return;
    }
    price = newPrice;
}

void ElectronicDevice::setWarrantyMonths(int months) {
    if (months < 0) {
        std::cout << "Отказ: гарантия не может быть отрицательной!\n";
        return;
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
    std::getline(is, model);
    std::cout << "Введите производителя: ";
    std::getline(is, manufacturer);

    std::cout << "Введите цену (BYN): ";
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
}