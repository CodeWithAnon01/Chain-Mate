#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

class Cryptocurrency {
protected:
    string symbol;
    string name;
    double price;
    
public:
    Cryptocurrency(string sym, string nm, double prc) {
        symbol = sym;
        name = nm;
        price = prc;
    }
    
    virtual void displayInfo() const {
        cout << name << " (" << symbol << ") - $" << fixed << setprecision(2) << price << endl;
    }
    
    string getSymbol() const { return symbol; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    
    void updatePrice() {
        double changePercent = ((rand() % 201 - 100) / 100.0) * 0.05;
        double oldPrice = price;
        price += price * changePercent;
        if (price < 0.01) price = 0.01;
        
        cout << name << " (" << symbol << "): $" << oldPrice << " -> $" << price;
        if (changePercent > 0) {
            cout << " (+" << (changePercent * 100) << "%)" << endl;
        } else {
            cout << " (" << (changePercent * 100) << "%)" << endl;
        }
    }
    
    virtual ~Cryptocurrency() {}
};

class Bitcoin : public Cryptocurrency {
public:
    Bitcoin() : Cryptocurrency("BTC", "Bitcoin", 118100.0) {}
    
    void displayInfo() const override {
        cout << "Bitcoin (BTC) - $" << fixed << setprecision(2) << price << " | Most Popular" << endl;
    }
};

class Ethereum : public Cryptocurrency {
public:
    Ethereum() : Cryptocurrency("ETH", "Ethereum", 3760.40) {}
    
    void displayInfo() const override {
        cout << "Ethereum (ETH) - $" << fixed << setprecision(2) << price << " | Smart Contracts" << endl;
    }
};

class Transaction {
private:
    string symbol;
    double amount;
    double price;
    string type;
    
public:
    Transaction() {
        symbol = "NONE";
        amount = 0.0;
        price = 0.0;
        type = "NONE";
    }
    
    Transaction(string sym, double amt, double prc, string tp) {
        symbol = sym;
        amount = amt;
        price = prc;
        type = tp;
    }
    
    void displayTransaction() const {
        cout << type << " " << amount << " " << symbol 
             << " @ $" << fixed << setprecision(2) << price << endl;
    }
};

class Portfolio {
private:
    string name;
    double cash;
    double totalValue;
    Transaction* transactions;
    int transactionCount;
    int maxTransactions;
    
public:
    Portfolio(string nm, double initialCash) {
        name = nm;
        cash = initialCash;
        totalValue = initialCash;
        maxTransactions = 10;
        transactions = new Transaction[maxTransactions];
        transactionCount = 0;
    }
    
    ~Portfolio() {
        delete[] transactions;
    }
    
    bool buyCrypto(Cryptocurrency* crypto, double amount) {
        double cost = amount * crypto->getPrice();
        if (cash >= cost) {
            cash -= cost;
            totalValue = cash;
            
            if (transactionCount < maxTransactions) {
                transactions[transactionCount] = Transaction(crypto->getSymbol(), amount, crypto->getPrice(), "BUY");
                transactionCount++;
            }
            
            cout << "Bought " << amount << " " << crypto->getSymbol() << " for $" << cost << endl;
            return true;
        }
        cout << "Not enough cash!" << endl;
        return false;
    }
    
    void sellCrypto(Cryptocurrency* crypto, double amount) {
        double revenue = amount * crypto->getPrice();
        cash += revenue;
        totalValue = cash;
        
        if (transactionCount < maxTransactions) {
            transactions[transactionCount] = Transaction(crypto->getSymbol(), amount, crypto->getPrice(), "SELL");
            transactionCount++;
        }
        
        cout << "Sold " << amount << " " << crypto->getSymbol() << " for $" << revenue << endl;
    }
    
    void showSummary() const {
        cout << "\n=== " << name << " ===" << endl;
        cout << "Cash: $" << fixed << setprecision(6) << cash << endl;
        cout << "Total Value: $" << totalValue << endl;
    }
    
    void showTransactions() const {
        cout << "\n--- Recent Transactions ---" << endl;
        for (int i = 0; i < transactionCount; i++) {
            transactions[i].displayTransaction();
        }
    }
    
    double getCash() const { return cash; }
    double getTotalValue() const { return totalValue; }
};

class CryptoTracker {
private:
    Portfolio* portfolio;
    Bitcoin* bitcoin;
    Ethereum* ethereum;
    bool running;
    
public:
    CryptoTracker() {
        portfolio = nullptr;
        bitcoin = new Bitcoin();
        ethereum = new Ethereum();
        running = true;
        srand(time(0));
    }
    
    ~CryptoTracker() {
        delete portfolio;
        delete bitcoin;
        delete ethereum;
    }
    
    void showMenu() {
        cout << "\n=== CRYPTO TRACKER ===" << endl;
        cout << "1. Create Portfolio" << endl;
        cout << "2. View Portfolio" << endl;
        cout << "3. View Market" << endl;
        cout << "4. Buy Crypto" << endl;
        cout << "5. Sell Crypto" << endl;
        cout << "6. Update Market" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
    }
    
    void createPortfolio() {
        if (portfolio != nullptr) {
            delete portfolio;
        }
        
        string name;
        double amount;
        cout << "Portfolio name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Initial cash: $";
        cin >> amount;
        
        portfolio = new Portfolio(name, amount);
        cout << "Portfolio created!" << endl;
    }
    
    void viewPortfolio() {
        if (portfolio == nullptr) {
            cout << "Create a portfolio first!" << endl;
            return;
        }
        
        portfolio->showSummary();
        portfolio->showTransactions();
    }
    
    void viewMarket() {
        cout << "\n=== MARKET PRICES ===" << endl;
        bitcoin->displayInfo();
        ethereum->displayInfo();
    }
    
    void buyCrypto() {
        if (portfolio == nullptr) {
            cout << "Create a portfolio first!" << endl;
            return;
        }
        
        cout << "\nSelect Crypto:" << endl;
        cout << "1. Bitcoin" << endl;
        cout << "2. Ethereum" << endl;
        cout << "Choice: ";
        
        int choice;
        cin >> choice;
        
        Cryptocurrency* selected = nullptr;
        switch (choice) {
            case 1: selected = bitcoin; break;
            case 2: selected = ethereum; break;
            default: cout << "Invalid choice!" << endl; return;
        }
        
        cout << "Amount to buy: ";
        double amount;
        cin >> amount;
        
        portfolio->buyCrypto(selected, amount);
    }
    
    void sellCrypto() {
        if (portfolio == nullptr) {
            cout << "Create a portfolio first!" << endl;
            return;
        }
        
        cout << "\nSelect Crypto:" << endl;
        cout << "1. Bitcoin" << endl;
        cout << "2. Ethereum" << endl;
        cout << "Choice: ";
        
        int choice;
        cin >> choice;
        
        Cryptocurrency* selected = nullptr;
        switch (choice) {
            case 1: selected = bitcoin; break;
            case 2: selected = ethereum; break;
            default: cout << "Invalid choice!" << endl; return;
        }
        
        cout << "Amount to sell: ";
        double amount;
        cin >> amount;
        
        portfolio->sellCrypto(selected, amount);
    }
    
    void updateMarket() {
        cout << "\n=== MARKET UPDATE ===" << endl;
        bitcoin->updatePrice();
        ethereum->updatePrice();
        cout << "Market updated!" << endl;
    }
    
    void run() {
        cout << "Welcome to Chain-Mate!" << endl;
        
        while (running) {
            showMenu();
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1: createPortfolio(); break;
                case 2: viewPortfolio(); break;
                case 3: viewMarket(); break;
                case 4: buyCrypto(); break;
                case 5: sellCrypto(); break;
                case 6: updateMarket(); break;
                case 0: 
                    cout << "Goodbye!" << endl;
                    running = false;
                    break;
                default: 
                    cout << "Invalid choice!" << endl;
            }
            
            if (running) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
        }
    }
};

int main() {
    CryptoTracker app;
    app.run();
    return 0;
}
