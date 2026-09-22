#pragma once

#include "ElectronicDevice.h"
#include <memory>

class Tablet : public ElectronicDevice {
private:
    double screenSizeInches;
    bool hasStylusSupport;

public:
    Tablet(std::string_view devModel, std::string_view devManufacturer,
        double initialPrice, int initialWarranty,
        double screenSize, bool stylus);

    void print(std::ostream& os) const override;
    void read(std::istream& is) override;

    std::unique_ptr<ElectronicDevice> clone() const override;

    std::string getExtraSpec() const override;
    void setExtraSpec(std::string_view spec) override;
};