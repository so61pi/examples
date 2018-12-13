#ifndef DICTIONARY_WIDGET_H
#define DICTIONARY_WIDGET_H


#include <memory>
#include <QWidget>

#include "DB/Database.h"


namespace Ui {
    class DictionaryWidget;
}


class DictionaryWidget : public QWidget
{
    Q_OBJECT


public:
    explicit DictionaryWidget(QWidget *parent);
    ~DictionaryWidget();

    void SetReadOnly(bool isReadOnly);

    void SetDictionary(const mydict::Dictionary& dictionary);
    mydict::Dictionary GetDictionary() const;
    
    void Clear();


private: // data
    std::unique_ptr<Ui::DictionaryWidget> ui;
};


#endif // DICTIONARY_WIDGET_H
