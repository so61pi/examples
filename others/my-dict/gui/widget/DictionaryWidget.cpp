#include "ui_DictionaryWidget.h"
#include "DictionaryWidget.h"

#include <cassert>


DictionaryWidget::DictionaryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DictionaryWidget)
{
    ui->setupUi(this);
}


DictionaryWidget::~DictionaryWidget() = default;


void DictionaryWidget::SetReadOnly(bool isReadOnly) {
    ui->textName->setReadOnly(isReadOnly);
    ui->textDescription->setReadOnly(isReadOnly);
}


void DictionaryWidget::SetDictionary(const mydict::Dictionary& dictionary) {
    ui->textName->setText(dictionary.Name.c_str());
    ui->textDescription->setPlainText(dictionary.Description.c_str());
}


mydict::Dictionary DictionaryWidget::GetDictionary() const {
    mydict::Dictionary dictionary;
    dictionary.Name = ui->textName->text().toStdString();
    dictionary.Description = ui->textDescription->toPlainText().toStdString();
    return dictionary;
}


void DictionaryWidget::Clear() {
    ui->textName->clear();
    ui->textDescription->clear();
}
