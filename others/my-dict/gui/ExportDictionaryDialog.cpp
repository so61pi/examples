#include "ExportDictionaryDialog.h"
#include "ui_ExportDictionaryDialog.h"

#include <exception>
#include <QFileDialog>
#include <QMessageBox>


ExportDictionaryDialog::ExportDictionaryDialog(QWidget *parent, mydict::Database& db) :
    QDialog(parent),
    ui(new Ui::ExportDictionaryDialog),
    m_db(db)
{
    ui->setupUi(this);

    // initially hide number of words label
    ui->labelNumOfWords->hide();

    QObject::connect(ui->btnOK, &QPushButton::clicked, this, &ExportDictionaryDialog::OnOK);
    QObject::connect(ui->btnCancel, &QPushButton::clicked, this, &ExportDictionaryDialog::reject);
    QObject::connect(ui->btnSelectPath, &QPushButton::clicked, this, &ExportDictionaryDialog::OnSelectPath);
    QObject::connect(ui->dictionaryListWidget, &DictionaryListWidget::DictionaryClicked, this, &ExportDictionaryDialog::OnDictionaryClicked);

    ui->dictionaryListWidget->SetDictionaryList(m_db.GetDictionaryList());
}


ExportDictionaryDialog::~ExportDictionaryDialog() = default;


void ExportDictionaryDialog::OnSelectPath() {
    try {
        auto path = QFileDialog::getSaveFileName(this, "Save As", "", "MydictDB (*.db)");
        ui->textPath->setText(path);
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void ExportDictionaryDialog::OnDictionaryClicked(int64_t dictionaryId) {
    try {
        if (!mydict::IsIdInValidRange(dictionaryId)) {
            ui->dictionaryWidget->Clear();

            ui->labelNumOfWords->hide();
            ui->labelNumOfWords->clear();

            return;
        }

        ui->dictionaryWidget->SetDictionary(m_db.LoadDictionary(dictionaryId));

        auto numOfWords = m_db.GetNumberOfWords(dictionaryId);
        ui->labelNumOfWords->setText(QString("Number of words: %1").arg(numOfWords));
        ui->labelNumOfWords->show();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void ExportDictionaryDialog::OnOK() {
    try {
        auto path = ui->textPath->text();
        if (path.size() == 0) {
            QMessageBox::information(this, "Info", "Please select path to save.");
            return;
        }

        auto selected = ui->dictionaryListWidget->GetSelectedDictionaryIdList();
        if (selected.size() == 0) {
            QMessageBox::information(this, "Info", "Please select at least 1 dictionary.");
            return;
        }

        mydict::Export(path.toStdString(), m_db, selected);

        QMessageBox::information(this, "Information", "Export completed successfully.");

        // close dialog
        accept();
    }
    catch (const std::exception& e) {
        ui->textPath->clear();
        ui->dictionaryListWidget->clear();
        QMessageBox::critical(this, "Error", e.what());
    }
}
