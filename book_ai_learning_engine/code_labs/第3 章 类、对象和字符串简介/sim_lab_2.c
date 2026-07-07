class Point {
       int x, y;
     public:
       Point(int x, int y) : x(x), y(y) {}
       void display() const { std::cout << "(" << x << "," << y << ")\n"; }
     };