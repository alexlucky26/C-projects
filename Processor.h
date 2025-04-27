#pragma once
#include <memory>
#include "Primitive.h"
#include "views/FileDialog.h"
#include "views/Viewport.h"

using namespace std;

/**
 * @class Processor
 * @brief Класс-контроллер, обрабатывающий пользовательские действия, пришедшими через обработчики событий в GUI_API, и взаимодействующий с представлениями (View).
 * @details Получает команды от GUI_API и вызывает соответствующие функции для работы с Viewport и диалогами.
 */
class Processor {
    unique_ptr<Viewport> mainView;
    /**
     * @brief Функция, возвращающая фигуру на основе выбранной пользователем точки.
     * @details Пользователь делает клик на Viewport'e. Данная функция анализирует местоположение всех созданных фигур и выдает соответствующую точке фигуру.
     * @param pPoint Точка фигуры на Viewport'e, выбранная пользователем.
     */
    shared_ptr<Primitive> MatchUserSelectedPointToPrimitiveObject(shared_ptr<Point> pPoint);
    vector<shared_ptr<Primitive>> primitives; // Все примитивные фигуры, нарисованные на Viewport'e

    FileHandle CreateSelectFileDialog(); // return selected file ptr
    FileHandle CreateSaveFileDialog(); // returns exported file
    void CallInfoDialog(); // saved/exported/imported/opened
public:
    Processor();
    ~Processor();
    /**
     * @brief Обработчик события импорта файла.
     */
    void ImportFile_Handler();
    /**
     * @brief Обработчик события экспорта файла.
     */
	void ExportFile_Handler();
    /**
     * @brief Обработчик создания нового документа.
     */
    void NewDocument_Handler();

    /**
     * @brief Обработчик создания новой примитивной фигуры.
     * @param primitiveType Тип создаваемой фигуры.
     */
    void CreatePrimitive_Handler(PrimitiveType primitiveType = PrimitiveType::Rectangle);
    /**
     * @brief Обработчик удаления выбранной фигуры.
     */
    void DeletePrimitive_Handler();
};

