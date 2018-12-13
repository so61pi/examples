#include "EditDictionaryDialog.h"
#include "ui_EditDictionaryDialog.h"

#include <exception>
#include <QMessageBox>


EditDictionaryDialog::EditDictionaryDialog(QWidget *parent, mydict::Dictionary& dictionary) :
    QDialog(parent),
    ui(new Ui::EditDictionaryDialog),
    m_dictionary(dictionary)
{
    ui->setupUi(this);

    //
    QObject::connect(ui->btnOK, &QPushButton::clicked, this, &EditDictionaryDialog::OnOK);
    QObject::connect(ui->btnCancel, &QPushButton::clicked, this, &EditDictionaryDialog::reject);

    //
    ui->dictionaryWidget->SetDictionary(m_dictionary);
}


EditDictionaryDialog::~EditDictionaryDialog() = default;


void EditDictionaryDialog::OnOK() {
    try {
        auto dictionary = ui->dictionaryWidget->GetDictionary();
        if (!mydict::IsDictionaryContentValid(dictionary)) {
            QMessageBox::warning(this, "Error", "Name must not be empty.");
            return;
        }

        m_dictionary.Name = dictionary.Name;
        m_dictionary.Description = dictionary.Description;
        
        accept();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}
