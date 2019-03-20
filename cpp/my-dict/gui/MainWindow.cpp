#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QAction>
#include <QCloseEvent>
#include <QDialog>
#include <QMessageBox>

#include "AddWordDialog.h"
#include "EditWordDialog.h"
#include "ManageDictionaryDialog.h"
#include "ImportDictionaryDialog.h"
#include "ExportDictionaryDialog.h"

#include "DB/Database.h"


MainWindow::MainWindow(QWidget *parent, mydict::Database& db) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_db(db)
{
    ui->setupUi(this);

    // add a combo box to contain dictionaries list
    ui->mainToolBar->addWidget(ui->listDictionary);

    QObject::connect(ui->listDictionary, &DictionaryComboBox::DictionaryChanged, this, &MainWindow::OnSelectDictionary);
    QObject::connect(ui->textSearch, &QLineEdit::textChanged, this, &MainWindow::OnSearch);
    QObject::connect(ui->actionEditSetting, &QAction::triggered, this, &MainWindow::OnEditSetting);
    QObject::connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    QObject::connect(ui->actionManageDictionary, &QAction::triggered, this, &MainWindow::OnManageDictionary);
    QObject::connect(ui->actionImportDictionary, &QAction::triggered, this, &MainWindow::OnImportDictionary);
    QObject::connect(ui->actionExportDictionary, &QAction::triggered, this, &MainWindow::OnExportDictionary);
    QObject::connect(ui->actionViewHelp, &QAction::triggered, this, &MainWindow::OnViewHelp);
    QObject::connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::OnAbout);
    QObject::connect(ui->actionAboutQt, &QAction::triggered, this, &MainWindow::OnAboutQt);
    QObject::connect(ui->actionAddWord, &QAction::triggered, this, &MainWindow::OnAddWord);
    QObject::connect(ui->actionEditWord, &QAction::triggered, this, &MainWindow::OnEditWord);
    QObject::connect(ui->actionRemoveWord, &QAction::triggered, this, &MainWindow::OnRemoveWord);

    //
    ReloadDictionaryList();
}


MainWindow::~MainWindow() = default;


void MainWindow::ReloadDictionaryList() {
    ui->listDictionary->SetDictionaryList(m_db.GetDictionaryList());
    ReloadWordList();
}


void MainWindow::ReloadWordList() {
    if (!mydict::IsIdInValidRange(m_currentDictionaryId)) {
        ui->listWord->Clear();
    }
    else {
        ui->listWord->SetWordList(m_db.Search(m_currentDictionaryId, ui->textSearch->text().toStdString()));
    }
}


void MainWindow::closeEvent(QCloseEvent *event) {
    auto confirm = QMessageBox::question(this,"Confirm", "Are you really want to exit?", QMessageBox::Yes, QMessageBox::No);
    if (confirm == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}


void MainWindow::OnEditSetting() {
    QMessageBox::information(this, "Error", "This feature is not implemented yet.");
}


void MainWindow::OnManageDictionary() {
    auto manageDialog = std::make_unique<ManageDictionaryDialog>(this, m_db);
    manageDialog->exec();
    ReloadDictionaryList();
}


void MainWindow::OnSelectDictionary(int dictionaryId) {
    m_currentDictionaryId = dictionaryId;
    ReloadWordList();
}


void MainWindow::OnAddWord() {
    try {
        auto addWordDialog = std::make_unique<AddWordDialog>(this);
        if (addWordDialog->exec() == QDialog::Accepted) {
            auto dictionaryId = ui->listDictionary->GetSelectedDictionaryId();
            auto word = addWordDialog->GetWord();
            m_db.AddWord(dictionaryId, word);
            ReloadWordList();
        }
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::OnEditWord() {
    try {
        auto wordId = ui->listWord->GetSelectedWordId();
        if (!mydict::IsIdInValidRange(wordId)) {
            QMessageBox::warning(this, "Error", "Please select a word to edit.");
            return;
        }

        auto word = m_db.LoadWord(wordId);
        auto editWordDialog = std::make_unique<EditWordDialog>(this, word);
        if (editWordDialog->exec() == QDialog::Accepted) {
            auto dictionaryId = ui->listDictionary->GetSelectedDictionaryId();
            m_db.UpdateWord(word);
            ReloadWordList();
        }
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::OnRemoveWord() {
    try {
        auto wordId = ui->listWord->GetSelectedWordId();
        if (!mydict::IsIdInValidRange(wordId)) {
            QMessageBox::warning(this, "Error", "Please select a word to remove.");
            return;
        }

        auto confirm = QMessageBox::warning(this, "Confirm", "Are you sure?", QMessageBox::Ok, QMessageBox::Cancel);
        if (confirm == QMessageBox::Cancel)
            return;

        m_db.RemoveWord(wordId);
        ReloadWordList();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::OnSearch() {
    try {
        ReloadWordList();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::OnImportDictionary() {
    try {
        auto importDialog = std::make_unique<ImportDictionaryDialog>(this, m_db);
        importDialog->exec();
        ReloadDictionaryList();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::OnExportDictionary() {
    try {
        auto exportDialog = std::make_unique<ExportDictionaryDialog>(this, m_db);
        exportDialog->exec();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void MainWindow::OnViewHelp() {
    QString helpContent =
        "This is your help.<br /><br />";

    QMessageBox aboutMsgBox(this);
    aboutMsgBox.setWindowTitle("Help");
    aboutMsgBox.setText(helpContent);
    aboutMsgBox.setTextFormat(Qt::RichText);
    aboutMsgBox.exec();
}


void MainWindow::OnAbout() {
    QMessageBox::about(this, "About MyDict", "MyDict");
}


void MainWindow::OnAboutQt() {
    QMessageBox::aboutQt(this, "AboutQt");
}
