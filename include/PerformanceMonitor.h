#pragma once
#include <chrono>
#include <string>
#include <iostream>
#include "Order.h"

class PerformanceTimer {
private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::string operationName;
    
public:
    PerformanceTimer(const std::string& operation);
    ~PerformanceTimer();
    
    void stop();
};

class PerformanceMonitor {
public:
    static void measureDataLoad();
    static void measureSignalGeneration();
    static void measureOrderPlacement();
    static void measureCPUAffinity();
    static void measureCacheOptimization();
    static void verifyMultiThreading();  // New verification method
};
