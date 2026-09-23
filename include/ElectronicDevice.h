#pragma once
#include <string>
#include <string_view>
#include <iostream>
#include <compare>
#include <memory>
#include <format>

class ElectronicDevice {
private:
    std::string typeName;
    std::string manufacturer;
    std::string model;
    double price;
    int warrantyMonths;

public:
    ElectronicDevice(std::string_view type, std::string_view devModel,
        std::string_view devManufacturer, double initialPrice,
        int initialWarranty);
    virtual ~ElectronicDevice() = default;

    virtual std::string getType() const { 
        return typeName; 
    }

    std::string getModel() const;
    std::string getManufacturer() const;
    double getPrice() const;
    int getWarrantyMonths() const;

    void setPrice(double newPrice);
    void setWarrantyMonths(int months);
    void setManufacturer(std::string_view newManufacturer);
    void setModel(std::string_view newModel);

    virtual std::string getExtraSpec() const { return ""; }
    virtual void setExtraSpec(std::string_view spec);

    virtual void print(std::ostream& os) const;
    virtual void read(std::istream& is);

    bool operator==(const ElectronicDevice& other) const;
    auto operator<=>(const ElectronicDevice& other) const {
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