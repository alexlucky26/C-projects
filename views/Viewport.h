#pragma once

//#include <type_traits>
#include <iomanip>
#include <vector>
#include "BasicTypes.h"

using namespace std;
/**
 * @class Viewport
 * @brief Рабочее пространство для отображения фигур - это классический View.
 * @details Viewport отображает только точки фигур, не зная их типы и внутренние данные (это висит на контроллере Pro). Этот Viewport считается основным и существует на протяжении всей программы.
 */
class Viewport {
public:
    /**
     * @brief Создать новый документ (очистить рабочее пространство).
     */
    void NewDocument(){}

    /**
     * @brief Разместить фигуру на Viewport'e в место, выбранное пользователем.
     * @return Вектор точек фигуры.
     */
    vector<std::shared_ptr<Point>> SetFigurePositionOnView() { return vector<std::shared_ptr<Point>>(); }
    /**
     * @brief Установить параметры для создания круга.
     * @param diameter Диаметр круга.
     */
    template<PrimitiveType primType, std::enable_if_t<primType == PrimitiveType::Circle, bool> = true>
    void SetUpPreferencesForFigure([[maybe_unused]] double& diameter){} //width, height or diameter
    /**
     * @brief Установить параметры для создания квадрата.
     * @param width Ширина квадрата.
     */
    template<PrimitiveType primType, std::enable_if_t<primType == PrimitiveType::Square, bool> = true>
    void SetUpPreferencesForFigure([[maybe_unused]] double& width){} //width, height or diameter
    /**
     * @brief Установить параметры для создания прямоугольника.
     * @param width Ширина.
     * @param height Высота.
     */
    void SetUpPreferencesForFigure([[maybe_unused]] double& width, [[maybe_unused]]double& height){} //width, height or diameter
     /**
     * @brief Отобразить фигуру по её точкам.
     * @param points Точки фигуры.
     */
    void DrawFigureByPoints([[maybe_unused]]vector<shared_ptr<Point>> points){} // called here privatly inside the class, but I left an option to call it outside by a controller
    /**
     * @brief Получить точку, выбранную пользователем на рабочем пространстве.
     * @return Выбранная точка.
     */
    std::shared_ptr<Point> GetUserSelectionOnViewport() { return make_shared<Point>(Point()); }
     /**
     * @brief Удалить фигуру по её точкам.
     * @param points Точки фигуры.
     */
    void DeleteFigureByPoints([[maybe_unused]] vector<shared_ptr<Point>> points){}
};
