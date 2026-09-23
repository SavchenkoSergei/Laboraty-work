#pragma once
#include "ElectronicDevice.h"
#include <string>
#include <memory>

class HomeAppliance : public ElectronicDevice {
private:
    std::string energyClass;
    int powerWatts;

public:
    HomeAppliance(std::string_view devModel, std::string_view devManufacturer,
        double initialPrice, int initialWarranty,
        std::string_view energy, int power);

    void print(std::ostream& os) const override;
    void read(std::istream& is) override;

    std::string getExtraSpec() const override;
    void setExtraSpec(std::string_view spec) override;
};