#include "WordListWidget.h"
#include "ui_WordListWidget.h"

#include <cassert>
#include <stdexcept>


Q_DECLARE_METATYPE(mydict::Word)


WordListWidget::WordListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordListWidget)
{
    ui->setupUi(this);

    // hide column 0
    ui->listWord->setColumnHidden(0, true);

    QObject::connect(ui->listWord, &QTreeWidget::itemClicked, this, &WordListWidget::OnWordClicked);
}


WordListWidget::~WordListWidget() = default;


void WordListWidget::SetWordList(const std::vector<mydict::Word>& wordList) {
    for (auto& word : wordList) {
        if (!mydict::IsIdInValidRange(word.Id) || !mydict::IsWordContentValid(word))
            throw std::runtime_error("Invalid word.");
    }

    Clear();
    for (auto& word : wordList) {
        QVariant itemData;
        itemData.setValue(word);

        auto *item = new QTreeWidgetItem();
        
        item->setData(0, QTreeWidgetItem::UserType, itemData);
        item->setText(1, word.Name.c_str()); // NAME
        item->setText(2, word.Definition.c_str()); // DEFINITION
        ui->listWord->addTopLevelItem(item);
    }

    QString numOfWords = QString("Number of words: %1").arg(wordList.size());
    ui->labelNumOfWords->setText(numOfWords);
}


void WordListWidget::Clear() {
    ui->listWord->clear();
    ui->textRelated->clear();
    ui->textExamples->clear();
}


int64_t WordListWidget::GetSelectedWordId() const {
    auto currentItem = ui->listWord->currentItem();
    if (currentItem) {
        auto itemData = currentItem->data(0, QTreeWidgetItem::UserType);
        auto word = itemData.value<mydict::Word>();
        assert(mydict::IsIdInValidRange(word.Id));
        return word.Id;
    }
    else
        return mydict::InvalidId;
}


void WordListWidget::OnWordClicked(QTreeWidgetItem *item, int /*column*/) {
    assert(item != nullptr);

    auto itemData = item->data(0, QTreeWidgetItem::UserType);
    auto word = itemData.value<mydict::Word>();

    assert(mydict::IsIdInValidRange(word.Id) && mydict::IsWordContentValid(word));

    ui->textRelated->setText(word.Related.c_str());
    ui->textExamples->setPlainText(word.Examples.c_str());
}
