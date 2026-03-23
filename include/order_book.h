
#pragma once

 

#include "types.h"

#include <list>

#include <unordered_map>

#include <vector>

 

#define MAX_PRICE 100001

#define BITMAP_WORDS ((MAX_PRICE + 63) / 64)

 

class OrderBook {

public:

 

    std::vector<Trade> addOrder(Side side, Price price, Quantity qty);

    bool cancelOrder(OrderId id);

 

    int bestBid() const;

    int bestAsk() const;

 

    typedef std::pair<Price, Quantity> LevelInfo;

    std::vector<LevelInfo> bidDepth(int levels) const;

    std::vector<LevelInfo> askDepth(int levels) const;

 

private:

    OrderId next_id_ = 1;

 

    unsigned long long bid_bitmap_[BITMAP_WORDS] = {};

    unsigned long long ask_bitmap_[BITMAP_WORDS] = {};

 

    std::list<Order*> bid_levels_[MAX_PRICE];

    std::list<Order*> ask_levels_[MAX_PRICE];

 

    std::unordered_map<OrderId, Order*> orders_;

 

    void setBit(unsigned long long* bitmap, int p);

    void clearBit(unsigned long long* bitmap, int p);

 

    Quantity levelQty(const std::list<Order*>& level) const;

 

    std::vector<Trade> matchBuy(Order* order);

    std::vector<Trade> matchSell(Order* order);

};

