
#include "matching_engine.h"

using namespace std;

 

vector<Trade> MatchingEngine::submit(const string& symbol, Side side,

                                     Price price, Quantity qty) {

    return books_[symbol].addOrder(side, price, qty);

}

 

bool MatchingEngine::cancel(const string& symbol, OrderId id) {

    auto it = books_.find(symbol);

    if (it == books_.end())

        return false;

    return it->second.cancelOrder(id);

}

 

OrderBook& MatchingEngine::book(const string& symbol) {

    return books_[symbol];

}

