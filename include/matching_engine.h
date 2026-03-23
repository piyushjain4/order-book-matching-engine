
#pragma once

 

#include "order_book.h"

#include <string>

#include <unordered_map>

#include <vector>

 

class MatchingEngine {

public:

    std::vector<Trade> submit(const std::string& symbol, Side side,

                              Price price, Quantity qty);

    bool cancel(const std::string& symbol, OrderId id);

 

    OrderBook& book(const std::string& symbol);

 

private:

    std::unordered_map<std::string, OrderBook> books_;

};

