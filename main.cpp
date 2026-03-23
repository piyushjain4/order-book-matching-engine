#include "matching_engine.h"
#include <iostream>

using namespace std;

void printTrades(const vector<Trade>& trades) {
    for (auto& t : trades) {
        cout << "  TRADE  bid#" << t.bid_id
             << " x ask#" << t.ask_id
             << "  price=" << t.price
             << "  qty=" << t.quantity << "\n";
    }
}

void printBook(OrderBook& book, int depth = 5) {

    auto asks = book.askDepth(depth);
    auto bids = book.bidDepth(depth);

    cout << "\n  ---- ASKS ----\n";
    for (int i = (int)asks.size() - 1; i >= 0; --i) {
        cout << "    " << asks[i].first << "  |  " << asks[i].second << "\n";
    }

    cout << "  --------------\n";

    cout << "  ---- BIDS ----\n";
    for (auto& p : bids) {
        auto& price = p.first;
        auto& qty   = p.second;

        cout << "    " << price << "  |  " << qty << "\n";
    }

    cout << "  --------------\n\n";
}

int main() {

    MatchingEngine engine;
    string sym = "AAPL";

    cout << "=== Building order book for " << sym << " ===\n\n";

    engine.submit(sym, Side::Sell, 1050, 100);
    engine.submit(sym, Side::Sell, 1050, 200);
    engine.submit(sym, Side::Sell, 1055, 150);
    engine.submit(sym, Side::Sell, 1060, 300);

    engine.submit(sym, Side::Buy, 1040, 100);
    engine.submit(sym, Side::Buy, 1040, 150);
    engine.submit(sym, Side::Buy, 1035, 200);
    engine.submit(sym, Side::Buy, 1030, 250);

    cout << "Initial book:\n";
    printBook(engine.book(sym));

    cout << "=== BUY 400 @ 1055 (crosses spread) ===\n";
    auto trades = engine.submit(sym, Side::Buy, 1055, 400);
    printTrades(trades);

    cout << "\nAfter aggressive buy:\n";
    printBook(engine.book(sym));

    cout << "=== Cancel order #7 (BID 200 @ 1035) ===\n";
    bool ok = engine.cancel(sym, 7);
    cout << "  Cancel " << (ok ? "succeeded" : "failed") << "\n";

    cout << "\nAfter cancel:\n";
    printBook(engine.book(sym));

    cout << "=== SELL 120 @ 1040 (hits top bid) ===\n";
    trades = engine.submit(sym, Side::Sell, 1040, 120);
    printTrades(trades);

    cout << "\nFinal book:\n";
    printBook(engine.book(sym));

    return 0;
}