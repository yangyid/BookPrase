#include <iostream>
#include <stdexcept>
#include <string>

class BankAccount {
private:
    std::string owner;
    double balance;
    static int totalAccounts;            // 静态成员声明

public:
    // 构造函数，使用成员初始化列表
    explicit BankAccount(const std::string& ownerName, double initial = 0.0)
        : owner(ownerName), balance(initial) {
        if (balance < 0)
            throw std::invalid_argument("Initial balance cannot be negative");
        ++totalAccounts;
    }

    // const成员函数：只读余额
    double getBalance() const {
        return balance;
    }

    // 非const存款函数
    void deposit(double amount) {
        if (amount <= 0)
            throw std::invalid_argument("Deposit amount must be positive");
        balance += amount;
    }

    // 取款，异常表示金额不足
    void withdraw(double amount) {
        if (amount <= 0)
            throw std::invalid_argument("Withdraw amount must be positive");
        if (amount > balance)
            throw std::runtime_error("Insufficient funds");
        balance -= amount;
    }

    // 静态成员函数
    static int getTotalAccounts() {
        return totalAccounts;
    }

    // 友元函数：打印账户摘要（可访问私有成员）
    friend std::ostream& operator<<(std::ostream& os, const BankAccount& acc);
};

// 静态数据成员类外定义
int BankAccount::totalAccounts = 0;

std::ostream& operator<<(std::ostream& os, const BankAccount& acc) {
    os << "Owner: " << acc.owner << ", Balance: $" << acc.balance;
    return os;
}

int main() {
    try {
        BankAccount alice("Alice", 500.0);
        std::cout << alice << std::endl;               // 使用友元
        alice.deposit(150.0);
        std::cout << alice.getBalance() << std::endl;  // const成员函数
        alice.withdraw(700.0);                         // 将抛出异常
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;  // 预期输出：Insufficient funds
    }
    catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
    }
    std::cout << "Total accounts: " << BankAccount::getTotalAccounts() << std::endl;
    return 0;
}