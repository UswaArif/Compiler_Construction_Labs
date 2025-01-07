#include <iostream>

using namespace std;

class Shape 
{
    public:
        virtual double getArea() = 0; 
};

class Circle : public Shape
{
    public:
        double radius;

        Circle(double radius) {
            this->radius = radius;
        }

        double getArea() override {
            return 3.14 * radius * radius; 
        }
};

main() 
{
    Shape* shape = new Circle(4);
    cout << "Area: " << shape->getArea() << endl;
}