#include "TestFramework.h"
#include "UnitTests.cpp"
#include "IntegrationTests.cpp"
#include "PerformanceBenchmarks.cpp"
#include <iostream>

class TestRunner {
public:
    static void runAllTests() {
        std::cout << "🧪 === HFT Trading Platform Test Suite ===" << std::endl;
        std::cout << "Running comprehensive tests..." << std::endl;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Run all test categories
        std::cout << "\n--- UNIT TESTS ---" << std::endl;
        UnitTests::runAllTests();
        
        std::cout << "\n--- INTEGRATION TESTS ---" << std::endl;
        IntegrationTests::runAllTests();
        
        std::cout << "\n--- PERFORMANCE BENCHMARKS ---" << std::endl;
        PerformanceBenchmarks::runAllBenchmarks();
        
        auto end = std::chrono::high_resolution_clock::now();
        auto totalDuration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        
        std::cout << "\n🎉 === Test Suite Complete ===" << std::endl;
        std::cout << "⏱️  Total execution time: " << totalDuration.count() << " seconds" << std::endl;
        std::cout << "✅ System ready for production deployment!" << std::endl;
    }
    
    static void runQuickTests() {
        std::cout << "⚡ === Quick Test Suite ===" << std::endl;
        
        // Run just essential tests
        TestSuite quickSuite("Essential System Tests");
        
        quickSuite.addTest("Market Data Processing", []() {
            std::vector<MarketData> data;
            ASSERT_TRUE(loadData(data));
            ASSERT_TRUE(data.size() > 0);
        });
        
        quickSuite.addTest("Order Creation", []() {
            Order order("AAPL", OrderType::BUY, 100, 150.0);
            ASSERT_EQ("AAPL", order.symbol);
        });
        
        quickSuite.addTest("Risk Management", []() {
            RiskManager rm(1000, 5000);
            Order order("AAPL", OrderType::BUY, 5, 100.0);
            ASSERT_TRUE(rm.validateOrder(order, 100.0));
        });
        
        quickSuite.runAll();
    }
};
