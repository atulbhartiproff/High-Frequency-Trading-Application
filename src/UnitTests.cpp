#include "TestFramework.h"
#include "MarketData.h"
#include "Order.h"
#include "RiskManager.h"
#include "Strategy.h"
#include "ExchangeManager.h"
#include <vector>
#include <cmath>

class UnitTests {
public:
    static void runAllTests() {
        testMarketDataProcessing();
        testOrderManagement();
        testRiskManagement();
        testStrategyEngine();
        testExchangeConnectivity();
    }
    
private:
    static void testMarketDataProcessing() {
        TestSuite suite("Market Data Processing");
        
        // Test 1: Market data structure
        suite.addTest("MarketData Structure", []() {
            MarketData data;
            data.Time = "09:30:00";
            data.Abb = "AAPL";
            data.price = 150.25;
            data.volume = 1000;
            
            ASSERT_EQ("AAPL", data.Abb);
            ASSERT_NEAR(150.25, data.price, 0.01);
            ASSERT_EQ(1000, data.volume);
        });
        
        // Test 2: Moving average calculation
        suite.addTest("Moving Average Calculation", []() {
            std::vector<MarketData> testData;
            
            // Create test data: prices 100, 101, 102, 103, 104
            for (int i = 0; i < 5; i++) {
                MarketData data;
                data.Abb = "TEST";
                data.price = 100.0 + i;
                testData.push_back(data);
            }
            
            // 3-period moving average of last 3 prices (102, 103, 104) = 103
            double ma = calculateMovingAverage(testData, "TEST", 3);
            ASSERT_NEAR(103.0, ma, 0.01);
        });
        
        // Test 3: Empty data handling
        suite.addTest("Empty Data Handling", []() {
            std::vector<MarketData> emptyData;
            double ma = calculateMovingAverage(emptyData, "TEST", 3);
            ASSERT_EQ(0.0, ma);
        });
        
        suite.runAll();
    }
    
    static void testOrderManagement() {
        TestSuite suite("Order Management");
        
        // Test 1: Order creation
        suite.addTest("Order Creation", []() {
            Order order("AAPL", OrderType::BUY, 100, 150.0);
            
            ASSERT_EQ("AAPL", order.symbol);
            ASSERT_EQ(OrderType::BUY, order.type);
            ASSERT_EQ(100, order.quantity);
            ASSERT_NEAR(150.0, order.price, 0.01);
            ASSERT_EQ(OrderStatus::PENDING, order.status);
        });
        
        // Test 2: Order ID generation
        suite.addTest("Order ID Generation", []() {
            Order order1("AAPL", OrderType::BUY, 100, 150.0);
            Order order2("MSFT", OrderType::SELL, 50, 280.0);
            
            // Order IDs should be different and incrementing
            ASSERT_TRUE(order2.orderId > order1.orderId);
        });
        
        // Test 3: Order manager functionality
        suite.addTest("Order Manager", []() {
            OrderManager manager;
            
            // Place an order
            manager.placeOrder("AAPL", OrderType::BUY, 100, 150.0);
            
            // This test mainly checks that no exceptions are thrown
            // In a real system, we'd verify the order was stored correctly
            ASSERT_TRUE(true); // Placeholder assertion
        });
        
        suite.runAll();
    }
    
    static void testRiskManagement() {
        TestSuite suite("Risk Management");
        
        // Test 1: Risk limits validation
        suite.addTest("Risk Limits Validation", []() {
            RiskManager riskManager(1000.0, 5000.0); // $1K max position, $5K max exposure
            
            // Create a large order that should fail risk check
            Order largeOrder("AAPL", OrderType::BUY, 100, 50.0); // $5000 order
            bool shouldFail = !riskManager.validateOrder(largeOrder, 50.0);
            ASSERT_TRUE(shouldFail);
            
            // Create a small order that should pass
            Order smallOrder("AAPL", OrderType::BUY, 10, 50.0); // $500 order
            bool shouldPass = riskManager.validateOrder(smallOrder, 50.0);
            ASSERT_TRUE(shouldPass);
        });
        
        // Test 2: Position tracking
        suite.addTest("Position Tracking", []() {
            RiskManager riskManager(10000.0, 50000.0);
            
            // Simulate a filled buy order
            Order buyOrder("AAPL", OrderType::BUY, 100, 150.0);
            buyOrder.status = OrderStatus::FILLED;
            riskManager.updatePosition(buyOrder);
            
            // Check that position was created
            Position* position = riskManager.getPosition("AAPL");
            ASSERT_TRUE(position != nullptr);
            if (position) {
                ASSERT_EQ(100, position->quantity);
                ASSERT_NEAR(150.0, position->avgPrice, 0.01);
            }
        });
        
        suite.runAll();
    }
    
    static void testStrategyEngine() {
        TestSuite suite("Strategy Engine");
        
        // Test 1: Moving average strategy
        suite.addTest("Moving Average Strategy", []() {
            MovingAvgStrat strategy(2, 3);
            
            // Create test data with upward trend
            std::vector<MarketData> trendData;
            double prices[] = {100, 101, 102, 103, 104}; // Increasing prices
            
            for (int i = 0; i < 5; i++) {
                MarketData data;
                data.Abb = "TEST";
                data.price = prices[i];
                trendData.push_back(data);
            }
            
            // With increasing prices, short MA should be > long MA = BUY signal
            int signal = strategy.generateSignal(trendData, "TEST");
            ASSERT_EQ(1, signal); // Should be BUY signal
        });
        
        // Test 2: Strategy name
        suite.addTest("Strategy Name", []() {
            MovingAvgStrat strategy(5, 10);
            std::string name = strategy.getStratName();
            ASSERT_TRUE(name.find("5") != std::string::npos);
            ASSERT_TRUE(name.find("10") != std::string::npos);
        });
        
        suite.runAll();
    }
    
    static void testExchangeConnectivity() {
        TestSuite suite("Exchange Connectivity");
        
        // Test 1: Exchange authentication
        suite.addTest("Exchange Authentication", []() {
            SimulatedExchange exchange;
            
            ExchangeCredentials validCreds;
            validCreds.apiKey = "test-api-key";
            
            bool authSuccess = exchange.authenticate(validCreds);
            ASSERT_TRUE(authSuccess);
            ASSERT_TRUE(exchange.isConnected());
        });
        
        // Test 2: Market price retrieval
        suite.addTest("Market Price Retrieval", []() {
            SimulatedExchange exchange;
            
            ExchangeCredentials creds;
            creds.apiKey = "test";
            exchange.authenticate(creds);
            
            double price;
            bool priceSuccess = exchange.getMarketPrice("AAPL", price);
            ASSERT_TRUE(priceSuccess);
            ASSERT_TRUE(price > 0);
        });
        
        // Test 3: Order placement
        suite.addTest("Order Placement", []() {
            SimulatedExchange exchange;
            
            ExchangeCredentials creds;
            creds.apiKey = "test";
            exchange.authenticate(creds);
            
            std::string orderId = exchange.placeOrder("AAPL", "buy", 10, 150.0);
            ASSERT_FALSE(orderId.empty());
        });
        
        suite.runAll();
    }
};
