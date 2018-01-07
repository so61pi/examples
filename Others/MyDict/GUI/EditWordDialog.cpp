#include "EditWordDialog.h"
#include "ui_EditWordDialog.h"

#include <exception>
#include <QMessageBox>


EditWordDialog::EditWordDialog(QWidget *parent, mydict::Word& word) :
    QDialog(parent),
    ui(new Ui::EditWordDialog),
    m_word(word)
{
    ui->setupUi(this);

    //
    QObject::connect(ui->btnOK, &QPushButton::clicked, this, &EditWordDialog::OnOK);
    QObject::connect(ui->btnCancel, &QPushButton::clicked, this, &EditWordDialog::reject);

    //
    ui->wordWidget->SetWord(m_word);
}


EditWordDialog::~EditWordDialog() = default;


void EditWordDialog::OnOK() {
    try {
        auto word = ui->wordWidget->GetWord();
        if (!mydict::IsWordContentValid(word)) {
            QMessageBox::warning(this, "Error", "Word or examples must not be empty.");
            return;
        }

        m_word.Name = word.Name;
        m_word.Definition = word.Definition;
        m_word.Related = word.Related;
        m_word.Examples = word.Examples;
        
        accept();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}
