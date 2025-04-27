#include "Processor.h"

Processor::Processor(){
    mainView = make_unique<Viewport>();
};

Processor::~Processor(){
    primitives.clear();
};

FileHandle Processor::CreateSelectFileDialog(){
    shared_ptr<ImportDialog> dialogView = make_shared<ImportDialog>();
    dialogView->show();
    dialogView->hide();
    return dialogView->ImportFile();
};

FileHandle Processor::CreateSaveFileDialog(){
    shared_ptr<ExportDialog> dialogView = make_shared<ExportDialog>();
    dialogView->show();
    dialogView->hide();
    return dialogView->SaveFile();
};

void Processor::CallInfoDialog(){
    shared_ptr<InfoDialog> dialogView = make_shared<InfoDialog>();
    dialogView->show();
    dialogView->hide();
};

void Processor::ImportFile_Handler(){
    FileHandle fh = CreateSaveFileDialog();
};

void Processor::ExportFile_Handler(){
    FileHandle fh = CreateSelectFileDialog();
};

void Processor::NewDocument_Handler(){
    mainView->NewDocument();
};

void Processor::CreatePrimitive_Handler(PrimitiveType primitiveType) {
    shared_ptr<Primitive> primitive;
    auto points = mainView->SetFigurePositionOnView();
    switch (primitiveType) {
        case PrimitiveType::Circle:
            double diameter;
            mainView->SetUpPreferencesForFigure<PrimitiveType::Circle>(diameter);
            primitive = make_shared<Circle>(Circle(points, diameter));
            break;
        case PrimitiveType::Square:
            double squareWidth;
            mainView->SetUpPreferencesForFigure<PrimitiveType::Square>(squareWidth);
            primitive = make_shared<Square>(Square(points, squareWidth));
            break;
        case PrimitiveType::Rectangle:
            [[fallthrough]];
        default:
            double width, height;
            mainView->SetUpPreferencesForFigure(width, height);
            primitive = make_shared<Rectangle>(Rectangle(points, width, height));
    }
    primitives.push_back(primitive);
};

shared_ptr<Primitive> Processor::MatchUserSelectedPointToPrimitiveObject([[maybe_unused]]shared_ptr<Point> pPoint)
{
    return nullptr;
};

void Processor::DeletePrimitive_Handler(){
    shared_ptr<Point> pPoint = mainView->GetUserSelectionOnViewport();
    shared_ptr<Primitive> pFigure = MatchUserSelectedPointToPrimitiveObject(pPoint);
    mainView->DeleteFigureByPoints(pFigure->GetPoints());
};
