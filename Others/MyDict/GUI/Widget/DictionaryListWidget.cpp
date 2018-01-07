#include "DictionaryListWidget.h"
#include "ui_DictionaryListWidget.h"

#include <cassert>
#include <stdexcept>
#include <QCheckBox>
#include <QVariant>


DictionaryListWidget::DictionaryListWidget(QWidget *parent) :
    QListWidget(parent),
    ui(new Ui::DictionaryListWidget)
{
    ui->setupUi(this);

    //
    QObject::connect(this, &QListWidget::itemClicked, this, &DictionaryListWidget::OnDictionaryClicked);
}


DictionaryListWidget::~DictionaryListWidget() = default;


void DictionaryListWidget::AddDictionary(const mydict::Dictionary& dictionary) {
    if (!mydict::IsIdInValidRange(dictionary.Id) || !mydict::IsDictionaryContentValid(dictionary))
        throw std::runtime_error("Invalid dictionary.");

    //
    auto item = new QListWidgetItem();
    item->setData(QListWidgetItem::UserType, dictionary.Id);

    //
    auto checkBox = new QCheckBox(dictionary.Name.c_str());

    //
    addItem(item);
    setItemWidget(item, checkBox);
}


void DictionaryListWidget::SetDictionaryList(const std::vector<mydict::Dictionary>& dictionaryList) {
    clear();

    //
    for (auto& dictionary : dictionaryList) {
        if (!mydict::IsIdInValidRange(dictionary.Id) || !mydict::IsDictionaryContentValid(dictionary))
            throw std::runtime_error("Invalid dictionary.");
    }

    for (auto& dictionary : dictionaryList) {
        AddDictionary(dictionary);
    }
}


std::vector<int64_t> DictionaryListWidget::GetSelectedDictionaryIdList() {
    std::vector<int64_t> selectedDictionaryIds;

    // loop through all QListWidgetItem items
    for (auto i = 0, end = count(); i < end; ++i) {
        // get QListWidgetItem at position i
        auto widgetItem = item(i);

        // get associated dictionary id
        auto dictionaryId = widgetItem->data(QListWidgetItem::UserType).toLongLong();

        assert(mydict::IsIdInValidRange(dictionaryId));

        // get associated QCheckBox
        auto checkBox = dynamic_cast<QCheckBox *>(itemWidget(widgetItem));

        if (checkBox && checkBox->isChecked())
            selectedDictionaryIds.push_back(dictionaryId);
    }

    return selectedDictionaryIds;
}


void DictionaryListWidget::OnDictionaryClicked(QListWidgetItem *item) {
    assert(item != nullptr);

    auto itemData = item->data(QListWidgetItem::UserType);
    auto dictionaryId = itemData.toLongLong();
    assert(mydict::IsIdInValidRange(dictionaryId));
    emit DictionaryClicked(dictionaryId);
}
