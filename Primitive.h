#pragma once
#include <memory>
#include <vector>
#include "BasicTypes.h"

/**
 * @class Primitive
 * @brief Базовый класс для всех примитивных фигур. Является model-class'ом
 */
class Primitive {
public:
     /**
     * @brief Конструктор примитива.
     * @param Points Точки, определяющие фигуру.
     */
    Primitive(std::vector<std::shared_ptr<Point>>& Points) : Points(Points) {}
    /**
     * @brief Получить точки фигуры.
     * @return Вектор точек.
     */
    std::vector<std::shared_ptr<Point>> GetPoints() { return Points; }
protected:
    std::vector<std::shared_ptr<Point>> Points;
};

/**
 * @class Circle
 * @brief Класс фигуры "Круг".
 */
class Circle : public Primitive {
public:
    /**
     * @brief Конструктор круга.
     * @param Points Точки центра круга.
     * @param Diameter Диаметр круга.
     */
    Circle(std::vector<std::shared_ptr<Point>>& Points, double Diameter) : Primitive(Points), Diameter(Diameter) {}
private:
    double Diameter;
};

/**
 * @class Square
 * @brief Класс фигуры "Квадрат".
 */
class Square : public Primitive {
    public:
         /**
         * @brief Конструктор квадрата.
         * @param Points Точки для построения квадрата.
         * @param Width Ширина квадрата.
         */
        Square(std::vector<std::shared_ptr<Point>>& Points, double Width) : Primitive(Points), Width(Width) {}
    private:
        double Width;
    };
    
/**
 * @class Rectangle
 * @brief Класс фигуры "Прямоугольник".
 */
class Rectangle : public Square {
public:
    /**
     * @brief Конструктор прямоугольника.
     * @param Points Точки для построения прямоугольника.
     * @param Width Ширина.
     * @param Height Высота.
     */
    Rectangle(std::vector<std::shared_ptr<Point>>& Points, double Width, double Height) : Square(Points, Width), Height(Height) {}
private:
    double Width;
    double Height;
};
