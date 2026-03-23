# 📈 Order Book Matching Engine (C++)

A high-performance **limit order book and matching engine** implemented
in modern C++ (C++17). This project simulates how financial exchanges
match buy and sell orders using price-time priority.

------------------------------------------------------------------------

## 🚀 Features

-   Limit order book (bid/ask)
-   Price-time priority matching
-   Aggressive order matching (crossing the spread)
-   Order cancellation support
-   Efficient best bid/ask lookup using bitmaps
-   Depth view (Level 2 market data)
-   Multi-symbol support via matching engine

------------------------------------------------------------------------

## 🧠 Core Concepts

### Order Matching

-   **Buy orders** match against the lowest available ask
-   **Sell orders** match against the highest available bid
-   Matching continues until:
    -   Order is fully filled, or
    -   No more compatible prices exist

### Price-Time Priority

-   Better price gets priority
-   For same price → earlier order gets filled first (FIFO)

------------------------------------------------------------------------

## 📂 Project Structure

    order-book/
    ├── include/
    │   ├── types.h
    │   ├── order_book.h
    │   └── matching_engine.h
    ├── src/
    │   ├── order_book.cpp
    │   └── matching_engine.cpp
    ├── main.cpp
    └── Makefile

------------------------------------------------------------------------

## ⚙️ Build & Run

### Prerequisites

-   C++17 compatible compiler (e.g., `g++`)
-   Make (Linux/macOS or Windows with MinGW)

### Build

``` bash
cd order-book
make
```

### Run

``` bash
./orderbook
```

On Windows:

``` powershell
.\orderbook.exe
```

------------------------------------------------------------------------

## 📊 Example Output

    === Building order book for AAPL ===
    
    Initial book:
    
      ---- ASKS ----
        1060  |  300
        1055  |  150
        1050  |  300
      --------------
      ---- BIDS ----
        1040  |  250
        1035  |  200
        1030  |  250
      --------------
    
    === BUY 400 @ 1055 (crosses spread) ===
      TRADE  bid#9 x ask#1  price=1050  qty=100
      TRADE  bid#9 x ask#2  price=1050  qty=200
      TRADE  bid#9 x ask#3  price=1055  qty=100
    
    After aggressive buy:
    
      ---- ASKS ----
        1060  |  300
        1055  |  50
      --------------
      ---- BIDS ----
        1040  |  250
        1035  |  200
        1030  |  250
      --------------
    
    === Cancel order #7 (BID 200 @ 1035) ===
      Cancel succeeded
    
    After cancel:
    
      ---- ASKS ----
        1060  |  300
        1055  |  50
      --------------
      ---- BIDS ----
        1040  |  250
        1030  |  250
      --------------
    
    === SELL 120 @ 1040 (hits top bid) ===
      TRADE  bid#5 x ask#10  price=1040  qty=100
      TRADE  bid#6 x ask#10  price=1040  qty=20
    
    Final book:
    
      ---- ASKS ----
        1060  |  300
        1055  |  50
      --------------
      ---- BIDS ----
        1040  |  130
        1030  |  250
      --------------

## 🔧 Key Components

### `OrderBook`

Handles: - Order storage - Matching logic - Price levels - Bitmaps for
fast lookup

### `MatchingEngine`

-   Manages multiple symbols
-   Routes orders to corresponding order books

### Bitmaps for Performance

-   Near O(1) access to best bid/ask
-   Efficient scanning of price levels

------------------------------------------------------------------------

## 📌 Supported Operations

  Operation      Description
  -------------- --------------------------
  `submit()`     Add a new buy/sell order
  `cancel()`     Cancel an existing order
  `bidDepth()`   View top bid levels
  `askDepth()`   View top ask levels

------------------------------------------------------------------------

## 🧪 Example Workflow

1.  Build initial book
2.  Submit aggressive order (cross spread)
3.  Observe trades
4.  Cancel an order
5.  Submit another order
6.  View final book state

------------------------------------------------------------------------

## 💡 Future Improvements

-   Market orders
-   Stop orders
-   Persistence (disk storage)
-   Multithreading / concurrency
-   Latency optimizations
-   Unit & stress testing

------------------------------------------------------------------------


## 🙌 Acknowledgements

Inspired by real-world exchange matching engines and high-frequency
trading systems.
