#include "std.h"
#include <QMessageBox>
#include "DictionaryDialog.h"
#include "ui_DictionaryDialog.h"


DictionaryDialog::DictionaryDialog(QWidget *parent, mydict::Dictionary& dictionary) :
    QDialog(parent),
    ui(new Ui::DictionaryDialog),
    m_dictionary(dictionary)
{
    ui->setupUi(this);

    QObject::connect(ui->btnOK, &QPushButton::clicked, this, &DictionaryDialog::accept);
    QObject::connect(ui->btnCancel, &QPushButton::clicked, this, &DictionaryDialog::reject);

    //
    ui->dictionaryWidget->setDictionary(dictionary);
}


DictionaryDialog::~DictionaryDialog() {
    delete ui;
}


bool DictionaryDialog::validate() {
    auto d = ui->dictionaryWidget->dictionary();
    return d.Name.size() != 0;
}


void DictionaryDialog::accept() {
    if (!validate()) {
        QMessageBox::warning(this, "Error", "Name must not be empty.");
        return;
    }

    m_dictionary = ui->dictionaryWidget->dictionary();
    QDialog::accept();
}


bool DictionaryDialog::editDictionary(QWidget *parent, mydict::Dictionary& dictionary) {
    std::unique_ptr<DictionaryDialog> dialog(new DictionaryDialog(parent, dictionary));
    return dialog->exec() == QDialog::Accepted;
}
