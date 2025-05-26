cmake_minimum_required(VERSION 3.20)
project(HFTTradingPlatform)

set(CMAKE_CXX_STANDARD 20)

# Performance optimization flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native -mtune=native -flto")

include_directories(include)

add_executable(trading_platform 
    src/main.cpp 
    src/MarketData.cpp 
    src/Strategy.cpp
    src/Order.cpp
    src/RiskManager.cpp
    src/PerformanceMonitor.cpp
    src/ExchangeAPI.cpp
    src/ExchangeManager.cpp
)

# Link pthread for multi-threading
target_link_libraries(trading_platform pthread)
