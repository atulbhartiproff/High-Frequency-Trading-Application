#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <chrono>

// Simple testing framework (similar to Catch2 but lighter)
class TestCase {
private:
    std::string name;
    std::function<void()> testFunction;
    
public:
    TestCase(const std::string& testName, std::function<void()> func) 
        : name(testName), testFunction(func) {}
    
    bool run() {
        std::cout << "🧪 Running: " << name << "..." << std::endl;
        
        try {
            auto start = std::chrono::high_resolution_clock::now();
            testFunction();
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "✅ PASSED: " << name << " (" << duration.count() << "μs)" << std::endl;
            return true;
        }
        catch (const std::exception& e) {
            std::cout << "❌ FAILED: " << name << " - " << e.what() << std::endl;
            return false;
        }
        catch (...) {
            std::cout << "❌ FAILED: " << name << " - Unknown error" << std::endl;
            return false;
        }
    }
    
    const std::string& getName() const { return name; }
};

class TestSuite {
private:
    std::vector<TestCase> tests;
    std::string suiteName;
    
public:
    TestSuite(const std::string& name) : suiteName(name) {}
    
    void addTest(const std::string& testName, std::function<void()> testFunc) {
        tests.emplace_back(testName, testFunc);
    }
    
    void runAll() {
        std::cout << "\n🚀 === Running Test Suite: " << suiteName << " ===" << std::endl;
        
        int passed = 0;
        int failed = 0;
        
        auto suiteStart = std::chrono::high_resolution_clock::now();
        
        for (auto& test : tests) {
            if (test.run()) {
                passed++;
            } else {
                failed++;
            }
        }
        
        auto suiteEnd = std::chrono::high_resolution_clock::now();
        auto suiteDuration = std::chrono::duration_cast<std::chrono::milliseconds>(suiteEnd - suiteStart);
        
        std::cout << "\n📊 === Test Results ===" << std::endl;
        std::cout << "✅ Passed: " << passed << std::endl;
        std::cout << "❌ Failed: " << failed << std::endl;
        std::cout << "⏱️  Total time: " << suiteDuration.count() << "ms" << std::endl;
        
        if (failed == 0) {
            std::cout << "🎉 All tests passed!" << std::endl;
        } else {
            std::cout << "⚠️  " << failed << " tests failed!" << std::endl;
        }
    }
};

// Assertion macros
#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        throw std::runtime_error("Assertion failed: " #condition); \
    }

#define ASSERT_FALSE(condition) \
    if (condition) { \
        throw std::runtime_error("Assertion failed: !(" #condition ")"); \
    }

#define ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        throw std::runtime_error("Assertion failed: " #expected " == " #actual); \
    }

#define ASSERT_NEAR(expected, actual, tolerance) \
    if (std::abs((expected) - (actual)) > (tolerance)) { \
        throw std::runtime_error("Assertion failed: " #expected " ≈ " #actual " (tolerance: " #tolerance ")"); \
    }
