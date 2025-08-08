#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

// Forward declarations
class Cryptocurrency;
class Portfolio;
class Transaction;

// Base Cryptocurrency class
class Cryptocurrency {
protected:
    string cryptoSymbol;
    string cryptoName;
    double currentPrice;
    double marketCap;
    
public:
    // Multiple constructors
    Cryptocurrency() {
        cryptoSymbol = "UNKNOWN";
        cryptoName = "Unknown Crypto";
        currentPrice = 0.0;
        marketCap = 0.0;
    }
    
    Cryptocurrency(string symbol, string name, double price) {
        cryptoSymbol = symbol;
        cryptoName = name;
        currentPrice = price;
        marketCap = 1.0;
    }
    
    // Virtual methods for polymorphism
    virtual double calculateVolatility() const = 0;
    virtual void displayInfo() const {
        cout << cryptoName << " (" << cryptoSymbol << ") - $" 
             << fixed << setprecision(9) << currentPrice << endl;
    }
    
    // Const methods
    string getSymbol() const { return cryptoSymbol; }
    string getName() const { return cryptoName; }
    double getPrice() const { return currentPrice; }
    
    // Price update method with realistic volatility
    void updatePrice() {
        double volatilityPercent = calculateVolatility();
        double changePercent = ((rand() % 201 - 100) / 100.0) * (volatilityPercent / 100.0);
        double oldPrice = currentPrice;
        
        currentPrice += currentPrice * changePercent;
        if (currentPrice < 0.01) currentPrice = 0.01;
        
        // Show price movement
        cout << cryptoName << " (" << cryptoSymbol << "): $" << fixed << setprecision(2) 
             << oldPrice << " -> $" << currentPrice;
        if (changePercent > 0) {
            cout << " (+" << fixed << setprecision(2) << (changePercent * 100) << "%)" << endl;
        } else {
            cout << " (" << fixed << setprecision(2) << (changePercent * 100) << "%)" << endl;
        }
    }
    
    // Operator overloading
    bool operator>(const Cryptocurrency& other) const {
        return currentPrice > other.currentPrice;
    }
    
    Cryptocurrency& operator+=(double increase) {
        currentPrice += increase;
        return *this;
    }
    
    virtual ~Cryptocurrency() {}
};

// Bitcoin derived class
class Bitcoin : public Cryptocurrency {
private:
    bool halvingYear;
    
public:
    Bitcoin() : Cryptocurrency("BTC", "Bitcoin", 118100.0) {
        halvingYear = false;
    }
    
    Bitcoin(double price) : Cryptocurrency("BTC", "Bitcoin", price) {
        halvingYear = false;
    }
    
    double calculateVolatility() const override {
        return halvingYear ? 4.0 : 2.5;
    }
    
    void displayInfo() const override {
        cout << "Bitcoin (BTC) - $" << fixed << setprecision(2) << currentPrice;
        cout << " | Volatility: " << calculateVolatility() << "%" << endl;
    }
    
    void setHalving(bool status) { halvingYear = status; }
};

// Ethereum derived class  
class Ethereum : public Cryptocurrency {
private:
    double stakingReward;
    
public:
    Ethereum() : Cryptocurrency("ETH", "Ethereum", 3760.40) {
        stakingReward = 4.2;
    }
    
    Ethereum(double price) : Cryptocurrency("ETH", "Ethereum", price) {
        stakingReward = 4.2;
    }
    
    double calculateVolatility() const override {
        return 3.2;
    }
    
    void displayInfo() const override {
        cout << "Ethereum (ETH) - $" << fixed << setprecision(2) << currentPrice;
        cout << " | Staking: " << stakingReward << "%" << endl;
    }
};

// Altcoin derived class
class Altcoin : public Cryptocurrency {
private:
    int marketRank;
    double riskLevel;
    
public:
    Altcoin() : Cryptocurrency("ALT", "Generic Altcoin", 0.003) {
        marketRank = 100;
        riskLevel = 8.5;
    }
    
    Altcoin(string symbol, string name, double price, int rank) 
        : Cryptocurrency(symbol, name, price) {
        marketRank = rank;
        riskLevel = 5.0 + (rank / 20.0);
    }
    
    double calculateVolatility() const override {
        return 5.0 + (riskLevel * 0.5);
    }
    
    void displayInfo() const override {
        cout << cryptoName << " (" << cryptoSymbol << ") - $" 
             << fixed << setprecision(2) << currentPrice;
        cout << " | Rank: #" << marketRank << endl;
    }
};

// Transaction class with static members
class Transaction {
private:
    static int totalTransactions;
    static double totalVolume;
    
    string txnId;
    string symbol;
    double amount;
    double price;
    string type;
    
public:
    Transaction() {
        totalTransactions++;
        txnId = "TXN" + to_string(totalTransactions);
        symbol = "NONE";
        amount = 0.0;
        price = 0.0;
        type = "BUY";
    }
    
    Transaction(string sym, double amt, double prc, string tp) {
        totalTransactions++;
        txnId = "TXN" + to_string(totalTransactions);
        symbol = sym;
        amount = amt;
        price = prc;
        type = tp;
        totalVolume += (amt * prc);
    }
    
    // Static methods
    static int getTotalTransactions() { return totalTransactions; }
    static double getTotalVolume() { return totalVolume; }
    static void showGlobalStats() {
        cout << "Total Transactions: " << totalTransactions << endl;
        cout << "Total Volume: $" << fixed << setprecision(2) << totalVolume << endl;
    }
    
    // Const methods
    void displayTransaction() const {
        cout << txnId << " | " << type << " " << amount << " " << symbol 
             << " @ $" << fixed << setprecision(2) << price << endl;
    }
    
    string getSymbol() const { return symbol; }
    double getAmount() const { return amount; }
    double getPrice() const { return price; }
};

// Initialize static members
int Transaction::totalTransactions = 0;
double Transaction::totalVolume = 0.0;

// Simple trading engine for composition
class TradingEngine {
private:
    double availableCash;
    
public:
    TradingEngine(double initialCash = 10000.0) {
        availableCash = initialCash;
    }
    
    bool buyOrder(double amount, double price) {
        double totalCost = amount * price;
        if (availableCash >= totalCost) {
            availableCash -= totalCost;
            cout << "Buy executed: " << amount << " units @ $" << price << endl;
            return true;
        }
        cout << "Insufficient funds!" << endl;
        return false;
    }
    
    void sellOrder(double amount, double price) {
        availableCash += (amount * price);
        cout << "Sell executed: " << amount << " units @ $" << price << endl;
    }
    
    double getCash() const { return availableCash; }
};

// Simple holding structure
struct Holding {
    Cryptocurrency* crypto;
    double amount;
    double avgPrice;
    
    Holding() {
        crypto = nullptr;
        amount = 0.0;
        avgPrice = 0.0;
    }
};

// Portfolio class demonstrating composition and aggregation
class Portfolio {
private:
    static int portfolioCount;
    
    string portfolioName;
    string portfolioId;
    double initialAmount;
    double currentValue;
    
    // Composition - Portfolio owns trading engine
    TradingEngine* tradingEngine;
    
    // Aggregation - Portfolio has cryptocurrencies
    Holding* holdings;
    int maxHoldings;
    int holdingCount;
    
    // Transaction history
    Transaction* transactions;
    int maxTransactions;
    int transactionCount;
    
public:
    // Multiple constructors
    Portfolio() {
        portfolioCount++;
        portfolioName = "Default Portfolio";
        portfolioId = "PF" + to_string(portfolioCount);
        initialAmount = 10000.0;
        currentValue = 0.0;
        
        tradingEngine = new TradingEngine(initialAmount);
        
        maxHoldings = 5;
        holdings = new Holding[maxHoldings];
        holdingCount = 0;
        
        maxTransactions = 20;
        transactions = new Transaction[maxTransactions];
        transactionCount = 0;
    }
    
    Portfolio(string name, double amount) {
        portfolioCount++;
        portfolioName = name;
        portfolioId = "PF" + to_string(portfolioCount);
        initialAmount = amount;
        currentValue = 0.0;
        
        tradingEngine = new TradingEngine(amount);
        
        maxHoldings = 5;
        holdings = new Holding[maxHoldings];
        holdingCount = 0;
        
        maxTransactions = 20;
        transactions = new Transaction[maxTransactions];
        transactionCount = 0;
    }
    
    // Copy constructor
    Portfolio(const Portfolio& other) {
        portfolioCount++;
        portfolioName = other.portfolioName + " Copy";
        portfolioId = "PF" + to_string(portfolioCount);
        initialAmount = other.initialAmount;
        currentValue = other.currentValue;
        
        tradingEngine = new TradingEngine(initialAmount);
        
        maxHoldings = other.maxHoldings;
        holdings = new Holding[maxHoldings];
        holdingCount = other.holdingCount;
        
        maxTransactions = 20;
        transactions = new Transaction[maxTransactions];
        transactionCount = 0;
    }
    
    ~Portfolio() {
        delete tradingEngine;
        delete[] holdings;
        delete[] transactions;
    }
    
    // Static method
    static int getTotalPortfolios() { return portfolioCount; }
    
    // Const methods
    string getName() const { return portfolioName; }
    double getCurrentValue() const { return currentValue; }
    double getCash() const { return tradingEngine->getCash(); }
    
    bool buyCrypto(Cryptocurrency* crypto, double amount) {
        if (crypto == nullptr || amount <= 0) return false;
        
        double price = crypto->getPrice();
        if (tradingEngine->buyOrder(amount, price)) {
            // Find existing holding or create new
            int index = -1;
            for (int i = 0; i < holdingCount; i++) {
                if (holdings[i].crypto == crypto) {
                    index = i;
                    break;
                }
            }
            
            if (index >= 0) {
                // Update existing holding
                double oldTotal = holdings[index].amount * holdings[index].avgPrice;
                double newTotal = amount * price;
                holdings[index].amount += amount;
                holdings[index].avgPrice = (oldTotal + newTotal) / holdings[index].amount;
            } else if (holdingCount < maxHoldings) {
                // Create new holding
                holdings[holdingCount].crypto = crypto;
                holdings[holdingCount].amount = amount;
                holdings[holdingCount].avgPrice = price;
                holdingCount++;
            }
            
            // Record transaction
            if (transactionCount < maxTransactions) {
                transactions[transactionCount] = Transaction(crypto->getSymbol(), amount, price, "BUY");
                transactionCount++;
            }
            
            updateValue();
            return true;
        }
        return false;
    }
    
    bool sellCrypto(string symbol, double amount) {
        int index = -1;
        for (int i = 0; i < holdingCount; i++) {
            if (holdings[i].crypto->getSymbol() == symbol) {
                index = i;
                break;
            }
        }
        
        if (index < 0 || holdings[index].amount < amount) {
            cout << "Insufficient " << symbol << " to sell!" << endl;
            return false;
        }
        
        double price = holdings[index].crypto->getPrice();
        tradingEngine->sellOrder(amount, price);
        holdings[index].amount -= amount;
        
        // Record transaction
        if (transactionCount < maxTransactions) {
            transactions[transactionCount] = Transaction(symbol, amount, price, "SELL");
            transactionCount++;
        }
        
        updateValue();
        return true;
    }
    
    void updateValue() {
        currentValue = tradingEngine->getCash();
        for (int i = 0; i < holdingCount; i++) {
            currentValue += holdings[i].amount * holdings[i].crypto->getPrice();
        }
    }
    
    void showSummary() const {
        cout << "\n=== " << portfolioName << " ===" << endl;
        cout << "ID: " << portfolioId << endl;
        cout << "Initial: $" << fixed << setprecision(2) << initialAmount << endl;
        cout << "Current: $" << currentValue << endl;
        cout << "Cash: $" << tradingEngine->getCash() << endl;
        cout << "P&L: $" << (currentValue - initialAmount) << endl;
        cout << "Holdings: " << holdingCount << endl;
    }
    
    void showHoldings() const {
        cout << "\n--- Holdings ---" << endl;
        for (int i = 0; i < holdingCount; i++) {
            cout << holdings[i].crypto->getName() << ": " << holdings[i].amount;
            cout << " @ $" << fixed << setprecision(2) << holdings[i].avgPrice << endl;
        }
    }
    
    void showTransactions() const {
        cout << "\n--- Recent Transactions ---" << endl;
        for (int i = transactionCount - 1; i >= 0 && i >= transactionCount - 5; i--) {
            transactions[i].displayTransaction();
        }
    }
    
    // Operator overloading
    bool operator>(const Portfolio& other) const {
        return currentValue > other.currentValue;
    }
    
    Portfolio& operator=(const Portfolio& other) {
        if (this != &other) {
            portfolioName = other.portfolioName;
            initialAmount = other.initialAmount;
            currentValue = other.currentValue;
        }
        return *this;
    }
};

// Initialize static member
int Portfolio::portfolioCount = 0;

// Main Application Class
class CryptoTracker {
private:
    Portfolio* userPortfolio;
    Bitcoin* bitcoin;
    Ethereum* ethereum;
    Altcoin* altcoin;
    bool appRunning;
    
public:
    CryptoTracker() {
        userPortfolio = nullptr;
        bitcoin = new Bitcoin(45000.0);
        ethereum = new Ethereum(3200.0);
        altcoin = new Altcoin("ADA", "Cardano", 0.45, 15);
        appRunning = true;
        srand(time(0));
    }
    
    ~CryptoTracker() {
        delete userPortfolio;
        delete bitcoin;
        delete ethereum;
        delete altcoin;
    }
    
    void showMenu() {
        cout << "\n=== CRYPTO PORTFOLIO TRACKER ===" << endl;
        cout << "1. Create Portfolio" << endl;
        cout << "2. View Portfolio" << endl;
        cout << "3. View Market Prices" << endl;
        cout << "4. Buy Cryptocurrency" << endl;
        cout << "5. Sell Cryptocurrency" << endl;
        cout << "6. Simulate Market Movement" << endl;
        cout << "7. Global Statistics" << endl;
        cout << "8. Auto-Trade Demo (5 rounds)" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
    }
    
    void createPortfolio() {
        if (userPortfolio != nullptr) {
            delete userPortfolio;
        }
        
        string name;
        double amount;
        cout << "Portfolio name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Initial amount: $";
        cin >> amount;
        
        userPortfolio = new Portfolio(name, amount);
        cout << "Portfolio created successfully!" << endl;
    }
    
    void viewPortfolio() {
        if (userPortfolio == nullptr) {
            cout << "Create a portfolio first!" << endl;
            return;
        }
        
        userPortfolio->showSummary();
        userPortfolio->showHoldings();
        userPortfolio->showTransactions();
    }
    
    void viewMarket() {
        cout << "\n=== CURRENT MARKET PRICES ===" << endl;
        bitcoin->displayInfo();
        ethereum->displayInfo();
        altcoin->displayInfo();
        
        cout << "\nVolatility Levels:" << endl;
        cout << "Bitcoin: " << bitcoin->calculateVolatility() << "%" << endl;
        cout << "Ethereum: " << ethereum->calculateVolatility() << "%" << endl;
        cout << "Cardano: " << altcoin->calculateVolatility() << "%" << endl;
    }
    
    void buyCrypto() {
        if (userPortfolio == nullptr) {
            cout << "Create a portfolio first!" << endl;
            return;
        }
        
        cout << "\nSelect Crypto:" << endl;
        cout << "1. Bitcoin" << endl;
        cout << "2. Ethereum" << endl;
        cout << "3. Cardano" << endl;
        cout << "Choice: ";
        
        int choice;
        cin >> choice;
        
        Cryptocurrency* selected = nullptr;
        switch (choice) {
            case 1: selected = bitcoin; break;
            case 2: selected = ethereum; break;
            case 3: selected = altcoin; break;
            default: cout << "Invalid choice!" << endl; return;
        }
        
        cout << "Amount to buy: ";
        double amount;
        cin >> amount;
        
        userPortfolio->buyCrypto(selected, amount);
    }
    
    void sellCrypto() {
        if (userPortfolio == nullptr) {
            cout << "Create a portfolio first!" << endl;
            return;
        }
        
        cout << "Crypto symbol (BTC/ETH/ADA): ";
        string symbol;
        cin >> symbol;
        cout << "Amount to sell: ";
        double amount;
        cin >> amount;
        
        userPortfolio->sellCrypto(symbol, amount);
    }
    
    void updatePrices() {
        cout << "\n=== MARKET UPDATE ===" << endl;
        cout << "Simulating market movement..." << endl;
        
        bitcoin->updatePrice();
        ethereum->updatePrice();
        altcoin->updatePrice();
        
        if (userPortfolio != nullptr) {
            double oldValue = userPortfolio->getCurrentValue();
            userPortfolio->updateValue();
            double newValue = userPortfolio->getCurrentValue();
            double change = newValue - oldValue;
            
            cout << "\nPortfolio Impact:" << endl;
            cout << "Previous Value: $" << fixed << setprecision(2) << oldValue << endl;
            cout << "Current Value: $" << newValue << endl;
            if (change > 0) {
                cout << "Gain: +$" << change << " (+" << ((change/oldValue)*100) << "%)" << endl;
            } else if (change < 0) {
                cout << "Loss: $" << change << " (" << ((change/oldValue)*100) << "%)" << endl;
            } else {
                cout << "No change in portfolio value" << endl;
            }
        }
        
        cout << "\nMarket update completed!" << endl;
    }
    
    void showGlobalStats() {
        cout << "\n=== GLOBAL STATISTICS ===" << endl;
        Transaction::showGlobalStats();
        cout << "Total Portfolios: " << Portfolio::getTotalPortfolios() << endl;
    }
    
    void autoTradeDemo() {
        if (userPortfolio == nullptr) {
            cout << "Create a portfolio first!" << endl;
            return;
        }
        
        cout << "\n=== AUTO-TRADE DEMO ===" << endl;
        cout << "Simulating 5 market rounds with automatic trading..." << endl;
        
        for (int round = 1; round <= 5; round++) {
            cout << "\n--- Round " << round << " ---" << endl;
            
            // Update market prices
            bitcoin->updatePrice();
            ethereum->updatePrice();
            altcoin->updatePrice();
            
            // Simulate some automatic trading
            if (round % 2 == 1) {
                // Buy something randomly
                int cryptoChoice = rand() % 3 + 1;
                double amount = 0.1 + (rand() % 50) / 100.0; // 0.1 to 0.5
                
                Cryptocurrency* selected = nullptr;
                switch (cryptoChoice) {
                    case 1: selected = bitcoin; break;
                    case 2: selected = ethereum; break;
                    case 3: selected = altcoin; break;
                }
                
                cout << "Auto-buying " << amount << " " << selected->getSymbol() << endl;
                userPortfolio->buyCrypto(selected, amount);
            }
            
            userPortfolio->updateValue();
            cout << "Portfolio value: $" << fixed << setprecision(2) 
                 << userPortfolio->getCurrentValue() << endl;
            
            // Small delay simulation
            cout << "Processing..." << endl;
        }
        
        cout << "\nAuto-trade demo completed!" << endl;
        userPortfolio->showSummary();
    }
    
    void run() {
        cout << "Welcome to Crypto Portfolio Tracker!" << endl;
        
        while (appRunning) {
            showMenu();
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1: createPortfolio(); break;
                case 2: viewPortfolio(); break;
                case 3: viewMarket(); break;
                case 4: buyCrypto(); break;
                case 5: sellCrypto(); break;
                case 6: updatePrices(); break;
                case 7: showGlobalStats(); break;
                case 8: autoTradeDemo(); break;
                case 0: 
                    cout << "Goodbye!" << endl;
                    appRunning = false;
                    break;
                default: 
                    cout << "Invalid choice!" << endl;
            }
            
            if (appRunning) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
        }
    }
};

// Main function
int main() {
    CryptoTracker app;
    app.run();
    return 0;
}