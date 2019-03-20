#include <exception>
#include <memory>
#include <QApplication>
#include <QMessageBox>

#include "DB/Database.h"
#include "MainWindow.h"


int main(int argc, char *argv[]) {
    try {
        QApplication a(argc, argv);

        mydict::Database db("dictionary.db");
        QIcon icon("icons/dictionary.png");

        auto mainWindow = std::make_unique<MainWindow>(nullptr, db);
        mainWindow->setWindowIcon(icon);
        mainWindow->showMaximized();

        return a.exec();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Error", e.what());
    }
    catch (...) {
        QMessageBox::critical(nullptr, "Error", "Unknown error");
    }
}
