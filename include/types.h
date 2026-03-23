
#pragma once

 

#include <list>

 

typedef long long OrderId;

typedef int Price;

typedef int Quantity;

 

enum class Side { Buy, Sell };

 

struct Order {

    OrderId id;

    Side side;

    Price price;

    Quantity quantity;

    Quantity filled = 0;

    std::list<Order*>::iterator queue_pos;

 

    Quantity remaining() const { return quantity - filled; }

};

 

struct Trade {

    OrderId bid_id;

    OrderId ask_id;

    Price price;

    Quantity quantity;

};

