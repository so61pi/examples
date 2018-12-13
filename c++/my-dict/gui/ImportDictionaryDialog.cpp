#include "ImportDictionaryDialog.h"
#include "ui_ImportDictionaryDialog.h"

#include <cassert>
#include <QFileDialog>
#include <QMessageBox>

#include "Db/Database.h"


ImportDictionaryDialog::ImportDictionaryDialog(QWidget *parent, mydict::Database& db) :
    QDialog(parent),
    ui(new Ui::ImportDictionaryDialog),
    m_db(db)
{
    ui->setupUi(this);

    // initially hide number of words label
    ui->labelNumOfWords->hide();

    //
    QObject::connect(ui->btnOK, &QPushButton::clicked, this, &ImportDictionaryDialog::OnOK);
    QObject::connect(ui->btnCancel, &QPushButton::clicked, this, &ImportDictionaryDialog::reject);
    QObject::connect(ui->btnSelectPath, &QPushButton::clicked, this, &ImportDictionaryDialog::OnSelectPath);
    QObject::connect(ui->dictionaryListWidget, &DictionaryListWidget::DictionaryClicked, this, &ImportDictionaryDialog::OnDictionaryClicked);
}


ImportDictionaryDialog::~ImportDictionaryDialog() = default;


void ImportDictionaryDialog::OnSelectPath() {
    try {
        // get database path from user
        auto path = QFileDialog::getOpenFileName(this, "Database", "", "MydictDB (*.db)");
        if (path.size() != 0) {
            ui->textPath->clear();
            ui->dictionaryListWidget->clear();

            // load database
            m_newDb.reset();
            m_newDb = std::make_unique<mydict::Database>(path.toStdString());

            // check if the database is valid
            if (!mydict::IsDatabaseValid(*m_newDb)) {
                QMessageBox::warning(this, "Error", "The chosen database is not valid.");
                return;
            }

            // display all dictionaries from loaded database
            ui->textPath->setText(path);
            ui->dictionaryListWidget->SetDictionaryList(m_newDb->GetDictionaryList());
        }
    }
    catch (const std::exception& e) {
        ui->textPath->clear();
        ui->dictionaryListWidget->clear();
        QMessageBox::critical(this, "Error", e.what());
    }
}


void ImportDictionaryDialog::OnDictionaryClicked(int64_t dictionaryId) {
    try {
        assert(m_newDb && "Database must be already available here.");

        // clear dictionary name & description if dictionaryId is not valid
        if (!mydict::IsIdInValidRange(dictionaryId)) {
            ui->dictionaryWidget->Clear();

            ui->labelNumOfWords->hide();
            ui->labelNumOfWords->clear();

            return;
        }

        auto dictionary = m_newDb->LoadDictionary(dictionaryId);
        ui->dictionaryWidget->SetDictionary(dictionary);

        auto numOfWords = m_db.GetNumberOfWords(dictionaryId);
        ui->labelNumOfWords->setText(QString("Number of words: %1").arg(numOfWords));
        ui->labelNumOfWords->show();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void ImportDictionaryDialog::OnOK() {
    try {
        // inform if user didn't choose any database
        if (!m_newDb) {
            QMessageBox::information(this, "Error", "Please choose a database.");
            return;
        }

        // inform if user didn't choose any dictionary to import
        auto dictionaryIdList = ui->dictionaryListWidget->GetSelectedDictionaryIdList();
        if (dictionaryIdList.size() == 0) {
            QMessageBox::information(this, "Error", "Please choose at least 1 dictionary.");
            return;
        }

        // import all chosen dictionaries
        for (auto& dictionaryId : dictionaryIdList) {
            auto dictionary = m_newDb->LoadDictionary(dictionaryId);
            auto wordList = m_newDb->GetWordList(dictionary.Id);

            mydict::Import(m_db, dictionary, wordList);
        }

        QMessageBox::information(this, "Information", "Import completed successfully.");

        // close dialog
        accept();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}
