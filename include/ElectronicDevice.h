#pragma once
#include <string>
#include <string_view>

class ElectronicDevice {
private:
    std::string type;
    std::string model;
    std::string manufacturer;
    double price;
    int warrantyMonths;
    std::string extraSpec;

public:
    ElectronicDevice(std::string_view devType, std::string_view devModel,
        std::string_view devManufacturer, double initialPrice,
        int initialWarranty, std::string_view initialExtra);

    std::string getType() const;
    std::string getModel() const;
    std::string getManufacturer() const;
    double getPrice() const;
    int getWarrantyMonths() const;
    std::string getExtraSpec() const;

    void setType(std::string_view newType);
    void setModel(std::string_view newModel);
    void setManufacturer(std::string_view newManufacturer);
    void setPrice(double newPrice);
    void setWarrantyMonths(int months);
    void setExtraSpec(std::string_view newExtra);

    void printInfo() const;
};