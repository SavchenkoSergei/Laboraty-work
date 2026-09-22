#pragma once

#include "ElectronicDevice.h"
#include <string>
#include <memory>

class Laptop : public ElectronicDevice {
private:
    std::string cpuModel;
    int batteryCapacityWh;

public:
    Laptop(std::string_view devModel, std::string_view devManufacturer,
        double initialPrice, int initialWarranty,
        std::string_view cpu, int battery);

    void print(std::ostream& os) const override;
    void read(std::istream& is) override;

    std::unique_ptr<ElectronicDevice> clone() const override;

    std::string getExtraSpec() const override;
    void setExtraSpec(std::string_view spec) override;
};