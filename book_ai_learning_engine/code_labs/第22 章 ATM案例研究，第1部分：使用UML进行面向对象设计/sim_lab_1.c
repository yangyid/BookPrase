#include <iostream>
#include <memory>
#include <vector>

// 前置声明
class Screen;
class Keypad;
class CashDispenser;
class DepositSlot;
class BankDatabase;
class Account;
class Transaction;

// ---------- 硬件模拟 ----------
class Screen {
public:
    void displayMessage(const std::string& msg) const {
        std::cout << "[Screen] " << msg << std::endl;
    }
};

class Keypad {
public:
    int getInput() const { return 42; } // 模拟输入
};

class CashDispenser {
public:
    void dispenseCash(int amount) {
        std::cout << "[CashDispenser] Dispensing $" << amount << std::endl;
    }
};

class DepositSlot {
public:
    bool isEnvelopeReceived() const { return true; }
};

// ---------- 业务逻辑 ----------
class Account {
    int accountNumber;
    int pin;
    double balance;
public:
    Account(int accNo, int p, double b) : accountNumber(accNo), pin(p), balance(b) {}
    bool validatePIN(int p) const { return pin == p; }
    double getBalance() const { return balance; }
    void credit(double amount) { balance += amount; }
    void debit(double amount) { balance -= amount; }
    int getAccountNumber() const { return accountNumber; }
};

class BankDatabase {
    std::vector<Account> accounts;
public:
    BankDatabase() {
        // 预置账户
        accounts.emplace_back(12345, 54321, 1000.0);
        accounts.emplace_back(98765, 56789, 500.0);
    }
    Account* getAccount(int accNo) {
        for (auto& acc : accounts)
            if (acc.getAccountNumber() == accNo)
                return &acc;
        return nullptr;
    }
};

// ---------- 交易抽象 ----------
class Transaction {
protected:
    int accountNumber;
    Screen& screen;
    BankDatabase& bankDB;
public:
    Transaction(int accNo, Screen& scr, BankDatabase& db)
        : accountNumber(accNo), screen(scr), bankDB(db) {}
    virtual ~Transaction() = default;
    virtual void execute() = 0; // 策略模式的抽象操作
};

class Withdrawal : public Transaction {
    int amount;
    CashDispenser& dispenser;
    Keypad& keypad;
public:
    Withdrawal(int accNo, Screen& scr, BankDatabase& db,
               CashDispenser& cd, Keypad& kp)
        : Transaction(accNo, scr, db), amount(0), dispenser(cd), keypad(kp) {}
    void execute() override {
        screen.displayMessage("Enter amount to withdraw: ");
        amount = keypad.getInput(); // 简化
        Account* acc = bankDB.getAccount(accountNumber);
        if (acc && acc->getBalance() >= amount) {
            dispenser.dispenseCash(amount);
            acc->debit(amount);
            screen.displayMessage("Please take your cash.");
        } else {
            screen.displayMessage("Insufficient funds.");
        }
    }
};

// ---------- ATM 组合所有部件 ----------
class ATM {
    bool userAuthenticated;
    int currentAccountNumber;
    Screen screen;
    Keypad keypad;
    CashDispenser cashDispenser;
    DepositSlot depositSlot;
    BankDatabase bankDB;
public:
    ATM() : userAuthenticated(false), currentAccountNumber(0) {}
    void run() {
        screen.displayMessage("Welcome!");
        // 简化的验证流程
        authenticateUser();
        if (userAuthenticated) {
            // 执行取款交易（示例）
            Withdrawal w(currentAccountNumber, screen, bankDB, cashDispenser, keypad);
            w.execute();
        }
        screen.displayMessage("Goodbye!");
    }
private:
    void authenticateUser() {
        screen.displayMessage("Enter account number: ");
        int accNo = keypad.getInput(); // 模拟
        screen.displayMessage("Enter PIN: ");
        int pin = keypad.getInput();
        Account* acc = bankDB.getAccount(accNo);
        if (acc && acc->validatePIN(pin)) {
            userAuthenticated = true;
            currentAccountNumber = accNo;
            screen.displayMessage("Authentication successful.");
        } else {
            screen.displayMessage("Invalid account or PIN.");
        }
    }
};

int main() {
    ATM atm;
    atm.run();
    return 0;
}