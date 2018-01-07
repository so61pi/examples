#include "ManageDictionaryDialog.h"
#include "ui_ManageDictionaryDialog.h"

#include <memory>
#include <QMessageBox>

#include "AddDictionaryDialog.h"
#include "EditDictionaryDialog.h"


ManageDictionaryDialog::ManageDictionaryDialog(QWidget *parent, mydict::Database& db) :
    QDialog(parent),
    ui(new Ui::ManageDictionaryDialog),
    m_db(db)
{
    ui->setupUi(this);

    // initially hide number of words label
    ui->labelNumOfWords->hide();

    ui->dictionaryWidget->SetReadOnly(true);

    //
    QObject::connect(ui->btnAdd, &QPushButton::clicked, this, &ManageDictionaryDialog::OnAddDictionary);
    QObject::connect(ui->btnEdit, &QPushButton::clicked, this, &ManageDictionaryDialog::OnEditDictionary);
    QObject::connect(ui->btnRemove, &QPushButton::clicked, this, &ManageDictionaryDialog::OnRemoveDictionary);
    QObject::connect(ui->listDictionary, &DictionaryComboBox::DictionaryChanged, this, &ManageDictionaryDialog::OnDictionaryChanged);


    //
    ReloadDictionaryList();
}


ManageDictionaryDialog::~ManageDictionaryDialog() = default;


void ManageDictionaryDialog::OnAddDictionary() {
    try {
        auto addDictionaryDialog = std::make_unique<AddDictionaryDialog>(this);
        if (addDictionaryDialog->exec() == QDialog::Accepted) {
            auto dictionary = addDictionaryDialog->GetDictionary();
            m_db.AddDictionary(dictionary);
            ReloadDictionaryList();
        }
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void ManageDictionaryDialog::OnEditDictionary() {
    try {
        //
        auto dictionaryId = ui->listDictionary->GetSelectedDictionaryId();
        if (!mydict::IsIdInValidRange(dictionaryId)) {
            QMessageBox::warning(this, "Error", "Please choose a dictionary to edit.");
            return;
        }

        //
        auto dictionary = m_db.LoadDictionary(dictionaryId);
        auto editDictionaryDialog = std::make_unique<EditDictionaryDialog>(this, dictionary);
        if (editDictionaryDialog->exec() == QDialog::Accepted) {
            m_db.UpdateDictionary(dictionary);
            ReloadDictionaryList();
        }
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void ManageDictionaryDialog::OnRemoveDictionary() {
    try {
        //
        auto dictionaryId = ui->listDictionary->GetSelectedDictionaryId();
        if (!mydict::IsIdInValidRange(dictionaryId)) {
            QMessageBox::warning(this, "Error", "Please choose a dictionary to edit.");
            return;
        }

        //
        auto confirm = QMessageBox::warning(this, "Confirm", "Are you sure?", QMessageBox::Ok, QMessageBox::Cancel);
        if (confirm == QMessageBox::Cancel)
            return;

        //
        m_db.RemoveDictionary(dictionaryId);
        ReloadDictionaryList();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void ManageDictionaryDialog::OnDictionaryChanged(int dictionaryId) {
    try {
        if (!mydict::IsIdInValidRange(dictionaryId)) {
            ui->dictionaryWidget->Clear();

            ui->labelNumOfWords->hide();
            ui->labelNumOfWords->clear();

            return;
        }

        auto dictionary = m_db.LoadDictionary(dictionaryId);
        ui->dictionaryWidget->SetDictionary(dictionary);

        auto numOfWords = m_db.GetNumberOfWords(dictionaryId);
        ui->labelNumOfWords->setText(QString("Number of words: %1").arg(numOfWords));
        ui->labelNumOfWords->show();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void ManageDictionaryDialog::ReloadDictionaryList() {
    ui->listDictionary->SetDictionaryList(m_db.GetDictionaryList());
}
