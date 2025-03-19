#include <iostream>

class Square;

class Rectangle {
private:
    int width, height;

public:
    Rectangle() {}

    int getArea() { return width * height; }
    void convert(Square a);
};

class Square {
friend class Rectangle;
private:
    int side;

public:
    Square(int a) : side(a) {}
};

void Rectangle::convert(Square a) {
    width = a.side;
    height = a.side;
}

int main() {
    Rectangle rect;
    Square square(2);
    rect.convert(square);
    std::cout << "Rectangle area: " << rect.getArea() << std::endl;
    return 0;
}