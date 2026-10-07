#pragma once

#include <vector>
#include <list>
#include <map>
#include <string>
#include "StockItem.h"

struct CategoryStats {
    int totalQuantity{ 0 };
    double totalValue{ 0.0 };
};

using InventoryContainer = std::vector<StockItem>;      
using LogContainer = std::list<std::string>;           
using CategoryMap = std::map<std::string, CategoryStats, std::less<>>;