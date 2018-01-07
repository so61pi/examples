#ifndef DICTIONARY_COMBOBOX_H
#define DICTIONARY_COMBOBOX_H


#include <memory>
#include <QComboBox>

#include "DB/Database.h"


namespace Ui {
    class DictionaryComboBox;
};


class DictionaryComboBox : public QComboBox
{
    Q_OBJECT


public:
    explicit DictionaryComboBox(QWidget *parent = 0);
    ~DictionaryComboBox();

    void SetDictionaryList(const std::vector<mydict::Dictionary>& dictionaryList);
    int64_t GetSelectedDictionaryId() const;


signals:
    void DictionaryChanged(int64_t dictionaryId);


private slots:
    void OnDictionaryChanged();


private: // data
    std::unique_ptr<Ui::DictionaryComboBox> ui;
};


#endif // DICTIONARY_COMBOBOX_H
