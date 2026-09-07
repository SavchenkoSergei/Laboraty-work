#include "ElectronicDevice.h"
#include <iostream>

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
   price = newPrice; 
}
void ElectronicDevice::setWarrantyMonths(int months) { 
    warrantyMonths = months; 
}
void ElectronicDevice::setExtraSpec(std::string_view newExtra) { 
    extraSpec = newExtra; 
}

void ElectronicDevice::printInfo() const {
    std::cout << "[" << type << "] "
        << "Производитель: " << manufacturer
        << ", Модель: " << model
        << ", Цена: " << price << " руб."
        << ", Гарантия: " << warrantyMonths << " мес."
        << ", Особенность: " << extraSpec << "\n";
}