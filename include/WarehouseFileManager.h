#pragma once

#include <string>
#include <string_view>
#include "Warehouse.h"
#include "AllExceptions.h"

class WarehouseFileManager {
public:
    static void appendToExternalLog(std::string_view message);

    static void saveStateToFile(const Warehouse& warehouse, const std::string& filename = "warehouse_data.txt");
    static void loadStateFromFile(Warehouse& warehouse, const std::string& filename = "warehouse_data.txt");

    static void generateReport(const Warehouse& warehouse, const std::string& filename = "report.txt");
};