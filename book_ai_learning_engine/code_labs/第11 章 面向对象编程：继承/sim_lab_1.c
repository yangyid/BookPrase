#include <iostream>
#include <string>
#include <memory>

// 基类：几何形状
class Shape {
public:
    // 构造函数
    explicit Shape(const std::string& name) : name_(name) {
        std::cout << "[Shape] Constructor: " << name_ << std::endl;
    }

    // 虚析构函数 （确保派生类正确析构）
    virtual ~Shape() {
        std::cout << "[Shape] Destructor: " << name_ << std::endl;
    }

    // 普通成员函数（非虚）
    void draw() const {
        std::cout << "Drawing shape: " << name_ << std::endl;
    }

protected:
    std::string name_;  // 受保护的成员，可被派生类直接访问
};

// 派生类：圆形
class Circle : public Shape {  // public 继承
public:
    Circle(double radius, const std::string& name = "Circle")
        : Shape(name),           // 显式调用基类构造函数
          radius_(radius) {
        std::cout << "[Circle] Constructor: radius = " << radius_ << std::endl;
    }

    ~Circle() override {
        std::cout << "[Circle] Destructor" << std::endl;
    }

    // 重定义基类 draw() ，隐藏了基类的 draw()
    void draw() const {
        // 调用基类版本
        Shape::draw();
        std::cout << "  -> Circle specific: radius = " << radius_ << std::endl;
    }

    double area() const {
        return 3.14159 * radius_ * radius_;
    }

private:
    double radius_;
};

// 测试
int main() {
    std::cout << "=== Creating derived object ===" << std::endl;
    Circle circle(5.0);
    circle.draw();
    std::cout << "Area: " << circle.area() << "\n\n";

    std::cout << "=== Access via base pointer (non-polymorphic) ===" << std::endl;
    Shape* sptr = &circle;
    sptr->draw();  // 调用 Shape::draw()，无多态

    std::cout << "\n=== Smart pointer with polymorphism ===" << std::endl;
    std::unique_ptr<Shape> polyPtr(new Circle(3.0, "Dynamic Circle"));
    polyPtr->draw(); // 仍然调用 Shape::draw()，除非 draw 是 virtual
    // 自动触发正确析构（基类析构为 virtual）

    return 0;
}