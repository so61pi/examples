#include "AddWordDialog.h"
#include "ui_AddWordDialog.h"

#include <exception>
#include <QMessageBox>


AddWordDialog::AddWordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddWordDialog)
{
    ui->setupUi(this);

    //
    QObject::connect(ui->btnOK, &QPushButton::clicked, this, &AddWordDialog::OnOK);
    QObject::connect(ui->btnCancel, &QPushButton::clicked, this, &AddWordDialog::reject);
}


AddWordDialog::~AddWordDialog() = default;


mydict::Word AddWordDialog::GetWord() const {
    return ui->wordWidget->GetWord();
}


void AddWordDialog::OnOK() {
    try {
        auto word = ui->wordWidget->GetWord();
        if (!mydict::IsWordContentValid(word)) {
            QMessageBox::warning(this, "Error", "Word or examples must not be empty.");
            return;
        }

        accept();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}
