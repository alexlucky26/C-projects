#include "Processor.h"
#include <iostream>
#include <functional>
#include <map>
#include <string>

using namespace std;
/**
 * @class GUI_API
 * @brief Mock-класс для обработки событий от пользовательского интерфейса.
 * @details При нажатии пользователем на кнопку в GUI генерируется событие, которое перехватывается GUI_API и передаётся соответствующему обработчику в Processor.
 */
class GUI_API {
	map<string, function<void()>> common_listeners;
	function<void(PrimitiveType)> create_primitive_listener;
public:
	GUI_API() {}

	/**
     * @brief Добавить обработчик событий создания фигуры с параметром - типом фигуры.
     * @param name Имя события.
     * @param listener Функция-обработчик для создания фигуры.
     */
	void AddListener([[maybe_unused]] string name, function<void(PrimitiveType)> listener) {
		create_primitive_listener = move(listener);
	}

	/**
     * @brief Добавить обработчик событий создания фигуры (без параметров).
     * @param name Имя события.
     * @param listener Функция-обработчик для создания фигуры.
     */
	void AddListener(string name, function<void()> listener) {
		common_listeners.insert_or_assign(name, move(listener));
	}

	/**
     * @brief Обработать нажатие на кнопку "Новый документ".
     */
	void OnBtnClick_NewDocument() {
		const auto& it = common_listeners.find("NewDocument");
		if (it != common_listeners.end())
			it->second();
	}

	/**
     * @brief Обработать нажатие на кнопку "Импорт файла".
     */
	void OnBtnClick_ImportFile() {
		const auto& it = common_listeners.find("ImportFile");
		if (it != common_listeners.end())
			it->second();
	}

	/**
     * @brief Обработать нажатие на кнопку "Экспорт файла".
     */
	void OnBtnClick_ExportFile() {
		const auto& it = common_listeners.find("ExportFile");
		if (it != common_listeners.end())
			it->second();
	}

	/**
     * @brief Обработать нажатие на кнопку создания фигуры типа Circle. Планируется, что в UI будет выбор Create a figure и выпадает popup с типом фигуры.
     */
	void OnBtnClick_CreateFigure_Circle() {
		create_primitive_listener(PrimitiveType::Circle);
	}
	/**
     * @brief Обработать нажатие на кнопку создания фигуры типа Square. Планируется, что в UI будет выбор Create a figure и выпадает popup с типом фигуры.
     */
	void OnBtnClick_CreateFigure_Square() {
		create_primitive_listener(PrimitiveType::Square);
	}
	/**
     * @brief Обработать нажатие на кнопку создания фигуры типа Rectangle. Планируется, что в UI будет выбор Create a figure и выпадает popup с типом фигуры.
     */
	void OnBtnClick_CreateFigure_Rectangle() {
		create_primitive_listener(PrimitiveType::Rectangle);
	}
	/**
     * @brief Обработать нажатие на кнопку удаления фигуры.
     */
	void OnBtnClick_DeleteFigure() {
		const auto& it = common_listeners.find("DeleteFigure");
		if (it != common_listeners.end())
			it->second();
	}
};

int main(int, char **) {
	GUI_API gui;
	Processor processor;
	gui.AddListener("NewDocument", [&processor](){ processor.NewDocument_Handler();});
	gui.AddListener("ImportFile", [&processor](){ processor.ImportFile_Handler();});
	gui.AddListener("ExportFile", [&processor](){ processor.ExportFile_Handler();});
	gui.AddListener("CreatePrimitive", [&processor](PrimitiveType type){ processor.CreatePrimitive_Handler(type);});
	gui.AddListener("DeleteFigure", [&processor](){ processor.DeletePrimitive_Handler();});

	return 0;
}