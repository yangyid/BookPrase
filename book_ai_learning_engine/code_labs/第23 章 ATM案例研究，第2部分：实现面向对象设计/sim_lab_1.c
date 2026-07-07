// atm.cpp - 完整ATM案例实现 (C++11标准)
#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

//--------------------- 硬件模拟组件 ---------------------
class Screen {
public:
    void displayMessage(const string& msg) { cout << msg << endl; }
    void displayDollarAmount(double amt) { cout << "$" << amt << endl; }
};

class Keypad {
public:
    int getInput() const { 
        int value;
        cin >> value;
        return value;
    }
};

class CashDispenser {
    int totalCash = 10000;
public:
    bool dispenseCash(double amount) {
        if (amount > totalCash) return false;
        totalCash -= static_cast<int>(amount);
        return true;
    }
    bool isSufficientCashAvailable(double amount) { return amount <= totalCash; }
};

class DepositSlot {
public:
    bool isEnvelopeReceived() const { return true; } // 简化
};

class BankDatabase {
    struct Account {
        int accountNumber;
        int pin;
        double availableBalance;
        double totalBalance;
    };
    vector<Account> accounts;
    int currentAccountNumber = -1;
public:
    BankDatabase() {
        accounts.push_back({12345, 54321, 1000.0, 1200.0});
        accounts.push_back({98765, 56789, 200.0, 200.0});
    }
    bool authenticateUser(int account, int pin) {
        for (auto& acc : accounts) {
            if (acc.accountNumber == account && acc.pin == pin) {
                currentAccountNumber = account;
                return true;
            }
        }
        return false;
    }
    double getAvailableBalance(int accNo) const {
        for (auto& acc : accounts) if (acc.accountNumber == accNo) return acc.availableBalance;
        return 0.0;
    }
    void debit(int accNo, double amount) {
        for (auto& acc : accounts) if (acc.accountNumber == accNo) acc.availableBalance -= amount;
    }
    void credit(int accNo, double amount) {
        for (auto& acc : accounts) if (acc.accountNumber == accNo) acc.totalBalance += amount;
    }
    int getCurrentAccountNumber() const { return currentAccountNumber; }
};

class ATM; // 前向声明

//--------------------- 抽象交易基类 ---------------------
class Transaction {
protected:
    int accountNumber;
    Screen& screen;
    BankDatabase& bankDatabase;
    // 模板方法骨架
    virtual void performTransaction() = 0; // 具体步骤由子类实现
public:
    Transaction(int accNo, Screen& scr, BankDatabase& db)
        : accountNumber(accNo), screen(scr), bankDatabase(db) {}
    virtual ~Transaction() = default;

    void execute() { // 非虚接口
        performTransaction();
    }
};

//--------------------- 具体交易类 ---------------------
class BalanceInquiry : public Transaction {
public:
    BalanceInquiry(int accNo, Screen& scr, BankDatabase& db)
        : Transaction(accNo, scr, db) {}
protected:
    virtual void performTransaction() override {
        double bal = bankDatabase.getAvailableBalance(accountNumber);
        screen.displayMessage("可用余额: ");
        screen.displayDollarAmount(bal);
    }
};

class Withdrawal : public Transaction {
    double amount;
    Keypad& keypad;
    CashDispenser& cashDispenser;
public:
    Withdrawal(int accNo, Screen& scr, BankDatabase& db, Keypad& kp, CashDispenser& cd)
        : Transaction(accNo, scr, db), keypad(kp), cashDispenser(cd) {}
protected:
    virtual void performTransaction() override {
        screen.displayMessage("请输入取款金额（20的倍数）:");
        amount = keypad.getInput();
        if (!cashDispenser.isSufficientCashAvailable(amount)) {
            screen.displayMessage("ATM现金不足。");
            return;
        }
        if (amount > bankDatabase.getAvailableBalance(accountNumber)) {
            screen.displayMessage("余额不足。");
            return;
        }
        bankDatabase.debit(accountNumber, amount);
        cashDispenser.dispenseCash(amount);
        screen.displayMessage("请取走您的现金。");
    }
};

class Deposit : public Transaction {
    double amount;
    Keypad& keypad;
    DepositSlot& depositSlot;
public:
    Deposit(int accNo, Screen& scr, BankDatabase& db, Keypad& kp, DepositSlot& ds)
        : Transaction(accNo, scr, db), keypad(kp), depositSlot(ds) {}
protected:
    virtual void performTransaction() override {
        screen.displayMessage("请输入存款信封金额:");
        amount = keypad.getInput();
        if (depositSlot.isEnvelopeReceived()) {
            bankDatabase.credit(accountNumber, amount);
            screen.displayMessage("信封已接收，余额将更新。");
        } else {
            screen.displayMessage("未收到存款信封。");
        }
    }
};

//--------------------- ATM 类（主控制器） ---------------------
class ATM {
    bool userAuthenticated = false;
    int currentAccountNumber = 0;
    Screen screen;
    Keypad keypad;
    CashDispenser cashDispenser;
    DepositSlot depositSlot;
    BankDatabase bankDatabase;

    void authenticateUser() {
        screen.displayMessage("输入账号:");
        int acc = keypad.getInput();
        screen.displayMessage("输入PIN:");
        int pin = keypad.getInput();
        userAuthenticated = bankDatabase.authenticateUser(acc, pin);
        if (userAuthenticated) {
            currentAccountNumber = bankDatabase.getCurrentAccountNumber();
            screen.displayMessage("认证成功。");
        } else {
            screen.displayMessage("账号或PIN错误。");
        }
    }

    void performTransactions() {
        Transaction* currentTransaction = nullptr;
        bool exit = false;
        while (!exit) {
            screen.displayMessage("\n主菜单:\n1 - 余额查询\n2 - 取款\n3 - 存款\n4 - 退出");
            int choice = keypad.getInput();
            switch (choice) {
                case 1:
                    currentTransaction = new BalanceInquiry(currentAccountNumber, screen, bankDatabase);
                    break;
                case 2:
                    currentTransaction = new Withdrawal(currentAccountNumber, screen, bankDatabase, keypad, cashDispenser);
                    break;
                case 3:
                    currentTransaction = new Deposit(currentAccountNumber, screen, bankDatabase, keypad, depositSlot);
                    break;
                case 4:
                    screen.displayMessage("感谢使用。");
                    exit = true;
                    break;
                default:
                    screen.displayMessage("无效选择。");
                    break;
            }
            if (currentTransaction != nullptr) {
                currentTransaction->execute();   // 多态调用
                delete currentTransaction;      // 虚析构保证正确清理
                currentTransaction = nullptr;
            }
        }
    }

public:
    void run() {
        while (true) {
            screen.displayMessage("\n欢迎!");
            authenticateUser();
            if (userAuthenticated) {
                performTransactions();
                userAuthenticated = false;
                currentAccountNumber = 0;
            }
        }
    }
};

//--------------------- 主程序 ---------------------
int main() {
    ATM atm;
    atm.run();
    return 0;
}