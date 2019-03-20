#include "AddDictionaryDialog.h"
#include "ui_AddDictionaryDialog.h"

#include <exception>
#include <QMessageBox>


AddDictionaryDialog::AddDictionaryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDictionaryDialog)
{
    ui->setupUi(this);

    //
    QObject::connect(ui->btnOK, &QPushButton::clicked, this, &AddDictionaryDialog::OnOK);
    QObject::connect(ui->btnCancel, &QPushButton::clicked, this, &AddDictionaryDialog::reject);
}


AddDictionaryDialog::~AddDictionaryDialog() = default;


mydict::Dictionary AddDictionaryDialog::GetDictionary() const {
    return ui->dictionaryWidget->GetDictionary();
}


void AddDictionaryDialog::OnOK() {
    try {
        auto dictionary = ui->dictionaryWidget->GetDictionary();
        if (!mydict::IsDictionaryContentValid(dictionary)) {
            QMessageBox::warning(this, "Error", "Name must not be empty.");
            return;
        }

        accept();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}
