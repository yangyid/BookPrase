#include <iostream>
#include <vector>
#include <memory>

// 抽象基类
class Shape {
public:
    virtual ~Shape() = default; // 虚析构函数
    virtual void draw() const = 0; // 纯虚函数
    virtual double area() const { return 0.0; } // 普通虚函数，默认实现
};

// 派生类 Circle，使用 override
class Circle final : public Shape { // final 禁止再派生
    double radius;
public:
    explicit Circle(double r) : radius(r) {}
    void draw() const override {
        std::cout << "Drawing a circle with radius " << radius << std::endl;
    }
    double area() const override {
        return 3.14159 * radius * radius;
    }
};

// 派生类 Rectangle，非 final
class Rectangle : public Shape {
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    void draw() const override {
        std::cout << "Drawing a rectangle " << width << "x" << height << std::endl;
    }
    double area() const override {
        return width * height;
    }
};

// 多态测试：接受基类引用
void render(const Shape& s) {
    s.draw();
    std::cout << "Area: " << s.area() << std::endl;
}

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(3.0));
    shapes.push_back(std::make_unique<Rectangle>(4.0, 5.0));

    for (const auto& sp : shapes) {
        render(*sp);
        
        // 运行时类型识别与安全的向下转型
        if (auto* rect = dynamic_cast<Rectangle*>(sp.get())) {
            std::cout << "  (This is a Rectangle)" << std::endl;
        } else if (auto* circ = dynamic_cast<Circle*>(sp.get())) {
            std::cout << "  (This is a Circle, final class)" << std::endl;
        }
        std::cout << std::endl;
    }

    // typeid 示例
    const auto& shape1 = *shapes[0];
    const auto& shape2 = *shapes[1];
    std::cout << "typeid of shape1: " << typeid(shape1).name() << std::endl;
    std::cout << "typeid of shape2: " << typeid(shape2).name() << std::endl;
    if (typeid(shape1) != typeid(shape2))
        std::cout << "Different dynamic types." << std::endl;

    return 0;
}