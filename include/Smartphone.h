#pragma once

#include "ElectronicDevice.h"
#include <string>
#include <memory>

class Smartphone : public ElectronicDevice {
private:
    int ramSize;
    std::string osName;

public:
    Smartphone(std::string_view devModel, std::string_view devManufacturer,
        double initialPrice, int initialWarranty,
        int ram, std::string_view os);

    void print(std::ostream& os) const override;
    void read(std::istream& is) override;

    std::unique_ptr<ElectronicDevice> clone() const override;

    std::string getExtraSpec() const override;
    void setExtraSpec(std::string_view spec) override;
};