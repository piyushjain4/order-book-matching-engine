
#include "order_book.h"

using namespace std;

 

void OrderBook::setBit(unsigned long long* bitmap, int p) {

    bitmap[p / 64] |= (1ULL << (p % 64));

}

 

void OrderBook::clearBit(unsigned long long* bitmap, int p) {

    bitmap[p / 64] &= ~(1ULL << (p % 64));

}

 

Quantity OrderBook::levelQty(const list<Order*>& level) const {

    Quantity total = 0;

    for (auto o : level)

        total += o->remaining();

    return total;

}

 

int OrderBook::bestBid() const {

    for (int w = BITMAP_WORDS - 1; w >= 0; --w) {

        if (bid_bitmap_[w]) {

            int bit = 63 - __builtin_clzll(bid_bitmap_[w]);

            return w * 64 + bit;

        }

    }

    return -1;

}

 

int OrderBook::bestAsk() const {

    for (int w = 0; w < BITMAP_WORDS; ++w) {

        if (ask_bitmap_[w]) {

            int bit = __builtin_ctzll(ask_bitmap_[w]);

            return w * 64 + bit;

        }

    }

    return -1;

}

 

vector<Trade> OrderBook::matchBuy(Order* order) {

    vector<Trade> trades;

 

    while (order->remaining() > 0) {

        int ap = bestAsk();

        if (ap == -1 || ap > order->price)

            break;

 

        auto& level = ask_levels_[ap];

 

        while (!level.empty() && order->remaining() > 0) {

            Order* resting = level.front();

            int fill = min(order->remaining(), resting->remaining());

 

            order->filled += fill;

            resting->filled += fill;

            trades.push_back({order->id, resting->id, ap, fill});

 

            if (resting->remaining() == 0) {

                level.pop_front();

                orders_.erase(resting->id);

                delete resting;

            }

        }

 

        if (level.empty())

            clearBit(ask_bitmap_, ap);

    }

 

    return trades;

}

 

vector<Trade> OrderBook::matchSell(Order* order) {

    vector<Trade> trades;

 

    while (order->remaining() > 0) {

        int bp = bestBid();

        if (bp == -1 || bp < order->price)

            break;

 

        auto& level = bid_levels_[bp];

 

        while (!level.empty() && order->remaining() > 0) {

            Order* resting = level.front();

            int fill = min(order->remaining(), resting->remaining());

 

            order->filled += fill;

            resting->filled += fill;

            trades.push_back({resting->id, order->id, bp, fill});

 

            if (resting->remaining() == 0) {

                level.pop_front();

                orders_.erase(resting->id);

                delete resting;

            }

        }

 

        if (level.empty())

            clearBit(bid_bitmap_, bp);

    }

 

    return trades;

}

 

vector<Trade> OrderBook::addOrder(Side side, Price price, Quantity qty) {

    Order* order = new Order{next_id_++, side, price, qty};

 

    vector<Trade> trades;

    if (side == Side::Buy)

        trades = matchBuy(order);

    else

        trades = matchSell(order);

 

    if (order->remaining() > 0) {

        if (side == Side::Buy) {

            bid_levels_[price].push_back(order);

            order->queue_pos = prev(bid_levels_[price].end());

            setBit(bid_bitmap_, price);

        } else {

            ask_levels_[price].push_back(order);

            order->queue_pos = prev(ask_levels_[price].end());

            setBit(ask_bitmap_, price);

        }

        orders_[order->id] = order;

    } else {

        delete order;

    }

 

    return trades;

}

 

bool OrderBook::cancelOrder(OrderId id) {

    auto it = orders_.find(id);

    if (it == orders_.end())

        return false;

 

    Order* order = it->second;

    int price = order->price;

 

    if (order->side == Side::Buy) {

        bid_levels_[price].erase(order->queue_pos);

        if (bid_levels_[price].empty())

            clearBit(bid_bitmap_, price);

    } else {

        ask_levels_[price].erase(order->queue_pos);

        if (ask_levels_[price].empty())

            clearBit(ask_bitmap_, price);

    }

 

    orders_.erase(it);

    delete order;

    return true;

}

 

vector<OrderBook::LevelInfo> OrderBook::bidDepth(int levels) const {

    vector<LevelInfo> result;

    int p = bestBid();

 

    while (p != -1 && (int)result.size() < levels) {

        result.push_back({p, levelQty(bid_levels_[p])});

 

        if (p == 0) break;

        int next = p - 1;

        bool found = false;

 

        for (int w = next / 64; w >= 0; --w) {

            unsigned long long word = bid_bitmap_[w];

            if (w == next / 64)

                word &= (2ULL << (next % 64)) - 1;

            if (word) {

                int bit = 63 - __builtin_clzll(word);

                p = w * 64 + bit;

                found = true;

                break;

            }

        }

        if (!found) break;

    }

 

    return result;

}

 

vector<OrderBook::LevelInfo> OrderBook::askDepth(int levels) const {

    vector<LevelInfo> result;

    int p = bestAsk();

 

    while (p != -1 && (int)result.size() < levels) {

        result.push_back({p, levelQty(ask_levels_[p])});

 

        int next = p + 1;

        if (next >= MAX_PRICE) break;

        bool found = false;

 

        for (int w = next / 64; w < BITMAP_WORDS; ++w) {

            unsigned long long word = ask_bitmap_[w];

            if (w == next / 64)

                word &= ~((1ULL << (next % 64)) - 1);

            if (word) {

                int bit = __builtin_ctzll(word);

                p = w * 64 + bit;

                found = true;

                break;

            }

        }

        if (!found) break;

    }

 

    return result;

}

