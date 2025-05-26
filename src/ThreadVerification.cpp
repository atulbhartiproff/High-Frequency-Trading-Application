#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <sched.h>
#include <unistd.h>
#include <atomic>

class ThreadVerification {
public:
    static void verifyConcurrentExecution() {
        std::cout << "\n🔍 Verifying Multi-Core Execution..." << std::endl;
        
        // Get number of CPU cores
        int num_cores = std::thread::hardware_concurrency();
        std::cout << "💻 Available CPU cores: " << num_cores << std::endl;
        
        if (num_cores <= 1) {
            std::cout << "⚠️  Single core system - threads will time-slice, not run in parallel!" << std::endl;
            return;
        }
        
        // Test 1: Sequential execution timing
        auto start_sequential = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 4; ++i) {
            heavyComputation(1000000);
        }
        auto end_sequential = std::chrono::high_resolution_clock::now();
        auto sequential_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_sequential - start_sequential);
        
        // Test 2: Parallel execution timing
        auto start_parallel = std::chrono::high_resolution_clock::now();
        std::vector<std::thread> threads;
        
        for (int i = 0; i < 4; ++i) {
            threads.emplace_back([i]() {
                // Pin each thread to a different core
                cpu_set_t cpuset;
                CPU_ZERO(&cpuset);
                CPU_SET(i % std::thread::hardware_concurrency(), &cpuset);
                pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
                
                std::cout << "🧵 Thread " << i << " running on core " << sched_getcpu() << std::endl;
                heavyComputation(1000000);
            });
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
        auto end_parallel = std::chrono::high_resolution_clock::now();
        auto parallel_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_parallel - start_parallel);
        
        // Results
        std::cout << "\n📊 Performance Comparison:" << std::endl;
        std::cout << "Sequential execution: " << sequential_duration.count() << "ms" << std::endl;
        std::cout << "Parallel execution: " << parallel_duration.count() << "ms" << std::endl;
        
        double speedup = (double)sequential_duration.count() / parallel_duration.count();
        std::cout << "Speedup: " << speedup << "x" << std::endl;
        
        if (speedup > 1.5) {
            std::cout << "✅ TRUE parallel execution detected! Using multiple cores." << std::endl;
        } else {
            std::cout << "❌ Sequential execution detected. Threads may be on same core." << std::endl;
        }
    }
    
    static void verifyLockFreeQueue() {
        std::cout << "\n🔍 Testing Lock-Free Queue Concurrency..." << std::endl;
        
        const int num_producers = 2;
        const int num_consumers = 2;
        const int items_per_thread = 1000;
        
        // Simple lock-free queue test
        std::atomic<int> produced_count{0};
        std::atomic<int> consumed_count{0};
        
        std::vector<std::thread> producers;
        std::vector<std::thread> consumers;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Start producers
        for (int i = 0; i < num_producers; ++i) {
            producers.emplace_back([&produced_count, items_per_thread, i]() {
                cpu_set_t cpuset;
                CPU_ZERO(&cpuset);
                CPU_SET(i, &cpuset);
                pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
                
                for (int j = 0; j < items_per_thread; ++j) {
                    produced_count.fetch_add(1);
                }
                std::cout << "🏭 Producer " << i << " finished on core " << sched_getcpu() << std::endl;
            });
        }
        
        // Start consumers
        for (int i = 0; i < num_consumers; ++i) {
            consumers.emplace_back([&consumed_count, &produced_count, i]() {
                cpu_set_t cpuset;
                CPU_ZERO(&cpuset);
                CPU_SET(i + 2, &cpuset);  // Use different cores
                pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
                
                while (consumed_count.load() < (num_producers * items_per_thread)) {
                    if (produced_count.load() > consumed_count.load()) {
                        consumed_count.fetch_add(1);
                    }
                }
                std::cout << "🛒 Consumer " << i << " finished on core " << sched_getcpu() << std::endl;
            });
        }
        
        // Wait for completion
        for (auto& producer : producers) {
            producer.join();
        }
        for (auto& consumer : consumers) {
            consumer.join();
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "📈 Produced: " << produced_count.load() << " items" << std::endl;
        std::cout << "📉 Consumed: " << consumed_count.load() << " items" << std::endl;
        std::cout << "⏱️  Duration: " << duration.count() << "ms" << std::endl;
        
        if (produced_count.load() == consumed_count.load()) {
            std::cout << "✅ Lock-free queue working correctly!" << std::endl;
        } else {
            std::cout << "❌ Race condition detected!" << std::endl;
        }
    }

private:
    static void heavyComputation(int iterations) {
        volatile long result = 0;
        for (int i = 0; i < iterations; ++i) {
            result += i * i * i;
        }
    }
};
