#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include <memory>
#include <vector>
#include <QCloseEvent>
#include <QMainWindow>

#include "DB/Database.h"


namespace Ui {
    class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent, mydict::Database& db);
    ~MainWindow();


private slots:
    void OnEditSetting();

    void OnManageDictionary();
    void OnSelectDictionary(int index);

    void OnAddWord();
    void OnEditWord();
    void OnRemoveWord();

    void OnSearch();

    void OnImportDictionary();
    void OnExportDictionary();

    void OnViewHelp();
    void OnAbout();
    void OnAboutQt();


protected:
    virtual void closeEvent(QCloseEvent *event) override;


private:
    void ReloadDictionaryList();
    void ReloadWordList();


private: // data
    std::unique_ptr<Ui::MainWindow> ui;

    mydict::Database& m_db;
    int m_currentDictionaryId;
};


#endif // MAIN_WINDOW_H
