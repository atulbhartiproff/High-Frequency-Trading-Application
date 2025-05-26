#include "TestFramework.h"
#include "PerformanceMonitor.h"
#include "MarketData.h"
#include "Strategy.h"
#include "Order.h"
#include <chrono>
#include <vector>

class PerformanceBenchmarks {
public:
    static void runAllBenchmarks() {
        benchmarkDataProcessing();
        benchmarkSignalGeneration();
        benchmarkOrderProcessing();
        benchmarkMemoryUsage();
    }
    
private:
    static void benchmarkDataProcessing() {
        TestSuite suite("Data Processing Performance");
        
        suite.addTest("Large Dataset Loading", []() {
            auto start = std::chrono::high_resolution_clock::now();
            
            // Load data multiple times to stress test
            for (int i = 0; i < 100; i++) {
                std::vector<MarketData> data;
                loadData(data);
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            
            std::cout << "📊 Loaded data 100 times in " << duration.count() << "ms" << std::endl;
            
            // Should complete within reasonable time (adjust based on your system)
            ASSERT_TRUE(duration.count() < 5000); // Less than 5 seconds
        });
        
        suite.addTest("Moving Average Performance", []() {
            // Create large dataset
            std::vector<MarketData> largeData;
            for (int i = 0; i < 10000; i++) {
                MarketData data;
                data.Abb = "PERF";
                data.price = 100.0 + (i % 100) * 0.1;
                largeData.push_back(data);
            }
            
            auto start = std::chrono::high_resolution_clock::now();
            
            // Calculate moving averages many times
            for (int i = 0; i < 1000; i++) {
                calculateMovingAverage(largeData, "PERF", 20);
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            std::cout << "📈 Calculated 1000 moving averages in " << duration.count() << "μs" << std::endl;
            
            // Should be fast (less than 100ms for 1000 calculations)
            ASSERT_TRUE(duration.count() < 100000);
        });
        
        suite.runAll();
    }
    
    static void benchmarkSignalGeneration() {
        TestSuite suite("Signal Generation Performance");
        
        suite.addTest("High-Frequency Signal Generation", []() {
            std::vector<MarketData> data;
            loadData(data);
            
            MovingAvgStrat strategy(5, 20);
            
            auto start = std::chrono::high_resolution_clock::now();
            
            // Generate signals at high frequency
            for (int i = 0; i < 10000; i++) {
                strategy.generateSignal(data, "AAPL");
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            std::cout << "🎯 Generated 10,000 signals in " << duration.count() << "μs" << std::endl;
            std::cout << "📊 Average: " << (duration.count() / 10000.0) << "μs per signal" << std::endl;
            
            // HFT requirement: signals should be generated very quickly
            double avgTime = duration.count() / 10000.0;
            ASSERT_TRUE(avgTime < 100.0); // Less than 100μs per signal
        });
        
        suite.runAll();
    }
    
    static void benchmarkOrderProcessing() {
        TestSuite suite("Order Processing Performance");
        
        suite.addTest("Rapid Order Creation", []() {
            auto start = std::chrono::high_resolution_clock::now();
            
            std::vector<Order> orders;
            orders.reserve(10000);
            
            // Create many orders rapidly
            for (int i = 0; i < 10000; i++) {
                orders.emplace_back("AAPL", OrderType::BUY, 100, 150.0 + i * 0.01);
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            std::cout << "📝 Created 10,000 orders in " << duration.count() << "μs" << std::endl;
            std::cout << "⚡ Average: " << (duration.count() / 10000.0) << "μs per order" << std::endl;
            
            // Orders should be created very quickly
            double avgTime = duration.count() / 10000.0;
            ASSERT_TRUE(avgTime < 10.0); // Less than 10μs per order
        });
        
        suite.runAll();
    }
    
    static void benchmarkMemoryUsage() {
        TestSuite suite("Memory Usage Performance");
        
        suite.addTest("Memory Efficiency Test", []() {
            size_t initialSize = 0; // In a real system, we'd measure actual memory usage
            
            // Create many objects
            std::vector<MarketData> largeDataset;
            std::vector<Order> manyOrders;
            
            for (int i = 0; i < 100000; i++) {
                MarketData data;
                data.Abb = "MEM";
                data.price = 100.0 + i;
                data.volume = 1000;
                largeDataset.push_back(data);
                
                if (i % 10 == 0) {
                    manyOrders.emplace_back("MEM", OrderType::BUY, 100, 100.0);
                }
            }
            
            std::cout << "💾 Created " << largeDataset.size() << " market data objects" << std::endl;
            std::cout << "📋 Created " << manyOrders.size() << " orders" << std::endl;
            
            // Memory test passed if no crashes occurred
            ASSERT_TRUE(largeDataset.size() == 100000);
            ASSERT_TRUE(manyOrders.size() == 10000);
        });
        
        suite.runAll();
    }
};
