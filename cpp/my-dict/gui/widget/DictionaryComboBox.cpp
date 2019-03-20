#include "DictionaryComboBox.h"
#include "ui_DictionaryComboBox.h"

#include <cassert>
#include <stdexcept>
#include <exception>
#include <QMessageBox>


DictionaryComboBox::DictionaryComboBox(QWidget *parent) :
    QComboBox(parent),
    ui(new Ui::DictionaryComboBox)
{
    ui->setupUi(this);

    setMinimumWidth(200);
    QObject::connect(
        this, static_cast<void (QComboBox:: *)(int)>(&QComboBox::currentIndexChanged),
        this, &DictionaryComboBox::OnDictionaryChanged);
}


DictionaryComboBox::~DictionaryComboBox() = default;


void DictionaryComboBox::SetDictionaryList(const std::vector<mydict::Dictionary>& dictionaryList) {
    for (auto& dictionary : dictionaryList) {
        if (!mydict::IsIdInValidRange(dictionary.Id) || !mydict::IsDictionaryContentValid(dictionary))
            throw std::runtime_error("Invalid dictionary.");
    }

    //
    clear();
    for (auto& dictionary : dictionaryList)
        addItem(dictionary.Name.c_str(), dictionary.Id);

    //
    OnDictionaryChanged();
}


int64_t DictionaryComboBox::GetSelectedDictionaryId() const {
    auto id = currentData();
    if (id.isValid()) {
        assert(mydict::IsIdInValidRange(id.toInt()));
        return id.toInt();
    }
    else
        return mydict::InvalidId;
}


void DictionaryComboBox::OnDictionaryChanged() {
    emit DictionaryChanged(GetSelectedDictionaryId());
}
