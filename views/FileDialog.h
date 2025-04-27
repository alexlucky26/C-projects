#pragma once
#include <memory>
#include <filesystem>

using namespace std;
// The File that contains some primitive figures for import
class File {
    filesystem::path path;
};
using FileHandle = shared_ptr<File>;

// Window dialog to import/export a model
class Dialog {
public:
    Dialog() = default;
    virtual ~Dialog() = default;
    virtual bool show() = 0;
    virtual bool hide() = 0;
};

class ExportDialog : public Dialog {
public:
    ExportDialog() = default;
    bool show() override { return false; };
    bool hide() override { return false; };
    FileHandle SaveFile() { return make_shared<File>(); };
};

class ImportDialog : public Dialog {
public:
    ImportDialog() = default;
    bool show() override { return false; };
    bool hide() override { return false; };
    FileHandle ImportFile() { return make_shared<File>(); };
};

class InfoDialog : public Dialog {
public:
    InfoDialog() = default;
    bool show() override { return false; };
    bool hide() override { return false; };
};