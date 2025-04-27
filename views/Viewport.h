#pragma once

//#include <type_traits>
#include <iomanip>
#include <vector>
#include "BasicTypes.h"

using namespace std;
// that's the Workspace that displays figures. It's always available even if there is no figures in the Workspace
// Viewport shouldn't know about types and classes of created figures... created figures for the View are different from figures that are used in Processor (Controller).
// The figures from Viewport's point of view are just points that are drawn
class Viewport {
public:
    void NewDocument(){};

    vector<std::shared_ptr<Point>> SetFigurePositionOnView() { return vector<std::shared_ptr<Point>>(); };
    //template<typename FigureType, std::enable_if_t<FigureType::value1 == PrimitiveType::Circle, int> = 0>
    template<PrimitiveType primType, std::enable_if_t<primType == PrimitiveType::Circle, bool> = true>
    void SetUpPreferencesForFigure([[maybe_unused]] double& diameter){}; //width, height or diameter
    //template<typename FigureType, std::enable_if_t<FigureType::value1 == PrimitiveType::Square, int> = 0>
    template<PrimitiveType primType, std::enable_if_t<primType == PrimitiveType::Square, bool> = true>
    void SetUpPreferencesForFigure([[maybe_unused]] double& width){}; //width, height or diameter
    void SetUpPreferencesForFigure([[maybe_unused]] double& width, [[maybe_unused]]double& height){}; //width, height or diameter
    void DrawFigureByPoints([[maybe_unused]]vector<shared_ptr<Point>> points){}; // called here privatly inside the class, but I left an option to call it outside by a controller
    
    std::shared_ptr<Point> GetUserSelectionOnViewport() { return make_shared<Point>(Point()); };
    void DeleteFigureByPoints([[maybe_unused]] vector<shared_ptr<Point>> points){};
};
