#include <iostream>
   #include <string>
   
   // 一个简单的银行账户类
   class Account {
   private:  // 数据私有
       std::string owner;
       double balance;
   
   public:   // 接口公有
       // 构造函数：初始化对象
       Account(const std::string& name, double initialDeposit)
           : owner(name), balance(initialDeposit) {  // 成员初始化列表
           if (initialDeposit < 0.0) {
               std::cerr << "初始余额不能为负，设为0。\n";
               balance = 0.0;
           }
       }
   
       // 存款
       void deposit(double amount) {
           if (amount > 0) balance += amount;
       }
   
       // 取款，返回是否成功
       bool withdraw(double amount) {
           if (amount > 0 && balance >= amount) {
               balance -= amount;
               return true;
           }
           return false;
       }
   
       // 查询余额
       double getBalance() const { return balance; }
   
       // 显示账户信息
       void display() const {
           std::cout << "账户持有者: " << owner
                     << "  余额: $" << balance << std::endl;
       }
   };
   
   int main() {
       // 创建对象（自动调用构造函数）
       Account alice("Alice Smith", 500.0);
       alice.display();
   
       alice.deposit(250.0);
       std::cout << "存款后余额: " << alice.getBalance() << std::endl;
   
       if(alice.withdraw(100.0))
           std::cout << "取款成功。\n";
       else
           std::cout << "余额不足。\n";
       alice.display();
   
       // 使用 string 类
       std::string greeting = "Hello, ";
       std::string name = "World";
       std::string message = greeting + name;  // 拼接
       std::cout << message << " 长度: " << message.length() << std::endl;
   
       return 0;
   }