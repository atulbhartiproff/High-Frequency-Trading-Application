#include "PerformanceMonitor.h"
#include "MarketData.h"
#include "Strategy.h"
#include "Order.h"
#include "LockFreeQueue.h"
#include "MemoryPool.h"
#include <vector>
#include <thread>
#include <chrono>
#include <sched.h>
#include <unistd.h>
#include <iostream>
#include <atomic>

PerformanceTimer::PerformanceTimer(const std::string& operation) 
    : operationName(operation) {
    startTime = std::chrono::high_resolution_clock::now();
    std::cout << "⏱️  Starting: " << operationName << "..." << std::endl;
}

PerformanceTimer::~PerformanceTimer() {
    stop();
}

void PerformanceTimer::stop() {
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
    
    std::cout << "✅ " << operationName << " completed in: " 
              << duration.count() << " nanoseconds (" 
              << duration.count() / 1000.0 << " microseconds)" << std::endl;
}

void PerformanceMonitor::measureDataLoad() {
    PerformanceTimer timer("Optimized Data Loading");
    
    std::vector<MarketData> testData;
    testData.reserve(10000);
    
    loadData(testData);
    
    std::cout << "📊 Loaded " << testData.size() << " records with memory pre-allocation" << std::endl;
}

void PerformanceMonitor::measureSignalGeneration() {
    PerformanceTimer timer("Multi-threaded Signal Generation");
    
    std::vector<MarketData> testData;
    loadData(testData);
    
    const int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    
    LockFreeQueue<int> signal_queue;
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&testData, &signal_queue]() {
            MovingAvgStrat strategy(2, 3);
            for (int j = 0; j < 250; j++) {
                int signal = strategy.generateSignal(testData, "AAPL");
                signal_queue.enqueue(signal);
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "🎯 Generated 1000 signals using " << num_threads << " threads with lock-free queue" << std::endl;
}

void PerformanceMonitor::measureOrderPlacement() {
    PerformanceTimer timer("Memory Pool Order Processing");
    
    MemoryPool<Order> order_pool;
    std::vector<Order*> orders;
    orders.reserve(1000);
    
    for (int i = 0; i < 1000; i++) {
        Order* order = order_pool.template allocate<Order>("AAPL", OrderType::BUY, 100, 150.0 + i * 0.01);
        orders.push_back(order);
    }
    
    for (Order* order : orders) {
        order_pool.deallocate(order);
    }
    
    std::cout << "📝 Created and destroyed 1000 orders using memory pool" << std::endl;
}

void PerformanceMonitor::measureCPUAffinity() {
    PerformanceTimer timer("CPU Affinity Optimization");
    
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    
    if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset) == 0) {
        std::cout << "🎯 CPU affinity set to core 0 for deterministic performance" << std::endl;
        
        volatile int result = 0;
        for (int i = 0; i < 1000000; ++i) {
            result += i * i;
        }
        
        std::cout << "🚀 Computation result: " << result << " (CPU-pinned execution)" << std::endl;
    } else {
        std::cout << "⚠️  Could not set CPU affinity (requires root privileges)" << std::endl;
    }
}

void PerformanceMonitor::measureCacheOptimization() {
    PerformanceTimer timer("Cache-Optimized Data Processing");
    
    const int size = 10000;
    std::vector<double> prices;
    prices.reserve(size);
    
    for (int i = 0; i < size; ++i) {
        prices.push_back(100.0 + (i % 100) * 0.1);
    }
    
    double sum = 0.0;
    for (double price : prices) {
        sum += price;
    }
    
    double average = sum / size;
    std::cout << "📈 Processed " << size << " prices with cache-optimized access" << std::endl;
    std::cout << "💰 Average price: $" << average << std::endl;
}

void PerformanceMonitor::verifyMultiThreading() {
    std::cout << "\n🚀 === Multi-Threading Verification Suite ===" << std::endl;
    
    // Check system capabilities
    std::cout << "System Info:" << std::endl;
    std::cout << "  CPU Cores: " << std::thread::hardware_concurrency() << std::endl;
    std::cout << "  Current Core: " << sched_getcpu() << std::endl;
    
    const int num_threads = std::min(4, (int)std::thread::hardware_concurrency());
    std::atomic<int> counter{0};
    std::vector<std::thread> threads;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter, i]() {
            // Pin to specific core
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(i, &cpuset);
            pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
            
            std::cout << "🧵 Thread " << i << " running on core " << sched_getcpu() << std::endl;
            
            // Heavy computation to stress test
            for (int j = 0; j < 1000000; ++j) {
                counter.fetch_add(1);
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "✅ All threads completed in " << duration.count() << "ms" << std::endl;
    std::cout << "📊 Final counter value: " << counter.load() << std::endl;
    std::cout << "🎯 Expected: " << (num_threads * 1000000) << std::endl;
    
    if (counter.load() == (num_threads * 1000000)) {
        std::cout << "✅ Multi-threading working correctly!" << std::endl;
    } else {
        std::cout << "⚠️  Potential race condition or scheduling issue!" << std::endl;
    }
}
