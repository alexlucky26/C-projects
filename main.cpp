#include "Processor.h"
#include <iostream>
#include <functional>
#include <map>
#include <string>

using namespace std;

/// When a User clicks somewhere in GUI, it will generate an Event for a listener! This event is handled by the Processor
class GUI_API {
	map<string, function<void()>> common_listeners;
	function<void(PrimitiveType)> create_primitive_listener;
public:
	GUI_API() {};

	void AddListener([[maybe_unused]] string name, function<void(PrimitiveType)> listener) {
		create_primitive_listener = move(listener);
	}

	void AddListener(string name, function<void()> listener) {
		common_listeners.insert_or_assign(name, move(listener));
	}

	void OnBtnClick_NewDocument() {
		const auto& it = common_listeners.find("NewDocument");
		if (it != common_listeners.end())
			it->second();
	};
	void OnBtnClick_ImportFile() {
		const auto& it = common_listeners.find("ImportFile");
		if (it != common_listeners.end())
			it->second();
	};
	void OnBtnClick_ExportFile() {
		const auto& it = common_listeners.find("ExportFile");
		if (it != common_listeners.end())
			it->second();
	};

	// When a user clicks on CreateFigure, a popup window appears to select a figure -> Circle, Square or Rectangle. That's why here is 3 calls
	void OnBtnClick_CreateFigure_Circle() {
		create_primitive_listener(PrimitiveType::Circle);
	};
	void OnBtnClick_CreateFigure_Square() {
		create_primitive_listener(PrimitiveType::Square);
	};
	void OnBtnClick_CreateFigure_Rectangle() {
		create_primitive_listener(PrimitiveType::Rectangle);
	};
	void OnBtnClick_DeleteFigure() {
		const auto& it = common_listeners.find("DeleteFigure");
		if (it != common_listeners.end())
			it->second();
	};
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