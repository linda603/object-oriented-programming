#include <iostream>

class Rectangle {
private:
    int width, height;

public:
    Rectangle() {}
    Rectangle(int x, int y): width(x), height(y) {}

    int getArea() { return width * height; }
    friend Rectangle duplicate(const Rectangle& param);
};

Rectangle duplicate (const Rectangle& param) {
    Rectangle rectangle;
    rectangle.width = param.width * 2;
    rectangle.height = param.height * 2;
    return rectangle;
};

int main() {
    Rectangle foo;
    Rectangle bar(2, 3);
    foo = duplicate(bar);
    std::cout << "Foo area: " << foo.getArea() << std::endl;
    return 0;
}