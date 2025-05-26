#include "TestFramework.h"
#include "MarketData.h"
#include "Order.h"
#include "RiskManager.h"
#include "ExchangeManager.h"
#include <vector>

class IntegrationTests {
public:
    static void runAllTests() {
        testEndToEndTradingFlow();
        testRiskIntegration();
        testExchangeIntegration();
    }
    
private:
    static void testEndToEndTradingFlow() {
        TestSuite suite("End-to-End Trading Flow");
        
        // Test complete trading workflow
        suite.addTest("Complete Trading Workflow", []() {
            // 1. Load market data
            std::vector<MarketData> marketData;
            bool dataLoaded = loadData(marketData);
            ASSERT_TRUE(dataLoaded);
            ASSERT_TRUE(marketData.size() > 0);
            
            // 2. Initialize risk manager
            RiskManager riskManager(5000.0, 25000.0);
            riskManager.updateMarketPrices(marketData);
            
            // 3. Create strategy
            MovingAvgStrat strategy(2, 3);
            
            // 4. Generate signal
            int signal = strategy.generateSignal(marketData, "AAPL");
            ASSERT_TRUE(signal >= -1 && signal <= 1);
            
            // 5. Create order based on signal
            if (signal == 1) { // BUY signal
                Order order("AAPL", OrderType::BUY, 10, 150.0);
                
                // 6. Validate with risk manager
                bool riskPassed = riskManager.validateOrder(order, 150.0);
                
                if (riskPassed) {
                    // 7. Execute order (simulate)
                    order.status = OrderStatus::FILLED;
                    riskManager.updatePosition(order);
                    
                    // 8. Verify position was created
                    Position* position = riskManager.getPosition("AAPL");
                    ASSERT_TRUE(position != nullptr);
                }
            }
            
            // Test completed successfully
            ASSERT_TRUE(true);
        });
        
        suite.runAll();
    }
    
    static void testRiskIntegration() {
        TestSuite suite("Risk Management Integration");
        
        suite.addTest("Risk Limits Prevent Overexposure", []() {
            RiskManager riskManager(1000.0, 5000.0); // Small limits for testing
            
            // Try to place multiple orders that exceed total exposure
            std::vector<Order> orders;
            
            for (int i = 0; i < 10; i++) {
                Order order("AAPL", OrderType::BUY, 10, 100.0); // $1000 each
                
                if (riskManager.validateOrder(order, 100.0)) {
                    order.status = OrderStatus::FILLED;
                    riskManager.updatePosition(order);
                    orders.push_back(order);
                } else {
                    break; // Risk limit reached
                }
            }
            
            // Should not be able to place all 10 orders (would exceed $5000 limit)
            ASSERT_TRUE(orders.size() < 10);
            
            // Total exposure should be within limits
            double totalExposure = riskManager.getTotalExposure();
            ASSERT_TRUE(totalExposure <= 5000.0);
        });
        
        suite.runAll();
    }
    
    static void testExchangeIntegration() {
        TestSuite suite("Exchange Integration");
        
        suite.addTest("Exchange Manager Full Flow", []() {
            ExchangeManager exchangeManager;
            
            // 1. Connect to exchange
            ExchangeCredentials creds;
            creds.apiKey = "test-integration";
            bool connected = exchangeManager.connectToExchange(creds);
            ASSERT_TRUE(connected);
            
            // 2. Get live price
            double price;
            bool priceReceived = exchangeManager.getLivePrice("AAPL", price);
            ASSERT_TRUE(priceReceived);
            ASSERT_TRUE(price > 0);
            
            // 3. Execute live order
            std::string orderId = exchangeManager.executeLiveOrder("AAPL", "buy", 1, price);
            ASSERT_FALSE(orderId.empty());
            
            // Test completed successfully
            ASSERT_TRUE(true);
        });
        
        suite.runAll();
    }
};
