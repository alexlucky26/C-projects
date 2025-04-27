#pragma once
#include <memory>
#include <vector>
#include "BasicTypes.h"

class Primitive {
public:
    Primitive(std::vector<std::shared_ptr<Point>>& Points) : Points(Points) {};
    std::vector<std::shared_ptr<Point>> GetPoints() { return Points; };
protected:
    std::vector<std::shared_ptr<Point>> Points;
};

class Circle : public Primitive {
public:
    Circle(std::vector<std::shared_ptr<Point>>& Points, double Diameter) : Primitive(Points), Diameter(Diameter) {};
private:
    double Diameter;
};

class Square : public Primitive {
    public:
        Square(std::vector<std::shared_ptr<Point>>& Points, double Width) : Primitive(Points), Width(Width) {};
    private:
        double Width;
    };
    

class Rectangle : public Square {
public:
    Rectangle(std::vector<std::shared_ptr<Point>>& Points, double Width, double Height) : Square(Points, Width), Height(Height) {};
private:
    double Width;
    double Height;
};
