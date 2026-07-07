【章节知识点概述】  
本章通过 ATM 案例演示如何运用 UML 进行面向对象分析与设计。重点包括用例建模、类图、序列图与活动图，以及从设计到 C++ 头文件的映射。设计核心采用策略模式封装交易类型，并通过组合与依赖关系连接 ATM 各组件。后续章节将基于此设计实现完整 C++ 代码。

---

1. **核心理论断言**  
- **断言**：先设计，后编码。用 UML 可视化需求与结构，能显著降低复杂软件的错误率。  
- **白话类比**：盖房子先画图纸，确认房间、管道布局再施工；UML 就是软件的“建筑图纸”。  
- **大师避坑指南**：避免过早编码。不要在未澄清类关系时就开始写实现，否则继承/组合误用将导致“屎山”。请牢记：`is-a` 用继承，`has-a` 用组合；依赖应尽量指向接口而非具体类。

2. **仿真代码示例**  
以下代码依据 UML 类图实现核心类的骨架，展示继承、关联、组合关系。可完整编译运行，输出各组件创建信息。

```cpp
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
```

3. **内核/系统验证实验**  
使用 Linux 工具观察多态与对象布局：  
```bash
# 编译保留调试符号
g++ -std=c++11 -g -O0 atm_demo.cpp -o atm_demo

# 查看虚函数表
objdump -t atm_demo | grep _ZTV    # 显示 vtable 符号
readelf -s atm_demo | grep VTT

# GDB 中断查看 this 指针与虚表
gdb ./atm_demo
(gdb) b Withdrawal::execute
(gdb) r
(gdb) p *this
(gdb) info vtbl this   # 查看虚表（需-ggdb3和g++）
```
通过上述实验，可以观察到 `Transaction` 的纯虚函数如何在派生类中覆盖，验证 UML 中继承与多态关系在二进制层面的实现。

4. **理论-实践映射表**  

| UML 概念          | C++ 实现元素                     | 示例说明                         |
|-------------------|----------------------------------|----------------------------------|
| 类（Class）       | class / struct                   | `Account`, `Screen`              |
| 继承（△）         | `: public Base`                  | `Withdrawal : Transaction`       |
| 组合（◆）         | 成员对象（值语义）               | `ATM` 内含 `Screen`, `Keypad` 等 |
| 单向关联          | 指针/引用成员                    | `Transaction` 持有 `Screen&`     |
| 依赖（虚线箭头）  | 函数参数中短暂使用               | `Withdrawal` 通过 `BankDatabase&` 调用 |
| 抽象类/接口       | 含纯虚函数的类                   | `Transaction`                    |
| 策略模式          | 抽象基类 + 具体策略类            | `Transaction` 为策略，具体交易类为具体策略 |

5. **可深挖的知识点与学习链接**  
- **UML 2.5 规范**：类图、序列图、用例图语法细节 → [OMG UML](https://www.omg.org/spec/UML/)  
- **设计模式**：策略模式、模板方法模式在 ATM 中的体现 → 《Head First 设计模式》、[refactoring.guru](https://refactoring.guru/design-patterns/strategy)  
- **C++ 面向对象最佳实践**：虚析构函数、纯虚接口、组合优于继承 → *C++ Core Guidelines* [C.120~C.133](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-interfaces)  
- **用例驱动开发**：Ivar Jacobson 的《面向对象软件工程》  

6. **代码练习任务**  
- **任务一**：根据 UML 添加 `Deposit` 类（继承 `Transaction`），并要求输入存款金额，通过 `DepositSlot` 检测信封，更新账户余额。  
- **任务二**：重构 `ATM::run()` 使用菜单循环，允许用户多次选择交易类型（取款、存款、余额查询），并实现完整的身份验证与退出逻辑。  
- **任务三**：引入日志类 `Logger`，使用组合方式添加到 `ATM`，记录所有交易操作及时间戳，并确保日志对象通过依赖注入传给 `Transaction`。  

> 提示：完成练习时，先画出修改后的 UML 类图，再编写代码，体会设计先行的优势。