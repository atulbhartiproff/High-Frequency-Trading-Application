cmake_minimum_required(VERSION 3.20)
project(HFTTradingPlatform)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(trading_platform src/main.cpp src/MarketData.cpp src/strategy.cpp src/Order.cpp)
