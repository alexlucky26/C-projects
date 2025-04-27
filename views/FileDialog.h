#pragma once
#include <memory>
#include <filesystem>

using namespace std;
/**
 * @class Dialog
 * @brief Базовый абстрактный класс диалоговых окон для импорта и экспорта файлов.
 * @details Это основа для View-диалоговых окон.
 */
class File {
    filesystem::path path;
};
using FileHandle = shared_ptr<File>;

// Window dialog to import/export a model
class Dialog {
public:
    Dialog() = default;
    virtual ~Dialog() = default;
    /**
     * @brief Показать диалоговое окно.
     * @return Успешность операции.
     */
    virtual bool show() = 0;
    /**
     * @brief Скрыть диалоговое окно.
     * @return Успешность операции.
     */
    virtual bool hide() = 0;
};

/**
 * @class ExportDialog
 * @brief Диалоговое окно для сохранения файлов.
 */
class ExportDialog : public Dialog {
public:
    ExportDialog() = default;
    bool show() override { return false; }
    bool hide() override { return false; }
    /**
     * @brief Сохранить файл.
     * @return Дескриптор файла.
     */
    FileHandle SaveFile() { return make_shared<File>(); }
};

/**
 * @class ImportDialog
 * @brief Диалоговое окно для загрузки файлов.
 */
class ImportDialog : public Dialog {
public:
    ImportDialog() = default;
    bool show() override { return false; }
    bool hide() override { return false; }
    /**
     * @brief Импортировать файл.
     * @return Дескриптор файла.
     */
    FileHandle ImportFile() { return make_shared<File>(); }
};

/**
 * @class InfoDialog
 * @brief Диалоговое окно с информацией для пользователя.
 */
class InfoDialog : public Dialog {
public:
    InfoDialog() = default;
    bool show() override { return false; }
    bool hide() override { return false; }
};