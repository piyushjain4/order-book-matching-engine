CXX      = g++

CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Iinclude

TARGET   = orderbook

SRCS     = src/order_book.cpp src/matching_engine.cpp main.cpp

OBJS     = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q $(TARGET).exe $(OBJS) 2>nul || true

.PHONY: all clean