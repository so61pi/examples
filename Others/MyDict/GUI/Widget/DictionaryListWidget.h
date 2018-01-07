#ifndef DICTIONARY_LIST_WIDGET_H
#define DICTIONARY_LIST_WIDGET_H


#include <memory>
#include <vector>
#include <QListWidget>
#include <QListWidgetItem>

#include "DB/Database.h"


namespace Ui {
    class DictionaryListWidget;
}


class DictionaryListWidget : public QListWidget
{
    Q_OBJECT


public:
    explicit DictionaryListWidget(QWidget *parent);
    ~DictionaryListWidget();

    void AddDictionary(const mydict::Dictionary& dictionary);
    void SetDictionaryList(const std::vector<mydict::Dictionary>& dictionaryList);
    std::vector<int64_t> GetSelectedDictionaryIdList();


signals:
    void DictionaryClicked(int64_t dictionaryId);


private slots:
    void OnDictionaryClicked(QListWidgetItem *item);


private: // data
    std::unique_ptr<Ui::DictionaryListWidget> ui;
};


#endif // DICTIONARY_LIST_WIDGET_H
