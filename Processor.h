#pragma once
#include <memory>
#include "Primitive.h"
#include "views/FileDialog.h"
#include "views/Viewport.h"

using namespace std;

// That's a controller class that creates views
class Processor {
    unique_ptr<Viewport> mainView;
    shared_ptr<Primitive> MatchUserSelectedPointToPrimitiveObject(shared_ptr<Point> pPoint);
    vector<shared_ptr<Primitive>> primitives; // all primitives that are drawn on Viewport

    FileHandle CreateSelectFileDialog(); // return selected file ptr
    FileHandle CreateSaveFileDialog(); // anyFormat - save the file to any format or into specific Editor format. Returns exported file
    void CallInfoDialog(); // saved/exported/imported/opened
public:
    Processor();
    ~Processor();
    // export/import dialog as a view
    void ImportFile_Handler();
	void ExportFile_Handler();
    void NewDocument_Handler();

    // main view
    void CreatePrimitive_Handler(PrimitiveType primitiveType = PrimitiveType::Rectangle); // creates a primitive class of the type
    void DeletePrimitive_Handler();
};

