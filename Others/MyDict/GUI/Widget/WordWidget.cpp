#include "WordWidget.h"
#include "ui_WordWidget.h"

#include <cassert>


WordWidget::WordWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordWidget)
{
    ui->setupUi(this);
}


WordWidget::~WordWidget() = default;


void WordWidget::SetWord(const mydict::Word& word) {
    ui->textWord->setText(word.Name.c_str());
    ui->textDefinition->setText(word.Definition.c_str());
    ui->textRelated->setText(word.Related.c_str());
    ui->textExamples->setPlainText(word.Examples.c_str());
}


mydict::Word WordWidget::GetWord() const {
    mydict::Word word;
    word.Name = ui->textWord->text().toStdString();
    word.Definition = ui->textDefinition->text().toStdString();
    word.Related = ui->textRelated->text().toStdString();
    word.Examples = ui->textExamples->toPlainText().toStdString();
    return word;
}


void WordWidget::Clear() {
    ui->textWord->clear();
    ui->textDefinition->clear();
    ui->textRelated->clear();
    ui->textExamples->clear();
}
