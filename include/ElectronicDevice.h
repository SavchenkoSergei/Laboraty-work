#pragma once
#include <string>
#include <string_view>
#include <iostream>

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
    void print(std::ostream& os) const;
    void read(std::istream& is);

    bool operator==(const ElectronicDevice& other) const;

    std::partial_ordering operator<=>(const ElectronicDevice& other) const {
        return price <=> other.price;
    }

    friend std::ostream& operator<<(std::ostream& os, const ElectronicDevice& device) {
        device.print(os);
        return os;
    }

    friend std::istream& operator>>(std::istream& is, ElectronicDevice& device) {
        device.read(is);
        return is;
    }
};